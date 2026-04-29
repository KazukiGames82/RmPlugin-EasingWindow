#include "Motion.h"
#include <cmath>

constexpr double PI = 3.14159265358979323846;

//-----------------------------------//
//              MOTIONS              //
//-----------------------------------//

static void MotionNone(double, double, double, PointDouble start, PointDouble& coords, PointDouble&, MotionParams&) noexcept
{
    coords = start;
}

static void MotionArc(double fn, double t, double, PointDouble start, PointDouble& coords, PointDouble&, MotionParams& p) noexcept
{
    const double x = std::sin(PI * fn);
    const PointDouble arc { x * p.radiusX, x * p.radiusY };
    coords = start + arc;
}

static void MotionSpring(double, double, double dt, PointDouble start, PointDouble& coords, PointDouble& s, MotionParams& p) noexcept
{
    const PointDouble strength = -p.stiffness * (coords - start) - p.damping * s;
    s += (strength / p.mass) * dt;
    coords += s * dt;
}

//---------------------------------//
//              TABLE              //
//---------------------------------//

static constexpr MotionFn MotionFunctions[] =
{
    MotionNone,
    MotionArc,
    MotionSpring
};

//-------------------------------//
//              API              //
//-------------------------------//

MotionFn GetMotionFn(const MOTION motion) noexcept
{
    return MotionFunctions[(uint8_t)motion];
}
