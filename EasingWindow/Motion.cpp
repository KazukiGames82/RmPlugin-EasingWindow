#include "Motion.h"
#include <cmath>
//#include <unordered_map>

constexpr double PI = 3.14159265358979323846;

//-----------------------------------//
//              MOTIONS              //
//-----------------------------------//

static void MotionNone(double, double, double, PointDouble start, PointDouble& coords, PointDouble&, Motion&) noexcept
{
    coords = start;
}

static void MotionArc(double fn, double t, double, PointDouble start, PointDouble& coords, PointDouble&, Motion& p) noexcept
{
    const double x = std::sin(PI * fn);
    const PointDouble arc { x * p.RadiusX(), x * p.RadiusY() };
    coords = start + arc;
}

static void MotionSpring(double, double, double dt, PointDouble start, PointDouble& coords, PointDouble& s, Motion& p) noexcept
{
    const double stiffness = p.Stiffness(), damping = p.Damping(), mass = p.Mass();
    const PointDouble strength = -stiffness * (coords - start) - damping * s;
    s += (strength / mass) * dt;
    coords += s * dt;
}

//---------------------------------//
//              TABLE              //
//---------------------------------//

static constexpr MotionFn MotionFunctions[] =
{
    MotionNone,
    MotionNone,
    MotionArc,
    MotionSpring
};

//static const std::unordered_map<std::wstring, MOTION> MotionMap =
//{
//    {L"none",       MOTION::NONE},
//    {L"arc",        MOTION::ARC},
//    {L"spring",     MOTION::SPRING}
//};


struct MotionEntry
{
    LPCWSTR name;
    MOTION value;
};

static constexpr MotionEntry MotionTable[] =
{
    {L"none",       MOTION::NONE},
    {L"arc",        MOTION::ARC},
    {L"spring",     MOTION::SPRING}
};

//-------------------------------//
//              API              //
//-------------------------------//

MotionFn GetMotionFn(const MOTION motion) noexcept
{
    return MotionFunctions[static_cast<uint8_t>(motion)];
}

//MOTION ParseMotion(const std::wstring& name) noexcept
//{
//    auto it = MotionMap.find(name);
//    return (it != MotionMap.end()) ? it->second : MOTION::UNKNOWN;
//}

MOTION ParseMotion(LPCWSTR name) noexcept
{
    if (!name || !*name)
        return MOTION::UNKNOWN;

    for (const auto& entry : MotionTable)
    {
        if (_wcsicmp(entry.name, name) == 0)
            return entry.value;
    }

    return MOTION::UNKNOWN;
}
