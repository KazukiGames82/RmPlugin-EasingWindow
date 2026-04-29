#pragma once

// 240kb
// 225kb
// 226kb
// 223kb
// 224kb
// 
//
//
//
//
//
//
//
#include <windows.h>
#include <string>
#include "../API/RainmeterAPI.h"

#include "Config.h"
#include "Easing.h"
#include "Motion.h"
#include "Direction.h"
#include "Command.h"

namespace Shared
{
	extern bool initialized;
	extern std::wstring filePath;
}

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

enum class MOVE_STATE : uint8_t
{
	AT_START,
	AT_END
};

class Measure
{
private:
	void* m_Rm = nullptr;
	void* m_Skin = nullptr;
	HWND m_Hwnd = nullptr;

	Config m_Config{};
	MotionParams m_MotionParams{};
	EASING m_Easing = EASING::DEFAULT;
	MOTION m_Motion = MOTION::DEFAULT;	
	DIRECTION m_Direction = DIRECTION::DEFAULT;
	COMMAND m_Command = COMMAND::DEFAULT;
	std::wstring m_Actions[2][2] = { L"" };
	bool m_StateInit = false;
	bool m_StartCaptured = false;
	bool m_ToEnd = true;
	MOVE_STATE m_State = MOVE_STATE::AT_START;

private:
	void ReadPoints() noexcept;
	void ReadConfig() noexcept;
	void ReadEasing() noexcept;
	void ReadMotion() noexcept;
	void ReadDirection() noexcept;
	void ReadActions() noexcept;

	void Start(bool toEnd);
	void Stop() const;

	MOVE_STATE DetectState() const;

public:
	Measure(void* rm);
    ~Measure();

	void ReadOptions() noexcept;
	void ReadCommand(const wchar_t* args) noexcept;

	void GetPosition();

};

inline bool ReadBoolean(void* rm, const wchar_t* option, bool defaultValue) noexcept
{
	const int value = RmReadInt(rm, option, defaultValue ? 1 : 0);

	if (value != 0 && value != 1)
	{
		RmLogF(rm, LOG_ERROR, L"%s should be 0 or 1", option);
		return defaultValue;
	}

	return value != 0;
}

inline float ReadMinFormula(void* rm, const wchar_t* option, float defaultValue, float minValue) noexcept
{
	const float value = (float)RmReadFormula(rm, option, defaultValue);

	if (value < minValue)
	{
		RmLogF(rm, LOG_ERROR, L"Value must not be less than %.2f", minValue);
		return defaultValue;
	}

	return value;
}

inline int ReadNonNegative(void* rm, const wchar_t* option, int defaultValue) noexcept
{
	const int value = (int)RmReadFormula(rm, option, defaultValue);

	if (value < 0.0f)
	{
		RmLogF(rm, LOG_ERROR, L"%s must not be negative", option);
		return defaultValue;
	}

	return value;
}