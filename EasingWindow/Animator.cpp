#include "Animator.h"
#include "../API/RainmeterAPI.h"

void Move(void* skin, PointInt point, PointInt anchor)
{
	const PointInt pos = point - anchor;

	wchar_t buffer[64];
	swprintf_s(buffer, L"!Move %d %d", pos.x, pos.y);
	RmExecute(skin, buffer);
}

void Execute(void* rm, void* skin, LPCWSTR action)
{
	if (!action || action[0] == L'\0') return;

	RmExecute(skin, action);
}

AnimationManager& AnimationManager::Instance()
{
	static AnimationManager instance;
	return instance;
}

AnimationManager::AnimationManager()
{
	worker = std::jthread(
		[this](std::stop_token st)
		{
			Loop(st);
		}
	);
}

AnimationManager::~AnimationManager()
{
	worker.request_stop();
	cv.notify_all();
}

void AnimationManager::Shutdown()
{
	std::lock_guard<std::mutex> lock(mtx);

	worker.request_stop();
	cv.notify_all();

	if (worker.joinable())
		worker.join();
}
void AnimationManager::Cancel(HWND hwnd)
{
	std::lock_guard<std::mutex> lock(mtx);

	for (auto& anim : animations)
	{
		if (anim.m_Hwnd == hwnd)
			anim.aborted = true;
	}
}

void AnimationManager::Add(Animation anim)
{
	{
		std::lock_guard<std::mutex> lock(mtx);

		for (auto& a : animations)
		{
			if (a.m_Hwnd == anim.m_Hwnd)
				a.aborted = true;
		}

		Execute(anim.m_Rm, anim.m_Skin, anim.action[static_cast<uint8_t>(State::Begin)].c_str());

		anim.ResetRuntime();
		animations.push_back(std::move(anim));
	}

	cv.notify_one();
	//{
	//	std::lock_guard lock(mtx);
	//
	//	std::erase_if(animations, [&](const auto& a) {
	//		return a.m_Hwnd == anim.m_Hwnd;
	//				  });
	//
	//	Execute(anim.m_Rm, anim.m_Skin, anim.action[static_cast<uint8_t>(State::Begin)].c_str());
	//
	//	anim.ResetRuntime();
	//	animations.push_back(std::move(anim));
	//}
	//cv.notify_one();
}

void AnimationManager::Loop(std::stop_token st)
{
	using clock = std::chrono::steady_clock;
	auto last = clock::now();

	std::vector<std::pair<HWND, BOOL>> mouseActions;
	std::vector<std::pair<void*, PointInt>> moves;
	std::vector<std::tuple<void*, void*, std::wstring>> completes;
	std::vector<std::pair<HWND, PointInt>> deferredMoves;

	while (!st.stop_requested())
	{
		auto now = clock::now();
		double dt = std::chrono::duration<double>(now - last).count();
		dt = (std::min)(dt, 0.05);
		last = now;

		{
			std::unique_lock lock(mtx);

			if (animations.empty())
			{
				cv.wait_for(lock, std::chrono::milliseconds(4), [&]
							{
								return st.stop_requested() || !animations.empty();
							});
				last = clock::now();
				continue;
			}

			for (auto& anim : animations)
			{
				if (!anim.started)
				{
					if (!anim.isMouseActions && IsWindow(anim.m_Hwnd))
						mouseActions.push_back({ anim.m_Hwnd, FALSE });

					anim.started = true;
					moves.push_back({ anim.m_Skin, anim.start });
					continue;
				}

				if (anim.aborted)
				{
					anim.finished = true;
					continue;
				}

				anim.elapsed += dt;

				double t = anim.duration > 0 ? ClampValue(anim.elapsed / anim.duration, 0.0, 1.0) : 1.0;
				double e = anim.ease(t);

				PointDouble easePos = ConvertToDouble(anim.start) + (ConvertToDouble(anim.animTarget) - ConvertToDouble(anim.start)) * e;
				anim.motion(e, t, dt, easePos, anim.coords, anim.velocity, anim.m_Motion);
				PointInt currentPos = { int(std::lround(anim.coords.x)), int(std::lround(anim.coords.y)) };

				if (anim.isDynamic)
				{
					if (HasSpringConverged(anim.coords.x - anim.animTarget.x, anim.coords.y - anim.animTarget.y, anim.velocity.x, anim.velocity.y))
					{
						anim.finished = true;
					}
				}
				else if (t >= 1.0)
				{
					anim.finished = true;
				}

				if (!anim.finished && !anim.aborted)
				{
					if (anim.isOnlyBangs)
					{
						if (++anim.updates >= anim.bangInterval && anim.bangInterval > 0)
						{
							anim.updates = 0;
							moves.push_back({ anim.m_Skin, currentPos });
						}
					}
					else 
					{
						if (++anim.updates >= anim.bangInterval && anim.bangInterval > 0)
						{
							anim.updates = 0;
							moves.push_back({ anim.m_Skin, currentPos });
						} 
						else
						{
							if (currentPos != anim.lastAppliedPos)
							{
								anim.lastAppliedPos = currentPos;
								deferredMoves.emplace_back(anim.m_Hwnd, currentPos);
							}
						}				
					}
				}

				if (anim.finished)
				{
					mouseActions.push_back({ anim.m_Hwnd, TRUE});
					moves.push_back({ anim.m_Skin, anim.animTarget });
					completes.push_back({ anim.m_Rm, anim.m_Skin, anim.action[static_cast<uint8_t>(State::Complete)]});
				}
			}
			std::erase_if(animations, [](const Animation& a) { return a.finished || a.aborted; });
		}

		if (!mouseActions.empty())
		{
			for (auto& [hwnd, state] : mouseActions)
			{
				EnableWindow(hwnd, state);
			}
			mouseActions.clear();
		}
		
		if (!deferredMoves.empty())
		{
			if (HDWP hdwp = BeginDeferWindowPos(static_cast<int>(deferredMoves.size())))
			{
				for (auto& [hwnd, pos] : deferredMoves)
				{
					hdwp = DeferWindowPos(hdwp, hwnd, nullptr, pos.x, pos.y, 0, 0,
										  SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOREDRAW);
					if (!hdwp) break;
				}
				EndDeferWindowPos(hdwp);
			}
			deferredMoves.clear();
		}

		if (!moves.empty())
		{
			for (auto& [skin, pos] : moves)
				Move(skin, pos);
			moves.clear();		
		}

		if (!completes.empty())
		{
			for (auto& [rm, skin, command] : completes)
				Execute(rm, skin, command.c_str());
			completes.clear();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}

void Animation::ResetRuntime()
{
	started = false;
	finished = false;
	aborted = false;

	elapsed = 0.0;
	updates = 0;

	coords = ConvertToDouble(start);
	velocity = {};
}