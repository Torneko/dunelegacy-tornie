// SPDX-License-Identifier: GPL-2.0-or-later
#include <CutScenes/WestwoodLogoVideoEvent.h>

#include <FileClasses/FileManager.h>
#include <FileClasses/Palfile.h>
#include <FileClasses/adl/sound_adlib.h>
#include <FileClasses/music/MusicPlayer.h>
#include <globals.h>
#include <misc/Scaler.h>
#include <misc/draw_util.h>

#include <algorithm>

WestwoodLogoVideoEvent::WestwoodLogoVideoEvent()
    : animation(pFileManager->openFile("WESTWOOD.WSA").get()),
      logoPalette(LoadPalette_RW(pFileManager->openFile("WESTWOOD.PAL").get())) {
    texture.reset(SDL_CreateTexture(renderer, SCREEN_FORMAT, SDL_TEXTUREACCESS_STREAMING,
                                    animation.getWidth() * 2, animation.getHeight() * 2));
    if(!texture) THROW(sdl_error, "Westwood logo texture: %s", SDL_GetError());
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);

    // Dune II plays music 0x24: DUNE0, track 4. Keep the original AdLib
    // signature even when the selected background-music source is a directory.
    if(musicPlayer)musicPlayer->stopMusic();
    if(settings.audio.playMusic && (!musicPlayer || musicPlayer->isMusicOn()) && Mix_QuerySpec(nullptr, nullptr, nullptr)) {
        SoundAdlibPC decoder(pFileManager->openFile("DUNE0.ADL").get());
        // The decoder defaults to half volume. Use its full output here, while
        // the mixer still applies the user's music volume (including zero).
        decoder.setVolume(MIX_MAX_VOLUME);
        signature.reset(decoder.getSubsong(4));
        if(!signature) THROW(sdl_error, "Westwood signature: %s", SDL_GetError());
    }
}

WestwoodLogoVideoEvent::~WestwoodLogoVideoEvent() {
    // The event owns the chunk. Stop it before freeing it, including when the
    // player skips the intro; do not affect a channel subsequently reused.
    if(channel >= 0 && Mix_GetChunk(channel) == signature.get()) {
        Mix_HaltChannel(channel);
        Mix_Volume(channel, previousVolume);
    }
}

int WestwoodLogoVideoEvent::draw() {
    // Original timing: fade in for 60 ticks, then start the signature and
    // advance WSA frames every 6 ticks. Hold until 360 ticks, fade out for 60.
    // At 60 Hz, one step here is 100 ms (six original ticks).
    if(frame == 10 && signature) {
        const int available = Mix_GroupAvailable(-1);
        if(available >= 0) {
            previousVolume = Mix_Volume(available, -1);
            Mix_Volume(available, settings.audio.musicVolume);
            channel = Mix_PlayChannel(available, signature.get(), 0);
            if(channel < 0) Mix_Volume(available, previousVolume);
        }
    }

    if(frame < 80) {
        const int sourceFrame = std::clamp(frame - 9, 0, animation.getNumFrames() - 1);
        auto picture = animation.getPicture(sourceFrame);
        // Wsafile normally uses the active house/mod palette. This sequence
        // has its own palette and must never recolour the global game palette.
        logoPalette.applyToSurface(picture.get());
        auto pixels = convertSurfaceToDisplayFormat(Scaler::doubleSurfaceNN(picture.get()).get());
        SDL_UpdateTexture(texture.get(), nullptr, pixels->pixels, pixels->pitch);

        const int alpha = frame < 10 ? 255 * frame / 10
                        : frame >= 70 ? 255 * (80 - frame) / 10 : 255;
        SDL_SetTextureAlphaMod(texture.get(), static_cast<Uint8>(alpha));
        const auto screen = getRendererSize();
        // Preserve (0,0) within the original 320x200 canvas, doubled exactly.
        const SDL_Rect destination{screen.x + (screen.w - 640) / 2,
                                   screen.y + (screen.h - 400) / 2,
                                   animation.getWidth() * 2, animation.getHeight() * 2};
        SDL_RenderCopy(renderer, texture.get(), nullptr, &destination);
    }
    ++frame;
    return 100;
}

bool WestwoodLogoVideoEvent::isFinished() {
    // The original waits on black for the signature to finish before the next
    // sequence. Music disabled keeps the same visual timing without waiting.
    return frame >= 80 && (channel < 0 || Mix_GetChunk(channel) != signature.get()
                                       || !Mix_Playing(channel));
}
