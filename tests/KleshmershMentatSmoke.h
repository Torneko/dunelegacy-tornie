#pragma once
#include "RebelsMentatSmoke.h"
#include <FileClasses/LoadSavePNG.h>
#include <Menu/BriefingMenu.h>

inline void verifyKleshmershMentat(const std::string& output, const std::string& mod) {
    if(mod != "Tornie" && mod != "Jericho" && mod != "ArcaKev" && mod != "vanilla") return;
    auto require = [](bool value, const std::string& message) {
        if(!value) throw std::runtime_error("Kleshmersh mentat check: " + message);
    };
    const auto& info = ModManager::instance().getActiveMentatInfo(HOUSE_KLESHMERSH);
    require(info.enabled && info.backgroundAsset == "MentatHusky_Kleshmersh.png",
            "Kleshmersh identity resolved to another portrait");
    auto* background = pGFXManager->getUIGraphicSurface(UI_MentatBackground, HOUSE_KLESHMERSH);
    auto* eyes = pGFXManager->getMentatEyesAnimation(HOUSE_KLESHMERSH);
    auto* mouth = pGFXManager->getMentatMouthAnimation(HOUSE_KLESHMERSH);
    require(background && background->w == 640 && background->h == 400, "native UI canvas");
    require(eyes && mouth && eyes->getNumberOfFrames() == 5 && mouth->getNumberOfFrames() == 5,
            "five eye and speech states");
    for(const auto& frame : eyes->getFrames())
        require(frame && frame->w == 126 && frame->h == 34, "eye strip decoding");
    for(const auto& frame : mouth->getFrames())
        require(frame && frame->w == 100 && frame->h == 46, "mouth strip decoding");

    const auto runtimeHouse = getRuntimeHouseForIdentity(HOUSE_KLESHMERSH);
    require(getHouseFactionIdentity(runtimeHouse) == HOUSE_KLESHMERSH, "Jericho faction alias");
    BriefingMenu menu(runtimeHouse, 15, BRIEFING);
    menu.setText("");
    menu.onMentatTextFinished();
    auto screen = sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(
        0, getRendererWidth(), getRendererHeight(), 32, SDL_PIXELFORMAT_RGBA32)};
    auto reference = sdl2::surface_ptr{SDL_ConvertSurfaceFormat(background, SDL_PIXELFORMAT_RGBA32, 0)};
    require(screen && reference, "render surfaces");
    for(int state = 0; state < 5; ++state) {
        eyes->setFrameOverride(state);
        mouth->setFrameOverride(state);
        SDL_RenderSetClipRect(renderer, nullptr);
        SDL_RenderClear(renderer);
        menu.draw();
        require(SDL_RenderReadPixels(renderer, nullptr, screen->format->format,
                                     screen->pixels, screen->pitch) == 0, "briefing render capture");
        // Check the actual menu rather than just the asset: the Jericho runtime
        // slot differs from the faction identity. The nose must remain fixed.
        for(int y = 202; y < 248; ++y) for(int x = 104; x < 152; ++x) {
            const auto* source = static_cast<Uint8*>(reference->pixels) + y * reference->pitch + x * 4;
            const auto* target = static_cast<Uint8*>(screen->pixels)
                + (y + menu.getPosition().y) * screen->pitch + (x + menu.getPosition().x) * 4;
            require(std::memcmp(source, target, 3) == 0, "nose moved or wrong portrait rendered");
        }
        require(SavePNG(screen.get(), (std::filesystem::path(output)
            / (mod + "-kleshmersh-" + std::to_string(state) + ".png")).string().c_str()) == 0,
            "animation PNG output");
    }
    eyes->resetFrameOverride();
    mouth->resetFrameOverride();
    menu.setText("Kleshmersh. Briefing.");
    menu.update();
    require(mouth->getLoopsLeft() > 0, "briefing did not start speech");
    require(ModManager::instance().getActiveMentatInfo(HOUSE_NEUTRAL).backgroundAsset == "ChaniMentat.png",
            "Neutral portrait changed");
    SDL_Log("KLESHMERSH MENTAT PASS: %s, five states, actual faction briefing, stable nose and speech", mod.c_str());
}
