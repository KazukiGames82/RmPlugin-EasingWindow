#include "Easing.h"
#include <cmath>

//-----------------------------------//
//              EASINGS              //
//-----------------------------------//

static double EasingLinear(double t) noexcept
{
    return t;
}

static double EasingInSine(double t) noexcept
{
    return 1.0 - std::cos((t * PI) / 2.0);
}

static double EasingOutSine(double t) noexcept
{
    return std::sin((t * PI) / 2.0);
}

static double EasingInOutSine(double t) noexcept
{
    return -(std::cos(PI * t) - 1.0) / 2.0;
}

static double EasingInCubic(double t) noexcept
{
    return t * t * t;
}

static double EasingOutCubic(double t) noexcept
{
    const double x = 1.0 - t;
    return 1.0 - x * x * x;
}

static double EasingInOutCubic(double t) noexcept
{
    const double x = -2.0 * t + 2.0;
    return t < 0.5
        ? 4.0 * t * t * t
        : 1.0 - (x * x * x) / 2.0;
}

static double EasingInQuint(double t) noexcept
{
    return t * t * t * t * t;
}

static double EasingOutQuint(double t) noexcept
{
    const double x = 1.0 - t;
    return 1.0 - x * x * x * x * x;
}

static double EasingInOutQuint(double t) noexcept
{
    const double x = -2.0 * t + 2.0;
    return t < 0.5
        ? 16.0 * t * t * t * t * t
        : 1.0 - (x * x * x * x * x) / 2.0;
}

static double EasingInCirc(double t) noexcept
{
    return 1.0 - std::sqrt(1.0 - t * t);
}

static double EasingOutCirc(double t) noexcept
{
    const double x = t - 1.0;
    return std::sqrt(1.0 - x * x);
}

static double EasingInOutCirc(double t) noexcept
{
    double x = 0.0;
    if (t < 0.5)
    {
        x = 2.0 * t;
        return (1.0 - std::sqrt(1.0 - x * x)) / 2.0;
    }
    else
    {
        x = -2.0 * t + 2.0;
        return (std::sqrt(1.0 - x * x) + 1.0) / 2.0;
    }
}

static double EasingInElastic(double t) noexcept
{
    if (t == 0.0 || t == 1.0) return t;
    constexpr double c4 = (2.0 * PI) / 3.0;
    return -std::exp2(10.0 * t - 10.0) * std::sin((t * 10.0 - 10.75) * c4);
}

static double EasingOutElastic(double t) noexcept
{
    if (t == 0.0 || t == 1.0) return t;
    constexpr double c4 = (2.0 * PI) / 3.0;
    return std::exp2(-10.0 * t) * std::sin((t * 10.0 - 0.75) * c4) + 1.0;
}

static double EasingInOutElastic(double t) noexcept
{
    if (t == 0.0 || t == 1.0) return t;
    constexpr double c5 = (2.0 * PI) / 4.5;

    return t < 0.5
        ? -(std::exp2(20.0 * t - 10.0) * std::sin((20.0 * t - 11.125) * c5)) / 2.0
        : (std::exp2(-20.0 * t + 10.0) * std::sin((20.0 * t - 11.125) * c5)) / 2.0 + 1.0;
}

static double EasingInQuad(double t) noexcept
{
    return t * t;
}

static double EasingOutQuad(double t) noexcept
{
    const double x = 1.0 - t;
    return 1.0 - x * x;
}

static double EasingInOutQuad(double t) noexcept
{
    const double x = -2.0 * t + 2.0;
    return t < 0.5
        ? 2.0 * t * t
        : 1.0 - (x * x) / 2.0;
}

static double EasingInQuart(double t) noexcept
{
    return t * t * t * t;
}

static double EasingOutQuart(double t) noexcept
{
    double x = 1.0 - t;
    return 1.0 - x * x * x * x;
}

static double EasingInOutQuart(double t) noexcept
{
    const double x = -2.0 * t + 2.0;
    return t < 0.5
        ? 8.0 * t * t * t * t
        : 1.0 - (x * x * x * x) / 2.0;
}

static double EasingInExpo(double t) noexcept
{
    return t == 0.0
        ? 0.0
        : std::exp2(10.0 * t - 10.0);
}

static double EasingOutExpo(double t) noexcept
{
    return t == 1.0
        ? 1.0 
        : 1.0 - std::exp2(-10.0 * t);
}

static double EasingInOutExpo(double t) noexcept
{
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;

    return t < 0.5
        ? std::exp2(20.0 * t - 10.0) / 2.0
        : (2.0 - std::exp2(-20.0 * t + 10.0)) / 2.0;
}

static double EasingInBack(double t) noexcept
{
    constexpr double c1 = 1.70158;
    constexpr double c3 = c1 + 1.0;
    return c3 * t * t * t - c1 * t * t;
}

static double EasingOutBack(double t) noexcept
{
    constexpr double c1 = 1.70158;
    constexpr double c3 = c1 + 1.0;
    const double x = t - 1.0;
    return 1.0 + c3 * (x * x * x) + c1 * (x * x);
}

static double EasingInOutBack(double t) noexcept
{
    constexpr double c1 = 1.70158;
    constexpr double c2 = c1 * 1.525;
    double x = 0.0;
    if (t < 0.5)
    {
        x = 2.0 * t;
        return (x * x * ((c2 + 1.0) * x - c2)) / 2.0;
    }
    else
    {
        x = 2.0 * t - 2.0;
        return (x * x * ((c2 + 1.0) * x + c2) + 2.0) / 2.0;
    }
}

static double EasingOutBounce(double t) noexcept
{
    constexpr double n1 = 7.5625;
    constexpr double d1 = 2.75;

    if (t < 1.0 / d1)
        return n1 * t * t;

    else if (t < 2.0 / d1)
        return n1 * (t -= 1.5 / d1) * t + 0.75;

    else if (t < 2.5 / d1)
        return n1 * (t -= 2.25 / d1) * t + 0.9375;

    else
        return n1 * (t -= 2.625 / d1) * t + 0.984375;
}

static double EasingInBounce(double t) noexcept
{
    return 1.0 - EasingOutBounce(1.0 - t);
}

static double EasingInOutBounce(double t) noexcept
{
    return t < 0.5
        ? (1.0 - EasingOutBounce(1.0 - 2.0 * t)) / 2.0
        : (1.0 + EasingOutBounce(2.0 * t - 1.0)) / 2.0;
}

//---------------------------------//
//              TABLE              //
//---------------------------------//

static constexpr EaseFn EasingFunctions[] =
{
    EasingLinear, EasingLinear,
    
    EasingInSine, EasingOutSine, EasingInOutSine,
    EasingInQuad, EasingOutQuad, EasingInOutQuad,
    EasingInCubic, EasingOutCubic, EasingInOutCubic,
    EasingInQuart, EasingOutQuart, EasingInOutQuart,
    EasingInQuint, EasingOutQuint, EasingInOutQuint,

    EasingInExpo, EasingOutExpo, EasingInOutExpo,
    EasingInCirc, EasingOutCirc, EasingInOutCirc,
    EasingInBack, EasingOutBack, EasingInOutBack,
    EasingInBounce, EasingOutBounce, EasingInOutBounce,
    EasingInElastic, EasingOutElastic, EasingInOutElastic
};

struct EasingEntry
{
    LPCWSTR name;
    EASING value;
};

static constexpr EasingEntry EasingTable[] =
{
    {L"linear",     EASING::LINEAR},
    {L"insine",     EASING::INSINE},
    {L"outsine",    EASING::OUTSINE},
    {L"inoutsine",  EASING::INOUTSINE},
    {L"inquad",     EASING::INQUAD},
    {L"outquad",    EASING::OUTQUAD},
    {L"inoutquad",  EASING::INOUTQUAD},
    {L"incubic",    EASING::INCUBIC},
    {L"outcubic",   EASING::OUTCUBIC},
    {L"inoutcubic", EASING::INOUTCUBIC},
    {L"inquart",    EASING::INQUART},
    {L"outquart",   EASING::OUTQUART},
    {L"inoutquart", EASING::INOUTQUART},
    {L"inquint",    EASING::INQUINT},
    {L"outquint",   EASING::OUTQUINT},
    {L"inoutquint", EASING::INOUTQUINT},
    {L"inexpo",     EASING::INEXPO},
    {L"outexpo",    EASING::OUTEXPO},
    {L"inoutexpo",  EASING::INOUTEXPO},
    {L"incirc",     EASING::INCIRC},
    {L"outcirc",    EASING::OUTCIRC},
    {L"inoutcirc",  EASING::INOUTCIRC},
    {L"inback",     EASING::INBACK},
    {L"outback",    EASING::OUTBACK},
    {L"inoutback",  EASING::INOUTBACK},
    {L"inbounce",   EASING::INBOUNCE},
    {L"outbounce",  EASING::OUTBOUNCE},
    {L"inoutbounce",EASING::INOUTBOUNCE},
    {L"inelastic",  EASING::INELASTIC},
    {L"outelastic", EASING::OUTELASTIC},
    {L"inoutelastic",EASING::INOUTELASTIC},
};

//-------------------------------//
//              API              //
//-------------------------------//

EaseFn GetEaseFn(EASING easing) noexcept
{
    return EasingFunctions[static_cast<uint8_t>(easing)];
}

EASING ParseEasing(LPCWSTR name) noexcept
{
    if (!name || !*name)
        return EASING::UNKNOWN;

    for (const auto& entry : EasingTable)
    {
        if (_wcsicmp(entry.name, name) == 0)
            return entry.value;
    }

    return EASING::UNKNOWN;
}
