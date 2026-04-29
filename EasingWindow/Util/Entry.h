#pragma once

#include <windows.h>

template<typename T>
struct Entry
{
    const wchar_t* name;
    T value;
};

template<typename T, size_t N>
bool ParseEnum(const wchar_t* name, const Entry<T>(&table)[N], T& output)
{
    for (size_t i = 0; i < N; ++i)
    {
        if (_wcsicmp(name, table[i].name) == 0)
        {
            output = table[i].value;
            return true;
        }
    }
    output = table[0].value;
    return false;
}
