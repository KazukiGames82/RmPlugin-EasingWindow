#pragma once

#include <string>
//#include <algorithm>

inline std::wstring TrimString(const std::wstring& text) noexcept
{
    size_t start = 0;
    size_t end = text.size();

    while (start < end && iswspace(text[start])) ++start;
    while (end > start && iswspace(text[end - 1])) --end;

    return text.substr(start, end - start);
}

inline std::wstring DuplicateBackslashes(const std::wstring& text, int duplicateCount) noexcept
{
    if (duplicateCount <= 0) return text;

    std::wstring result;
    result.reserve(text.size());

    const std::wstring backslashes(duplicateCount, L'\\');

    for (size_t i = 0; i < text.size();)
    {
        if (text[i] == L'\\')
        {
            while (i < text.size() && text[i] == L'\\')
            {
                ++i;
            }
            result += backslashes;
        }
        else
        {
            result += text[i++];
        }
    }

    return result;
}