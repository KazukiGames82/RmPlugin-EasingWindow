#include <windows.h>
#include "../API/RainmeterAPI.h"

#include <cmath>
#include <string>
#include <cwctype>
#include <chrono>
#include "Math.h"
#include "StringUtil.h"
#include "FileWin.h"
#include "WindowWin.h"

#include "Config.h"
#include "Easing.h"
#include "Motion.h"
#include "Direction.h"
#include "CommandHandler.h"
#include "Animator.h"
#include "EasingWindow.h"

namespace Shared
{
	bool initialized = false;
	std::atomic<int> measureCount = 0;
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
	//Shared::measureCount++;
	Measure* measure = new Measure(rm);
	*data = measure;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;
	measure->GetConfig();
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;
	return 0.0;
}

PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
{
	Measure* measure = (Measure*)data;
	measure->GetCommand(args);
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	//if (--Shared::measureCount <= 0)
	//{
	//	AnimationManager::Instance().Shutdown();
	//}

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
	GetConfig();
}

Measure::~Measure()
{
	Stop();
}

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
	anim.updates = 0;
	anim.coords = ConvertToDouble(start);
	anim.velocity = { 0.0, 0.0 };
	anim.m_Rm = this->m_Rm;
	anim.m_Skin = this->m_Skin;
	anim.m_Hwnd = this->m_Hwnd;
	anim.m_Motion = this->m_Motion;
	
	anim.isDynamic = this->m_Motion.isDynamic();
	anim.isMouseActions = this->m_Config.MouseAction();
	anim.isOnlyBangs = this->m_Config.OnlyBangs();
	anim.start = start;
	anim.userTarget = userTarget;
	anim.animTarget = animTarget;
	anim.duration = this->m_Config.Duration() / 1000.0;
	anim.bangInterval = m_Config.BangInterval();
	anim.coords = { double(start.x), double(start.y) };
	anim.ease = GetEaseFn(this->m_Easing.Get());
	anim.motion = GetMotionFn(this->m_Motion.Get());
	anim.action[0] = m_Command.GetAction(m_Command.Get(), State::Begin);
	anim.action[1] = m_Command.GetAction(m_Command.Get(), State::Complete);

	AnimationManager::Instance().Add(std::move(anim));
}

void Measure::Stop() const
{
	AnimationManager::Instance().Cancel(m_Hwnd);
	//AnimationManager::Instance().PurgeInvalid();
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
	
	if (m_Direction.IsSkin())
	{
		value = ReadString(L"StartX", L"");
		bool hasStartX = !value.empty();
		value = ReadString(L"StartY", L"");
		bool hasStartY = !value.empty();

		bool hasStart = hasStartX || hasStartY;
		m_Config.HasUserStart(hasStart);

		if (hasStart)
		{
			point.x = ReadNumber(L"StartX", start.x);
			point.y = ReadNumber(L"StartY", start.y);
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

		point.x = ReadNumber(L"EndX", ReadNumber(L"TargetX", 0)) + start.x;
		point.y = ReadNumber(L"EndY", ReadNumber(L"TargetY", 0)) + start.y;
		m_Config.End(point);

		m_Config.Start(start - (point - start));
	}
	else
	{
		point.y = ReadNumber(L"EndY", ReadNumber(L"TargetY", start.y));
		point.x = ReadNumber(L"EndX", ReadNumber(L"TargetX", start.x));
		m_Config.End(point);
	}

	if (!m_Direction.IsSkin())
	{
		point = GetStartPosition(m_Hwnd, m_Config, m_Direction.Get());
		m_Config.HasUserStart(true);
		m_Config.Start(point);
	}
}

void Measure::GetConfig()
{
	std::wstring value;

	//------------------------------------//
	//              M_CONFIG              //
	//------------------------------------//

	m_Config.IgnoreAnchors(ReadBoolean(L"IgnoreAnchors", false));
	m_Config.DynamicConfig(ReadBoolean(L"DynamicConfig", false));
	m_Config.RelativePosition(ReadBoolean(L"RelativePosition", false));
	m_Config.MouseAction(ReadBoolean(L"MouseAction", true));
	m_Config.OnlyBangs(ReadBoolean(L"OnlyBangs", false));
	m_Config.BangInterval(MinError(L"BangInterval", 255, 0));
	m_Config.Duration(MinError(L"Duration", MinError(L"Ms", 250, 0), 0));

	//------------------------------------//
	//              M_EASING              //
	//------------------------------------//

	value = ReadString(L"Easing", L"Linear");
	ConvertToLowerCase(value);
	RemoveSubstring(value, L"ease");
	m_Easing.Set(ParseEasing(value.c_str()));
	ConvertToUpperCase(value);
	InvalidValue(L"Easing", value.c_str(), m_Easing.Is(EASING::UNKNOWN));

	//------------------------------------//
	//              M_MOTION              //
	//------------------------------------//

	value = ReadString(L"Motion", L"None");
	m_Motion.Set(ParseMotion(value.c_str()));
	InvalidValue(L"Motion", value.c_str(), m_Motion.Is(MOTION::UNKNOWN));

	switch (m_Motion.Get())
	{
		case MOTION::ARC:
			m_Motion.RadiusX(ReadNumber(L"RadiusX", 0.0f));
			m_Motion.RadiusY(ReadNumber(L"RadiusY", 0.0f));
			break;
		case MOTION::SPRING:
			m_Motion.Stiffness(MinError(L"Stiffness", 180.0f, 1.0f));
			m_Motion.Damping(MinError(L"Damping", 24.0f, 1.0f));
			m_Motion.Mass(MinError(L"Mass", 1.0f, 1.0f));
			break;
		default:
			break;
	}

	//------------------------------------//
	//             M_Direction            //
	//------------------------------------//

	if (m_Config.RelativePosition())
	{
		m_Direction.Reset();
	}
	else
	{
		value = ReadString(L"StartFrom", L"Skin");
		m_Direction.Set(ParseDirection(value.c_str()));
		InvalidValue(L"Direction", value.c_str(), m_Direction.Is(Direction::UNKNOWN));
	}

	//----------------------------------//
	//             M_COMMAND            //
	//----------------------------------//

	m_Command.SetAction(Command::Start, State::Begin, RmReplaceVariables(m_Rm, ReadString(L"OnStartBeginAction", L"")));
	m_Command.SetAction(Command::Start, State::Complete, RmReplaceVariables(m_Rm, ReadString(L"OnStartCompleteAction", L"")));
	m_Command.SetAction(Command::Reverse, State::Begin, RmReplaceVariables(m_Rm, ReadString(L"OnReverseBeginAction", L"")));
	m_Command.SetAction(Command::Reverse, State::Complete, RmReplaceVariables(m_Rm, ReadString(L"OnReverseCompleteAction", L"")));
}

void Measure::GetCommand(LPCWSTR args)
{
	m_Command.Set(ParseCommand(args));
	InvalidValue(L"Command", args, m_Command.Is(Command::Unknown));

	const Command cmd = m_Command.Get();
	
	//if (m_Config.DynamicConfig())
	//{
	//	GetConfig();
	//}
	GetPosition();

	if (!m_StateInit)
	{
		m_StateInit = true;
		m_State = DetectState();
		m_ToEnd = !(m_State == MOVE_STATE::AT_START);
	}

	switch (cmd)
	{
		case Command::Stop:
			Stop();
			break;

		case Command::Start:
			//if (m_State == MOVE_STATE::AT_END) break;
			Stop();
			m_State = MOVE_STATE::AT_END;
			m_ToEnd = true;
			Start(true);
			break;

		case Command::Reverse:
			//if (m_State == MOVE_STATE::AT_START) break;
			Stop();
			m_State = MOVE_STATE::AT_START;
			m_ToEnd = false;
			Start(false);
			break;

		case Command::Finish:
			Stop();
			Move(m_Skin, m_Config.End(), m_Config.Anchor());
			m_State = MOVE_STATE::AT_END;
			m_ToEnd = true;
			break;

		case Command::Reset:
			Stop();
			Move(m_Skin, m_Config.Start(), m_Config.Anchor());
			m_State = MOVE_STATE::AT_START;
			m_ToEnd = false;
			break;

		case Command::Toggle:
			if (!m_Config.HasUserStart()) break;

			Stop();
			m_ToEnd = !m_ToEnd;
			m_State = m_ToEnd ? MOVE_STATE::AT_END : MOVE_STATE::AT_START;
			m_Command.Set(m_ToEnd ? Command::Start : Command::Reverse);
			Start(m_ToEnd);
			break;

		case Command::Update:
			GetConfig();
			break;

		default:
			break;
	}
}
