#pragma once

#include <windows.h>
#include "Config.h"
#include "Direction.h"

PointInt GetStartPosition(HWND hwnd, const Config& config, Direction direction) noexcept;