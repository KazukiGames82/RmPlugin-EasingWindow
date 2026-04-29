#pragma once

#include <cstdint>
#include "../Util/Entry.h"

constexpr double PI = 3.14159265358979323846;

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class EASING : uint8_t
{
    DEFAULT,

    INSINE,
    OUTSINE,
    INOUTSINE,

    INQUAD,
    OUTQUAD,
    INOUTQUAD,

    INCUBIC,
    OUTCUBIC,
    INOUTCUBIC,

    INQUART,
    OUTQUART,
    INOUTQUART,

    INQUINT,
    OUTQUINT,
    INOUTQUINT,

    INEXPO,
    OUTEXPO,
    INOUTEXPO,

    INCIRC,
    OUTCIRC,
    INOUTCIRC,

    INBACK,
    OUTBACK,
    INOUTBACK,

    INBOUNCE,
    OUTBOUNCE,
    INOUTBOUNCE,
    
    INELASTIC,
    OUTELASTIC,
    INOUTELASTIC
};

static constexpr Entry<EASING> EasingTable[] =
{
    { L"DEFAULT",       EASING::DEFAULT      },
    { L"INSINE",        EASING::INSINE       },
    { L"OUTSINE",       EASING::OUTSINE      },
    { L"INOUTSINE",     EASING::INOUTSINE    },
    { L"INQUAD",        EASING::INQUAD       },
    { L"OUTQUAD",       EASING::OUTQUAD      },
    { L"INOUTQUAD",     EASING::INOUTQUAD    },
    { L"INCUBIC",       EASING::INCUBIC      },
    { L"OUTCUBIC",      EASING::OUTCUBIC     },
    { L"INOUTCUBIC",    EASING::INOUTCUBIC   },
    { L"INQUART",       EASING::INQUART      },
    { L"OUTQUART",      EASING::OUTQUART     },
    { L"INOUTQUART",    EASING::INOUTQUART   },
    { L"INQUINT",       EASING::INQUINT      },
    { L"OUTQUINT",      EASING::OUTQUINT     },
    { L"INOUTQUINT",    EASING::INOUTQUINT   },
    { L"INEXPO",        EASING::INEXPO       },
    { L"OUTEXPO",       EASING::OUTEXPO      },
    { L"INOUTEXPO",     EASING::INOUTEXPO    },
    { L"INCIRC",        EASING::INCIRC       },
    { L"OUTCIRC",       EASING::OUTCIRC      },
    { L"INOUTCIRC",     EASING::INOUTCIRC    },
    { L"INBACK",        EASING::INBACK       },
    { L"OUTBACK",       EASING::OUTBACK      },
    { L"INOUTBACK",     EASING::INOUTBACK    },
    { L"INBOUNCE",      EASING::INBOUNCE     },
    { L"OUTBOUNCE",     EASING::OUTBOUNCE    },
    { L"INOUTBOUNCE",   EASING::INOUTBOUNCE  },
    { L"INELASTIC",     EASING::INELASTIC    },
    { L"OUTELASTIC",    EASING::OUTELASTIC   },
    { L"INOUTELASTIC",  EASING::INOUTELASTIC }
};

//-------------------------------//
//              API              //
//-------------------------------//

using EaseFn = double(*)(double);
EaseFn GetEaseFn(EASING easing) noexcept;