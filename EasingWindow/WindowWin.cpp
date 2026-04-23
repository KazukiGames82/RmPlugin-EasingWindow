#include "WindowWin.h"

PointInt GetStartPosition(HWND hwnd, const Config& config, Direction direction) noexcept
{
    RECT rc{};
    GetWindowRect(hwnd, &rc);

    const int width = rc.right - rc.left;
    const int height = rc.bottom - rc.top;

    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi{ sizeof(mi) };
    GetMonitorInfo(monitor, &mi);

    const RECT& screen = mi.rcMonitor;

    PointInt a
    {
        config.Anchor().x * (1 - config.IgnoreAnchors()),
        config.Anchor().y * (1 - config.IgnoreAnchors())
    };

    PointInt out = config.End();

    switch (direction)
    {
        case Direction::LEFTTOP:
            out.x = screen.left - width + a.x;
            out.y = screen.top - height + a.y;
            break;
        case Direction::TOP:
            out.x = config.End().x;
            out.y = screen.top - height + a.y;
            break;
        case Direction::RIGHTTOP:
            out.x = screen.right + a.x;
            out.y = screen.top - height + a.y;
            break;
        case Direction::LEFT:
            out.x = screen.left - width + a.x;
            out.y = config.End().y;
            break;
        case Direction::RIGHT:
            out.x = screen.right + a.x;
            out.y = config.End().y;
            break;
        case Direction::LEFTBOTTOM:
            out.x = screen.left - width + a.x;
            out.y = screen.bottom + a.y;
            break;
        case Direction::BOTTOM:
            out.x = config.End().x;
            out.y = screen.bottom + a.y;
            break;
        case Direction::RIGHTBOTTOM:
            out.x = screen.right + a.x;
            out.y = screen.bottom + a.y;
            break;
        default:
            break;
    }

    return out;
}