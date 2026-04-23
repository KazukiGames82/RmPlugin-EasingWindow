#pragma once

#include <cstdint>
#include "Point.h"

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

class Config
{
private:
	bool hasStart : 1 = false;
	bool backwardMove : 1 = false;
	bool freeMove : 1 = false;
	bool ignoreAnchors : 1 = false;
	bool dynamicConfig : 1 = false;
	bool relativePosition : 1 = false;
	bool mouseAction : 1 = true;
	bool onlyBangs : 1 = false;

	uint16_t bangInterval = 0;
	uint32_t duration = 0;

	PointInt anchor{};
	PointInt start{};
	PointInt end{};
	PointInt backward{};

public:
	inline bool HasUserStart() const noexcept { return hasStart; }
	inline void HasUserStart(bool v) noexcept { hasStart = v; }

	inline bool BackwardsMove() const noexcept { return backwardMove; }
	inline void BackwardsMove(bool value) noexcept { backwardMove = value; }

	inline bool FreeMove() const noexcept{ return freeMove; }
	inline void FreeMove(bool value) noexcept{ freeMove = value; }

	inline bool IgnoreAnchors() const noexcept { return ignoreAnchors; }
	inline void IgnoreAnchors(bool value) noexcept{ ignoreAnchors = value; }

	inline bool DynamicConfig() const noexcept { return dynamicConfig; }
	inline void DynamicConfig(bool value) noexcept{ dynamicConfig = value; }

	inline bool RelativePosition() const noexcept { return relativePosition; }
	inline void RelativePosition(bool value) noexcept{ relativePosition = value; }
	
	inline bool MouseAction() const noexcept { return mouseAction; }
	inline void MouseAction(bool value) noexcept{ mouseAction = value; }
	
	inline bool OnlyBangs() const noexcept { return onlyBangs; }
	inline void OnlyBangs(bool value) noexcept{ onlyBangs = value; }

public:
	inline uint16_t BangInterval() const noexcept { return bangInterval; }
	inline void BangInterval(uint16_t value) noexcept{ bangInterval = value; }
	
	inline uint32_t Duration() const noexcept { return duration; }
	inline void Duration(uint32_t value) noexcept{ duration = value; }

public:
	inline PointInt Anchor() const noexcept { return anchor; }
	inline void Anchor(PointInt point) noexcept{ anchor = point; }

	inline PointInt Start() const noexcept { return start; }
	inline void Start(PointInt point) noexcept{ start = point; }

	inline PointInt End() const noexcept { return end; }
	inline void End(PointInt point) noexcept{ end = point; }

	inline PointInt Backwards() const noexcept { return backward; }
	inline void Backwards(PointInt point) noexcept{ backward = point; }
};