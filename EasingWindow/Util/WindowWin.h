#pragma once

#include <Windows.h>

#include "../Dependecies/Config.h"
#include "../Dependecies/Direction.h"
#include "Point.h"

PointInt GetStartPosition(const HWND hwnd, const Config& config, const DIRECTION direction) noexcept;