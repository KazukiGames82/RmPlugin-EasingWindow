#include <windows.h>
#include "FileWin.h"

void GetAnchors(const wchar_t* path, const wchar_t*  section, std::wstring& anchorX, std::wstring& anchorY) noexcept
{
    wchar_t buf[256]{};

    GetPrivateProfileStringW(section, L"AnchorX", L"0", buf, 256, path);
    anchorX = buf;

    GetPrivateProfileStringW(section, L"AnchorY", L"0", buf, 256, path);
    anchorY = buf;
}