// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <CutScenes/CutScene.h>
#include <vector>
#include <DataTypes.h>
#include <SDL_mixer.h>

class OriginalCredits : public CutScene {
public:
    explicit OriginalCredits(int house=HOUSE_HARKONNEN);
};

class OriginalCreditsVideoEvent final : public VideoEvent {
public:
    explicit OriginalCreditsVideoEvent(int house=HOUSE_HARKONNEN);
    ~OriginalCreditsVideoEvent() override;
    int draw() override;
    bool isFinished() override { return false; } // Original credits repeat until skipped.

private:
    friend struct IntroLogoFixture;
    sdl2::texture_ptr text;
    sdl2::texture_ptr initialPlanet;
    sdl2::mix_chunk_ptr music;
    int channel=-1;
    int previousVolume=MIX_MAX_VOLUME;
    bool audioStarted=false;
    std::vector<sdl2::texture_ptr> pictures;
    int textHeight = 0;
    int scroll = 0;
    int pictureIndex = -1;
    int positionIndex = -1;
    int stage = 4;
    int counter = 60;
    int pictureAlpha = 255;
};
