#include "Math.h"
#include "StringUtil.h"
#include <cstdlib>

int ParseAnchorPosition(const std::wstring& anchorValue, int skinSize) noexcept
{
    std::wstring trimmedValue = TrimString(anchorValue);
    if (trimmedValue.empty())
        return 0;

    bool isPercentage = false;
    bool isRelative = false;

    while (!trimmedValue.empty())
    {
        wchar_t lastCharacter = trimmedValue.back();
        if (lastCharacter == L'%')
        {
            isPercentage = true;
            trimmedValue.pop_back();
        }
        else if (lastCharacter == L'R' || lastCharacter == L'B')
        {
            isRelative = true;
            trimmedValue.pop_back();
        }
        else
            break;
    }

    trimmedValue = TrimString(trimmedValue);

    wchar_t* parseEnd = nullptr;
    double numericalValue = std::wcstod(trimmedValue.c_str(), &parseEnd);
    if (parseEnd == trimmedValue.c_str())
        return skinSize;

    if (isPercentage)
        numericalValue = numericalValue * skinSize / 100.0;

    int result = static_cast<int>(numericalValue);
    if (isRelative)
        result = skinSize - result;

    return result;
}