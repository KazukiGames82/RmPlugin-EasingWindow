#pragma once

#include <windows.h>
#include <string>

void GetAnchors(LPCWSTR path, LPCWSTR section, std::wstring& anchorX, std::wstring& anchorY) noexcept;