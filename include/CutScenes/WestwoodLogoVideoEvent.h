// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <CutScenes/VideoEvent.h>
#include <FileClasses/Palette.h>
#include <FileClasses/Wsafile.h>
#include <SDL_mixer.h>

/// The original Dune II Westwood logo, palette and AdLib signature.
class WestwoodLogoVideoEvent final : public VideoEvent {
public:
    WestwoodLogoVideoEvent();
    ~WestwoodLogoVideoEvent() override;

    int draw() override;
    bool isFinished() override;

private:
    friend struct IntroLogoFixture;

    Wsafile animation;
    Palette logoPalette;
    sdl2::texture_ptr texture;
    sdl2::mix_chunk_ptr signature;
    int frame = 0;
    int channel = -1;
    int previousVolume = MIX_MAX_VOLUME;
};
