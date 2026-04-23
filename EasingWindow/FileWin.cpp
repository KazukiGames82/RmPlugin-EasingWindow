#include "FileWin.h"
#include <windows.h>

void GetAnchors(LPCWSTR path, LPCWSTR section, std::wstring& anchorX, std::wstring& anchorY) noexcept
{
    wchar_t buf[256]{};

    GetPrivateProfileStringW(section, L"AnchorX", L"0", buf, 256, path);
    anchorX = buf;

    GetPrivateProfileStringW(section, L"AnchorY", L"0", buf, 256, path);
    anchorY = buf;
}