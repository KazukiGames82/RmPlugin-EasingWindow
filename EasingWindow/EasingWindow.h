#pragma once

namespace Shared
{
	extern bool initialized;
	extern std::atomic<int> measureCount;
	extern std::wstring filePath;
	extern std::wstring skinPath;
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
	Easing m_Easing{};
	Motion m_Motion{};
	DirectionHandler m_Direction{};
	CommandHandler m_Command{};
	bool m_StateInit = false;
	bool m_StartCaptured = false;
	bool m_ToEnd = true;
	MOVE_STATE m_State = MOVE_STATE::AT_START;

private:
	void Start(bool toEnd);
	void Stop() const;

	MOVE_STATE DetectState() const;

public:
	bool updateValues = true;

	Measure(void* rm);
    ~Measure();
	void GetConfig();
	void GetPosition();
	void GetCommand(LPCWSTR args);

public:
	inline void InvalidValue(LPCWSTR option, LPCWSTR value, bool showError) { if (showError) RmLogF(m_Rm, LOG_ERROR, L"Invalid %s: %s", option, value); }
	inline LPCWSTR ReadString(LPCWSTR option, LPCWSTR defValue) { return RmReadString(m_Rm, option, defValue); }
	inline bool ReadBoolean(LPCWSTR option, bool defValue) { return ClampValue(RmReadInt(m_Rm, option, defValue ? 1 : 0), 0, 1) == 1; }
	template<typename T>
	T ReadNumber(LPCWSTR option, T defValue);
	template<typename T>
	T MinError(LPCWSTR option, T defValue, T minValue);
};

template<typename T>
inline T Measure::ReadNumber(LPCWSTR option, T defValue)
{
	return static_cast<T>(RmReadFormula(m_Rm, option, defValue));
}

template<typename T>
inline T Measure::MinError(LPCWSTR option, T defValue, T minValue)
{
	std::wstring upper = option;
	ConvertToUpperCase(upper);
	T value = ReadNumber(option, defValue);

	if (value < minValue)
	{
		RmLogF(m_Rm, LOG_ERROR, L"%s shouldn't be less than %g", upper.c_str(), static_cast<double>(minValue));
		return minValue;
	}

	return value;
}
