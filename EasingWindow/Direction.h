#pragma once

#include <windows.h>
#include <cstdint>

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class Direction : uint8_t
{
    UNKNOWN = 0,
    SKIN,
    LEFTTOP,
    TOP,
    RIGHTTOP,
    LEFT,
    RIGHT,
    LEFTBOTTOM,
    BOTTOM,
    RIGHTBOTTOM
};

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

class DirectionHandler
{
private:
    Direction direction = Direction::SKIN;

public:
    inline bool IsSkin() const noexcept { return direction == Direction::SKIN; }
    inline Direction Get() const noexcept { return direction; }
    inline void Set(Direction value) noexcept { direction = value; }
    inline bool Is(Direction value) const noexcept { return direction == value; }
    inline void Reset() noexcept { direction = Direction::SKIN; }
};

//-------------------------------//
//              API              //
//-------------------------------//

//Direction ParseDirection(const std::wstring& name) noexcept;
Direction ParseDirection(LPCWSTR name) noexcept;