#pragma once
#include <Menu/OptionsMenu.h>
#include <misc/VideoModes.h>
#include <main.h>
#include <globals.h>
#include <filesystem>

struct VideoOptionsFixture {
    static void verify() {
        auto require = [](bool ok, const char* message) {
            if(!ok) throw std::runtime_error(std::string("VIDEO CHECK: ") + message);
        };
        const int display = std::max(0, SDL_GetWindowDisplayIndex(window));
        const Coord bounds = VideoModes::displaySize(display);
        int width = 0, height = 0;
        SDL_GetWindowSize(window, &width, &height);
        require(width <= bounds.x && height <= bounds.y, "window spans beyond its monitor");
        if(const char* expected = std::getenv("DUNELEGACY_EXPECT_VIDEO_SIZE")) {
            int w = 0, h = 0;
            require(std::sscanf(expected, "%dx%d", &w, &h) == 2, "invalid expected test size");
            require(settings.video.physicalWidth == w && settings.video.physicalHeight == h,
                    "requested resolution was replaced by a desktop mode");
        }
        OptionsMenu menu;
        require(!menu.availScreenRes.empty(), "resolution list is empty");
        require(menu.resolutionDropDownBox.getSelectedEntryIntData() >= 0, "current resolution is missing");
        require(std::find(menu.availScreenRes.begin(), menu.availScreenRes.end(), Coord(640,480)) != menu.availScreenRes.end(),
                "smaller window resolutions disappeared");
        const int selected = menu.resolutionDropDownBox.getSelectedIndex();
        menu.resolutionDropDownBox.setSelectedItem(0);
        require(menu.resolutionDropDownBox.getSelectedEntryIntData() == 0, "resolution cannot be selected");
        menu.resolutionDropDownBox.setSelectedItem(selected);
        SDL_SetRenderTarget(renderer, screenTexture);
        SDL_RenderClear(renderer);
        menu.draw();
        require(menu.resolutionDropDownBox.handleMouseLeft(5,5,true), "resolution dropdown did not open");
        menu.draw();
        if(const char* output = std::getenv("DUNELEGACY_SMOKE_DIR")) {
            auto pixels = sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0, settings.video.width, settings.video.height,
                                                                          32, SDL_PIXELFORMAT_ARGB8888));
            require(pixels && SDL_RenderReadPixels(renderer, nullptr, pixels->format->format, pixels->pixels, pixels->pitch) == 0,
                    "options screenshot");
            require(SDL_SaveBMP(pixels.get(), (std::filesystem::path(output) / "video-options.bmp").string().c_str()) == 0,
                    "save options screenshot");
        }
        SDL_SetRenderTarget(renderer, nullptr);
        const bool wasFullscreen = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
        toogleFullscreen();
        require(((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) != wasFullscreen, "fullscreen toggle");
        SDL_GetWindowSize(window, &width, &height);
        require(width <= bounds.x && height <= bounds.y, "fullscreen spans beyond its monitor");
        toogleFullscreen();
        require(((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) == wasFullscreen, "fullscreen restore");
        SDL_GetWindowSize(window, &width, &height);
        require(width <= bounds.x && height <= bounds.y, "restored window spans beyond its monitor");
        SDL_Log("VIDEO CHECK PASSED: display=%d monitor=%dx%d selected=%dx%d choices=%zu window=%dx%d fullscreen round-trip",
                display, bounds.x, bounds.y, settings.video.physicalWidth, settings.video.physicalHeight,
                menu.availScreenRes.size(), width, height);
    }
};
