#pragma once

#include <cstdint>
#include "ParseEnum.h"

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class COMMAND : uint8_t
{
    DEFAULT,
    STOP,
    START,
    REVERSE,
    FINISH,
    RESET,
    TOGGLE,
    LOOP,
    UPDATE,

    PAUSE = 9,
    RESUME = 10,
    RESTART = 11,
    SKIP = 12,
    NEXT = 13,
    PREVIOUS = 14,

    ENABLE = 15,
    DISABLE = 16,

    OPEN = 17,
    CLOSE = 18,

    LOCK = 19,
    UNLOCK = 20,

    ACTIVATE = 21,
    DEACTIVATE = 22,

    INCREASE = 23,
    DECREASE = 24,

    MOVE_UP = 25,
    MOVE_DOWN = 26,
    MOVE_LEFT = 27,
    MOVE_RIGHT = 28,

    SAVE = 29,
    LOAD = 30,

    CONNECT = 31,
    DISCONNECT = 32,

    BEGIN = 33,
    END = 34,

    CONFIRM = 35,
    CANCEL = 36,

    UNDO = 37,
    REDO = 38,
};

enum class STATE : uint8_t
{
    BEGIN,
    COMPLETE
};

static constexpr Entry<COMMAND> CommandTable[] =
{
    { L"STOP",    COMMAND::STOP    },
    { L"START",   COMMAND::START   },
    { L"REVERSE", COMMAND::REVERSE },
    { L"FINISH",  COMMAND::FINISH  },
    { L"RESET",   COMMAND::RESET   },
    { L"TOGGLE",  COMMAND::TOGGLE  },
    { L"LOOP",    COMMAND::LOOP    },
    { L"UPDATE",  COMMAND::UPDATE  },
};