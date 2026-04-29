#include <cmath>
#include <cwctype>
#include <chrono>

#include "Math.h"
#include "StringUtil.h"
#include "FileWin.h"
#include "WindowWin.h"
#include "ParseEnum.h"
#include "Animator.h"
#include "EasingWindow.h"

namespace Shared
{
	bool initialized = false;
	std::wstring filePath = L"";
}

//-----------------------------------//
//             RAINMETER             //
//-----------------------------------//

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	if (!Shared::initialized)
	{
		Shared::initialized = true;
		Shared::filePath = DuplicateBackslashes(RmReplaceVariables(rm, L"#SETTINGSPATH#\\Rainmeter.ini"), 2);
	}

	Measure* measure = new Measure(rm);
	*data = measure;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;

	measure->ReadOptions();
}

PLUGIN_EXPORT double Update(void* data)
{
	return 0.0;
}

PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
{
	Measure* measure = (Measure*)data;

	measure->ReadCommand(args);
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	delete measure;
}

//-----------------------------------//
//              MEASURE              //
//-----------------------------------//

Measure::Measure(void* rm)
{
	m_Rm = rm;
	m_Skin = RmGetSkin(rm);
	m_Hwnd = RmGetSkinWindow(rm);
}

Measure::~Measure()
{
	Stop();
}

//-----------------------------------//
//              ANIMATOR             //
//-----------------------------------//

void Measure::Start(bool toEnd)
{
	RECT rc;
	GetWindowRect(m_Hwnd, &rc);

	PointInt start{ rc.left, rc.top };
	PointInt userTarget = toEnd ? m_Config.End() : m_Config.Start();
	PointInt animTarget = userTarget - m_Config.Anchor();

	if (start == animTarget)
		return;

	Animation anim{};
	anim.started = false;
	anim.finished = false;
	anim.aborted = false;
	anim.elapsed = 0.0;
	//anim.updates = 0;
	anim.coords = ConvertToDouble(start);
	anim.velocity = { 0.0, 0.0 };
	anim.m_Rm = this->m_Rm;
	anim.m_Skin = this->m_Skin;
	anim.m_Hwnd = this->m_Hwnd;
	anim.m_Motion = this->m_Motion;
	anim.m_MotionParams = this->m_MotionParams;

	anim.isDynamic = this->m_Motion == MOTION::SPRING;
	anim.isMouseActions = this->m_Config.MouseAction();
	//anim.isOnlyBangs = this->m_Config.OnlyBangs();
	anim.start = start;
	anim.userTarget = userTarget;
	anim.animTarget = animTarget;
	anim.duration = this->m_Config.Duration() / 1000.0;
	//anim.bangInterval = m_Config.BangInterval();
	anim.coords = { double(start.x), double(start.y) };
	anim.ease = GetEaseFn(this->m_Easing);
	anim.motion = GetMotionFn(this->m_Motion);
	anim.action[0] = m_Actions[(uint8_t)m_Command-(uint8_t)COMMAND::START][(uint8_t)STATE::BEGIN].c_str();
	anim.action[1] = m_Actions[(uint8_t)m_Command-(uint8_t)COMMAND::START][(uint8_t)STATE::COMPLETE].c_str();

	AnimationManager::Instance().Add(std::move(anim));
}

void Measure::Stop() const
{
	AnimationManager::Instance().Cancel(m_Hwnd);
}

MOVE_STATE Measure::DetectState() const
{
	RECT rc;
	GetWindowRect(m_Hwnd, &rc);

	PointInt current{ rc.left, rc.top };

	if (current == (m_Config.End() - m_Config.Anchor()))
		return MOVE_STATE::AT_END;

	return MOVE_STATE::AT_START;
}

//-----------------------------------//
//              OPTIONS              //
//-----------------------------------//

void Measure::ReadPoints() noexcept
{
}

void Measure::ReadConfig() noexcept
{
	m_Config.IgnoreAnchors(ReadBoolean(m_Rm, L"IgnoreAnchors", false));
	m_Config.RelativePosition(ReadBoolean(m_Rm, L"RelativePosition", false));
	m_Config.MouseAction(ReadBoolean(m_Rm, L"MouseAction", true));
	m_Config.Duration(ReadNonNegative(m_Rm, L"Duration", 250));
}

void Measure::ReadEasing() noexcept
{
	const LPCWSTR raw = RmReadString(m_Rm, L"Easing", L"DEFAULT");
	LPCWSTR easing = raw;

	if (_wcsnicmp(easing, L"EASE", 4) == 0)
	{
		easing += 4;
	}

	if (!ParseEnum(easing, EasingTable, m_Easing))
	{
		RmLogF(m_Rm, LOG_ERROR, L"Easing=%s is not valid", raw);
	}
}

void Measure::ReadMotion() noexcept
{
	const LPCWSTR motion = RmReadString(m_Rm, L"Motion", L"DEFAULT");

	if (!ParseEnum(motion, MotionTable, m_Motion))
	{
		RmLogF(m_Rm, LOG_ERROR, L"Motion=%s is not valid", motion);
		return;
	}

	switch (m_Motion)
	{
		case MOTION::ARC:
			m_MotionParams.radiusX = (float)RmReadFormula(m_Rm, L"RadiusX", 0.0f);
			m_MotionParams.radiusY = (float)RmReadFormula(m_Rm, L"RadiusY", 0.0f);
			break;
		case MOTION::SPRING:
			m_MotionParams.stiffness = ReadMinFormula(m_Rm, L"Stiffness", 180.0f, 1.0f);
			m_MotionParams.damping = ReadMinFormula(m_Rm, L"Damping", 24.0f, 1.0f);
			m_MotionParams.mass = ReadMinFormula(m_Rm, L"Mass", 1.0f, 1.0f);
			break;
		default:
			break;
	}
}

void Measure::ReadDirection() noexcept
{
	if (m_Config.RelativePosition())
	{
		m_Direction = DIRECTION::DEFAULT;
		return;
	}

	const LPCWSTR direction = RmReadString(m_Rm, L"StartFrom", L"DEFAULT");

	if (!ParseEnum(direction, DirectionTable, m_Direction))
	{
		RmLogF(m_Rm, LOG_ERROR, L"Direction=%s is not valid", direction);
	}
}

void Measure::ReadActions() noexcept
{
	m_Actions[(uint8_t)COMMAND::START - (uint8_t)COMMAND::START][(uint8_t)STATE::BEGIN] = RmReplaceVariables(m_Rm, RmReadString(m_Rm, L"OnStartBeginAction", L""));
	m_Actions[(uint8_t)COMMAND::START - (uint8_t)COMMAND::START][(uint8_t)STATE::COMPLETE] = RmReplaceVariables(m_Rm, RmReadString(m_Rm, L"OnStartCompleteAction", L""));
	m_Actions[(uint8_t)COMMAND::REVERSE - (uint8_t)COMMAND::START][(uint8_t)STATE::BEGIN] = RmReplaceVariables(m_Rm, RmReadString(m_Rm, L"OnReverseBeginAction", L""));
	m_Actions[(uint8_t)COMMAND::REVERSE - (uint8_t)COMMAND::START][(uint8_t)STATE::COMPLETE] = RmReplaceVariables(m_Rm, RmReadString(m_Rm, L"OnReverseCompleteAction", L""));
}

//-----------------------------------//
	
void Measure::GetPosition()
{
	RECT rc{};
	GetWindowRect(m_Hwnd, &rc);

	const int width = rc.right - rc.left;
	const int height = rc.bottom - rc.top;

	std::wstring value;
	PointInt point{};

	// ------------------------------------------------ //

	if (!m_Config.IgnoreAnchors())
	{
		std::wstring anchorX, anchorY;
		const std::wstring section = RmReplaceVariables(m_Rm, L"#CURRENTCONFIG#");

		GetAnchors(Shared::filePath.c_str(), section.c_str(), anchorX, anchorY);

		point.x = ParseAnchorPosition(anchorX, width);
		point.y = ParseAnchorPosition(anchorY, height);
	}

	m_Config.Anchor(point);

	// ------------------------------------------------ //

	// ------------------------------------------------ //

	PointInt start{ rc.left + m_Config.Anchor().x, rc.top + m_Config.Anchor().y };
	
	if (m_Direction == DIRECTION::DEFAULT)
	{
		value = RmReadString(m_Rm, L"StartX", L"");
		bool hasStartX = !value.empty();
		value = RmReadString(m_Rm, L"StartY", L"");
		bool hasStartY = !value.empty();

		bool hasStart = hasStartX || hasStartY;
		m_Config.HasUserStart(hasStart);

		if (hasStart)
		{
			point.x = RmReadInt(m_Rm, L"StartX", start.x);
			point.y = RmReadInt(m_Rm, L"StartY", start.y);
			m_Config.Start(point);
		}
		else if (!m_StartCaptured)
		{
			m_StartCaptured = true;
			m_Config.Start(start);
		}
	}

	const bool relative = m_Config.RelativePosition();
	
	if (relative)
	{
		m_Config.HasUserStart(true);

		point.x = RmReadInt(m_Rm, L"EndX", 0) + start.x;
		point.y = RmReadInt(m_Rm, L"EndY", 0) + start.y;
		m_Config.End(point);

		m_Config.Start(start - (point - start));
	}
	else
	{
		point.y = RmReadInt(m_Rm, L"EndY", start.y);
		point.x = RmReadInt(m_Rm, L"EndX", start.x);
		m_Config.End(point);
	}

	if (m_Direction != DIRECTION::DEFAULT)
	{
		point = GetStartPosition(m_Hwnd, m_Config, m_Direction);
		m_Config.HasUserStart(true);
		m_Config.Start(point);
	}
}
// 
// -----------------------------
// Easing | Default value: Linear
// -----------------------------
// values: all easings from https://easings.net/
// Note:
// - Plugin will remove "ease" word if it's in the value
// -----------------------------
// 
// -----------------------------
// Motion | Default value: None
// -----------------------------
// values: None | Arc | Spring
// Note:
// - Arc motion has 2 extra options: 
// 	- RadiusX (number) | Default value: 0
// 	- RadiusY (number) | Default value: 0
// - Spring motion has 3 extra options: 
//     - Stiffness (number) | Default value: 180
// 	- Damping (number)   | Default value: 24
// 	- Mass (number)		 | Default value: 1
// -----------------------------
// 

void Measure::ReadOptions() noexcept
{
	ReadConfig();
	ReadEasing();
	ReadMotion();
	ReadDirection();
	ReadActions();
}

void Measure::ReadCommand(const wchar_t* args) noexcept
{
	if (!ParseEnum(args, CommandTable, m_Command))
	{
		RmLogF(m_Rm, LOG_ERROR, L"Unknown command: %s", args);
		return;
	}

	GetPosition();

	if (!m_StateInit)
	{
		m_StateInit = true;
		m_State = DetectState();
		m_ToEnd = !(m_State == MOVE_STATE::AT_START);
	}

	switch (m_Command)
	{
		case COMMAND::STOP:
			Stop();
			break;

		case COMMAND::START:
			Stop();
			m_State = MOVE_STATE::AT_END;
			m_ToEnd = true;
			Start(true);
			break;

		case COMMAND::REVERSE:
			Stop();
			m_State = MOVE_STATE::AT_START;
			m_ToEnd = false;
			Start(false);
			break;

		case COMMAND::FINISH:
			Stop();
			Move(m_Skin, m_Config.End(), m_Config.Anchor());
			m_State = MOVE_STATE::AT_END;
			m_ToEnd = true;
			break;

		case COMMAND::RESET:
			Stop();
			Move(m_Skin, m_Config.Start(), m_Config.Anchor());
			m_State = MOVE_STATE::AT_START;
			m_ToEnd = false;
			break;

		case COMMAND::TOGGLE:
			if (!m_Config.HasUserStart()) break;

			Stop();
			m_ToEnd = !m_ToEnd;
			m_State = m_ToEnd ? MOVE_STATE::AT_END : MOVE_STATE::AT_START;
			m_Command = m_ToEnd ? COMMAND::START : COMMAND::REVERSE;
			Start(m_ToEnd);
			break;

		case COMMAND::UPDATE:
			ReadOptions();
			break;

		case COMMAND::LOOP:

			break;
		default:
			break;
	}
}