#pragma once

#include <cstdint>
#include "../Util/Point.h"
#include "../Util/Entry.h"

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class MOTION : uint8_t
{
    DEFAULT,
    ARC,
    SPRING
};

static constexpr Entry<MOTION> MotionTable[] =
{
    { L"DEFAULT",   MOTION::DEFAULT },
    { L"ARC",       MOTION::ARC     },
    { L"SPRING",    MOTION::SPRING  }
};

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

struct MotionParams
{
    float radiusX = 0.0f;
    float radiusY = 0.0f;
    float stiffness = 180.0f;
    float damping = 24.0f;
    float mass = 1.0f;
};

//-------------------------------//
//              API              //
//-------------------------------//

using MotionFn = void(*)(double fn, double t, double dt, PointDouble start, PointDouble& coords, PointDouble& state, MotionParams& params);
MotionFn GetMotionFn(const MOTION motion) noexcept;