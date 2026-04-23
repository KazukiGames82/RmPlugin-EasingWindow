#pragma once

#include <cmath>
#include <string>

template<typename T>
inline constexpr T ClampValue(T value, T minimum, T maximum) noexcept
{
    return value < minimum ? minimum : (value > maximum ? maximum : value);
}

template<typename T>
inline constexpr T MaxValue(T value, T maximum) noexcept
{
    return value > maximum ? maximum : value;
}

template<typename T>
inline constexpr T MinValue(T value, T minimum) noexcept
{
    return value < minimum ? minimum : value;
}

inline bool HasSpringConverged(double deltaX, double deltaY, double velocityX, double velocityY) noexcept
{
    return (std::abs(deltaX) < 0.5 &&
            std::abs(deltaY) < 0.5 &&
            std::abs(velocityX) < 0.1 &&
            std::abs(velocityY) < 0.1);
}

int ParseAnchorPosition(const std::wstring& anchorValue, int skinSize) noexcept;