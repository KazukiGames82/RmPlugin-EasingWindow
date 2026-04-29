#pragma once

#include <cstdint>
#include "../Util/Entry.h"

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class DIRECTION : uint8_t
{
    DEFAULT,

    LEFT,
    TOP,
    RIGHT,
    BOTTOM,

    LEFTTOP,
    RIGHTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM
};

static constexpr Entry<DIRECTION> DirectionTable[] =
{
    { L"DEFAULT",      DIRECTION::DEFAULT     },
    { L"LEFT",         DIRECTION::LEFT        },
    { L"TOP",          DIRECTION::TOP         },
    { L"RIGHT",        DIRECTION::RIGHT       },
    { L"BOTTOM",       DIRECTION::BOTTOM      },
    { L"LEFTTOP",      DIRECTION::LEFTTOP     },
    { L"RIGHTTOP",     DIRECTION::RIGHTTOP    },
    { L"LEFTBOTTOM",   DIRECTION::LEFTBOTTOM  },
    { L"RIGHTBOTTOM",  DIRECTION::RIGHTBOTTOM }
};