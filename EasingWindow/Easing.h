#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>

constexpr double PI = 3.14159265358979323846;

//----------------------------------------//
//              ENUMERATIONS              //
//----------------------------------------//

enum class EASING : uint8_t
{
    UNKNOWN = 0,
    LINEAR,

    INSINE, OUTSINE, INOUTSINE,
    INQUAD, OUTQUAD, INOUTQUAD,
    INCUBIC, OUTCUBIC, INOUTCUBIC,
    INQUART, OUTQUART, INOUTQUART,
    INQUINT, OUTQUINT, INOUTQUINT,

    INEXPO, OUTEXPO, INOUTEXPO,
    INCIRC, OUTCIRC, INOUTCIRC,
    INBACK, OUTBACK, INOUTBACK,
    INBOUNCE, OUTBOUNCE, INOUTBOUNCE,
    INELASTIC, OUTELASTIC, INOUTELASTIC
};

//-----------------------------------//
//              STRUCTS              //
//-----------------------------------//

class Easing
{
private:
    EASING easing = EASING::LINEAR;

public:
    inline EASING Get() const noexcept { return easing; }
    inline void Set(EASING value) noexcept { easing = value; }
    inline bool Is(EASING value) const noexcept { return easing == value; }
    inline void Reset() noexcept { easing = EASING::LINEAR; }
};

//-------------------------------//
//              API              //
//-------------------------------//

using EaseFn = double(*)(double);
EaseFn GetEaseFn(EASING easing) noexcept;
EASING ParseEasing(LPCWSTR name) noexcept;