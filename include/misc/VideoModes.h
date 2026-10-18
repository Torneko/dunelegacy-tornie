#pragma once

#include <DataTypes.h>
#include <algorithm>
#include <vector>

// Borderless fullscreen uses the current monitor, not an exclusive video mode.
// Window sizes need not appear in SDL's (often one-entry) list of video modes.
namespace VideoModes {
inline Coord displaySize(int displayIndex, bool usable = false) {
    SDL_Rect bounds{};
    if((usable ? SDL_GetDisplayUsableBounds(displayIndex, &bounds)
               : SDL_GetDisplayBounds(displayIndex, &bounds)) == 0 && bounds.w > 0 && bounds.h > 0)
        return Coord(bounds.w, bounds.h);
    SDL_DisplayMode mode{};
    if(SDL_GetDesktopDisplayMode(displayIndex, &mode) == 0 && mode.w > 0 && mode.h > 0)
        return Coord(mode.w, mode.h);
    return Coord(SCREEN_MIN_WIDTH, SCREEN_MIN_HEIGHT);
}

inline Coord fit(Coord requested, Coord bounds) {
    bounds.x = std::max(SCREEN_MIN_WIDTH, bounds.x);
    bounds.y = std::max(SCREEN_MIN_HEIGHT, bounds.y);
    if(requested.x < SCREEN_MIN_WIDTH || requested.y < SCREEN_MIN_HEIGHT) return bounds;
    // A stale combined-desktop setting must not carry its aspect ratio onto
    // a single monitor. Use that monitor's dimensions when the setting exceeds it.
    if(requested.x > bounds.x || requested.y > bounds.y) return bounds;
    return requested;
}

inline std::vector<Coord> choices(Coord bounds, const std::vector<Coord>& reported, Coord current) {
    bounds = fit(bounds, bounds);
    std::vector<Coord> result;
    auto add = [&](Coord size) {
        if(size.x >= SCREEN_MIN_WIDTH && size.y >= SCREEN_MIN_HEIGHT && size.x <= bounds.x && size.y <= bounds.y
           && std::find(result.begin(), result.end(), size) == result.end()) result.push_back(size);
    };
    for(const Coord size : {Coord(640,480), Coord(800,480), Coord(800,600), Coord(960,540), Coord(960,720),
        Coord(1024,576), Coord(1024,640), Coord(1024,768), Coord(1152,864), Coord(1280,720), Coord(1280,768),
        Coord(1280,800), Coord(1280,960), Coord(1280,1024), Coord(1366,768), Coord(1400,1050), Coord(1440,900),
        Coord(1440,1080), Coord(1600,900), Coord(1600,1200), Coord(1680,1050), Coord(1920,1080),
        Coord(1920,1200), Coord(2560,1440), Coord(2560,1600), Coord(3440,1440), Coord(3840,2160)}) add(size);
    for(const Coord size : reported) add(size);
    add(bounds);
    add(current);
    std::sort(result.begin(), result.end(), [](Coord a, Coord b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    return result;
}
}
