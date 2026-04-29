#pragma once

#include <windows.h>

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "Point.h"
#include "Easing.h"
#include "Motion.h"

struct Animation
{
	void* m_Rm = nullptr;
	void* m_Skin = nullptr;
	HWND m_Hwnd = nullptr;

	bool started : 1 = false;
	bool finished : 1 = false;
	bool aborted : 1 = false;
	bool completed : 1 = false;
	bool isDynamic : 1 = false;
	bool isMouseActions : 1 = true;
	//bool isOnlyBangs : 1 = false;

	MOTION m_Motion{};
	MotionParams m_MotionParams{};
	MotionFn motion = nullptr;
	EaseFn ease = nullptr;

	double duration = 0.0;
	double elapsed = 0.0;
	//int updates = 0;
	//uint16_t bangInterval = 255;

	PointInt lastAppliedPos{};
	PointInt start{};
	PointInt userTarget{};
	PointInt animTarget{};

	PointDouble coords{};
	PointDouble velocity{};
	
	std::wstring action[2] = { L"" };

	void ResetRuntime();
};

class AnimationManager
{
private: 
	std::vector<Animation> animations;
	std::mutex mtx;
	std::condition_variable_any cv;
	std::jthread worker;

private:

	void Loop(std::stop_token st);

public:
	AnimationManager();
	~AnimationManager();
	static AnimationManager& Instance();

	void Shutdown();
	void Add(Animation anim);
	void Cancel(HWND hwnd);
};

void Move(void* skin, PointInt point, PointInt anchor = { 0,0 }) noexcept;
void Execute(void* skin, LPCWSTR action) noexcept;