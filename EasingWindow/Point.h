#pragma once

struct PointDouble
{
    double x = 0.0;
    double y = 0.0;
};

struct PointInt
{
    int x = 0;
    int y = 0;
};

inline PointDouble ConvertToDouble(const PointInt& point) noexcept
{
    return { double(point.x), double(point.y) };
}

inline constexpr PointDouble operator+(const PointDouble& first, const PointDouble& second) noexcept
{
    return { first.x + second.x, first.y + second.y };
}

inline constexpr PointDouble operator-(const PointDouble& first, const PointDouble& second) noexcept
{
    return { first.x - second.x, first.y - second.y };
}

inline constexpr PointDouble operator*(const PointDouble& point, double scalar) noexcept
{
    return { point.x * scalar, point.y * scalar };
}

inline constexpr PointDouble operator*(double scalar, const PointDouble& point) noexcept
{
    return point * scalar;
}

inline constexpr PointDouble operator/(const PointDouble& point, double scalar) noexcept
{
    return { point.x / scalar, point.y / scalar };
}

inline PointDouble& operator+=(PointDouble& first, const PointDouble& second) noexcept
{
    first.x += second.x;
    first.y += second.y;
    return first;
}

inline PointInt ConvertToInt(const PointDouble& point) noexcept
{
    return { int(point.x), int(point.y) };
}

inline constexpr bool operator==(const PointInt& first, const PointInt& second) noexcept
{
    return first.x == second.x && first.y == second.y;
}

inline constexpr bool operator!=(const PointInt& first, const PointInt& second) noexcept
{
    return !(first == second);
}

inline constexpr PointInt operator+(const PointInt& first, const PointInt& second) noexcept
{
    return { first.x + second.x, first.y + second.y };
}

inline constexpr PointInt operator-(const PointInt& first, const PointInt& second) noexcept
{
    return { first.x - second.x, first.y - second.y };
}

inline PointInt& operator+=(PointInt& first, const PointInt& second) noexcept
{
    first.x += second.x;
    first.y += second.y;
    return first;
}