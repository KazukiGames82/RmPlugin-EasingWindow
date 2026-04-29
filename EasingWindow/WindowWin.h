#pragma once

#include <Windows.h>
#include "Config.h"
#include "Direction.h"

PointInt GetStartPosition(const HWND hwnd, const Config& config, const DIRECTION direction) noexcept;