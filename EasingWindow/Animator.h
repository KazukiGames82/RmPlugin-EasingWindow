#pragma once

#include <windows.h>

#include <string>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <chrono>

#include "Point.h"
#include "Math.h"
#include "CommandHandler.h"
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
	bool isOnlyBangs : 1 = false;

	Motion m_Motion{};
	MotionFn motion = nullptr;
	EaseFn ease = nullptr;

	double duration = 0.0;
	double elapsed = 0.0;
	int updates = 0;
	uint16_t bangInterval = 0;

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

void Move(void* skin, PointInt point, PointInt anchor = { 0,0 });
void Execute(void* rm, void* skin, LPCWSTR action);