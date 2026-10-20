// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <CutScenes/CutScene.h>
#include <vector>

class OriginalCredits : public CutScene {
public:
    OriginalCredits();
};

class OriginalCreditsVideoEvent final : public VideoEvent {
public:
    OriginalCreditsVideoEvent();
    int draw() override;
    bool isFinished() override { return false; } // Original credits repeat until skipped.

private:
    friend struct IntroLogoFixture;
    sdl2::texture_ptr text;
    std::vector<sdl2::texture_ptr> pictures;
    int textHeight = 0;
    int scroll = 0;
    int pictureIndex = -1;
    int positionIndex = -1;
    int stage = 4;
    int counter = 60;
    int pictureAlpha = 0;
};
