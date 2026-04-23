#include "Direction.h"

//---------------------------------//
//              TABLE              //
//---------------------------------//

struct DirectionEntry
{
    LPCWSTR name;
    Direction value;
};

static constexpr DirectionEntry DirectionTable[] =
{
    {L"default",      Direction::SKIN},
    {L"none",         Direction::SKIN},
    {L"skin",         Direction::SKIN},
    {L"lefttop",      Direction::LEFTTOP},
    {L"top",          Direction::TOP},
    {L"righttop",     Direction::RIGHTTOP},
    {L"left",         Direction::LEFT},
    {L"right",        Direction::RIGHT},
    {L"leftbottom",   Direction::LEFTBOTTOM},
    {L"bottom",       Direction::BOTTOM},
    {L"rightbottom",  Direction::RIGHTBOTTOM}
};

//-------------------------------//
//              API              //
//-------------------------------//

Direction ParseDirection(LPCWSTR name) noexcept
{
    if (!name || !*name)
        return Direction::UNKNOWN;

    for (const auto& entry : DirectionTable)
    {
        if (_wcsicmp(entry.name, name) == 0)
            return entry.value;
    }

    return Direction::UNKNOWN;
}
