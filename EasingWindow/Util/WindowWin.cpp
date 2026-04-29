#include "WindowWin.h"

PointInt GetStartPosition(const HWND hwnd, const Config& config, const DIRECTION direction) noexcept
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
        case DIRECTION::LEFTTOP:
            out.x = screen.left - width + a.x;
            out.y = screen.top - height + a.y;
            break;
        case DIRECTION::TOP:
            out.x = config.End().x;
            out.y = screen.top - height + a.y;
            break;
        case DIRECTION::RIGHTTOP:
            out.x = screen.right + a.x;
            out.y = screen.top - height + a.y;
            break;
        case DIRECTION::LEFT:
            out.x = screen.left - width + a.x;
            out.y = config.End().y;
            break;
        case DIRECTION::RIGHT:
            out.x = screen.right + a.x;
            out.y = config.End().y;
            break;
        case DIRECTION::LEFTBOTTOM:
            out.x = screen.left - width + a.x;
            out.y = screen.bottom + a.y;
            break;
        case DIRECTION::BOTTOM:
            out.x = config.End().x;
            out.y = screen.bottom + a.y;
            break;
        case DIRECTION::RIGHTBOTTOM:
            out.x = screen.right + a.x;
            out.y = screen.bottom + a.y;
            break;
        default:
            break;
    }

    return out;
}