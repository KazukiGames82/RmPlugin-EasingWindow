#pragma once

#include <Windows.h>
#include <cstdint>
#include "Point.h"

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class MOTION : uint8_t
{
    UNKNOWN = 0,
    NONE,
    ARC,
    SPRING
};

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

class Motion
{
private:
    MOTION motion = MOTION::NONE;

    float radiusX = 0.0f;
    float radiusY = 0.0f;
    float stiffness = 0.0f;
    float damping = 0.0f;
    float mass = 1.0f;

public:
    inline bool isDynamic() const noexcept { return (motion >= MOTION::SPRING); };
    inline MOTION Get() const noexcept { return motion; }
    inline void Set(MOTION value) noexcept { motion = value; }
    inline bool Is(MOTION value) const noexcept { return motion == value; }
    inline void Reset() noexcept { motion = MOTION::NONE; }

public:
    inline float RadiusX() const noexcept { return radiusX; }
    inline void RadiusX(float v) noexcept { radiusX = v; }

    inline float RadiusY() const noexcept { return radiusY; }
    inline void RadiusY(float v) noexcept { radiusY = v; }

    inline float Stiffness() const noexcept { return stiffness; }
    inline void Stiffness(float v) noexcept { stiffness = v; }

    inline float Damping() const noexcept { return damping; }
    inline void Damping(float v) noexcept { damping = v; }

    inline float Mass() const noexcept { return mass; }
    inline void Mass(float v) noexcept { mass = v; }
};

//-------------------------------//
//              API              //
//-------------------------------//

using MotionFn = void(*)(double fn, double t, double dt, PointDouble start, PointDouble& coords, PointDouble& state, Motion& params);
MotionFn GetMotionFn(const MOTION motion) noexcept;
//MOTION ParseMotion(const std::wstring& name) noexcept;
MOTION ParseMotion(LPCWSTR name) noexcept;
