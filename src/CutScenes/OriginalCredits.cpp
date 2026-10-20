// SPDX-License-Identifier: GPL-2.0-or-later
#include <CutScenes/OriginalCredits.h>
#include <globals.h>
#include <FileClasses/adl/sound_adlib.h>
#include <FileClasses/music/MusicPlayer.h>
#include <FileClasses/FileManager.h>
#include <FileClasses/Palfile.h>
#include <FileClasses/Shpfile.h>
#include <FileClasses/Cpsfile.h>
#include <FileClasses/TextManager.h>
#include <misc/Scaler.h>
#include <misc/draw_util.h>
#include <algorithm>
#include <string>

namespace {
std::vector<Uint8> readOriginalFile(const std::string& filename) {
    auto file = pFileManager->openFile(filename);
    const auto size = SDL_RWsize(file.get());
    if(size <= 0 || size > 16 * 1024 * 1024) THROW(std::runtime_error, "Invalid credits resource: %s", filename);
    std::vector<Uint8> data(static_cast<std::size_t>(size));
    if(SDL_RWread(file.get(), data.data(), 1, data.size()) != data.size())
        THROW(std::runtime_error, "Cannot read credits resource: %s", filename);
    return data;
}

// Read the original packed-nibble bitmap fonts. Keep CP850 glyph indices and
// the original one-pixel negative spacing; no replacement TTF or translated names.
struct CreditsFont {
    std::vector<Uint8> data;
    int height, widths, offsets, lines, count;
    int word(int at) const { return data.at(at) | (data.at(at + 1) << 8); }
    explicit CreditsFont(const std::string& filename) : data(readOriginalFile(filename)) {
        if(data.at(2) != 0 || data.at(3) != 5) THROW(std::runtime_error, "Invalid credits font");
        height = data.at(word(4) + 4);
        offsets = word(6); widths = word(8); lines = word(12); count = word(10) - widths;
    }
    int width(const std::string& value) const {
        int result = 0;
        for(unsigned char c : value) if(c < count) result += std::max(0, int(data.at(widths + c)) - 1);
        return result;
    }
    void draw(SDL_Surface* surface, const std::string& value, int x, int y) const {
        const auto white = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
        const auto shadow = SDL_MapRGBA(surface->format, 0, 0, 0, 255);
        for(unsigned char c : value) {
            if(c >= count) continue;
            const int w = data.at(widths + c), top = data.at(lines + 2 * c);
            const int h = data.at(lines + 2 * c + 1), start = word(offsets + 2 * c);
            if(start) for(int gy = 0; gy < h; ++gy) for(int gx = 0; gx < w; ++gx) {
                const int value = (data.at(start + gy * ((w + 1) / 2) + gx / 2) >> (4 * (gx % 2))) & 15;
                const int px = x + gx, py = y + top + gy;
                if(value && px >= 0 && px < surface->w && py >= 0 && py < surface->h)
                    static_cast<Uint32*>(static_cast<void*>(static_cast<Uint8*>(surface->pixels) + py * surface->pitch))[px]
                        = value == 1 ? white : shadow;
            }
            x += std::max(0, w - 1);
        }
    }
};
}

OriginalCredits::OriginalCredits(int house) {
    startNewScene();
    addVideoEvent(std::make_unique<OriginalCreditsVideoEvent>(house));
    // The event plays DUNE20 track 2 directly, including with directory music selected.
}

OriginalCreditsVideoEvent::OriginalCreditsVideoEvent(int house) {
    if(musicPlayer)musicPlayer->stopMusic();
    if(settings.audio.playMusic && (!musicPlayer || musicPlayer->isMusicOn()) && Mix_QuerySpec(nullptr,nullptr,nullptr)) {
        SoundAdlibPC decoder(pFileManager->openFile("DUNE20.ADL").get());
        music.reset(decoder.getSubsong(2));
        if(!music)THROW(sdl_error,"Original credits music: %s",SDL_GetError());
    }
    const CreditsFont small("NEW6P.FNT"), large("NEW8P.FNT");
    auto data = readOriginalFile("CREDITS." + _("LanguageFileExtension"));
    struct Line { std::string text; int x, y; bool large; };
    std::vector<Line> rows;
    bool largeFont = true;
    int y = 0;
    for(std::size_t at = 0; at < data.size() && data[at] != 0;) {
        int type = 0;
        if(data[at] >= 1 && data[at] <= 4) {
            type = data[at++];
            if(type == 1) largeFont = false;
            if(type == 2) largeFont = true;
        }
        std::string line;
        while(at < data.size() && data[at] != 0 && data[at] != 5 && data[at] != '\r')
            line += static_cast<char>(data[at++]);
        const auto& font = largeFont ? large : small;
        const int width = font.width(line);
        const int x = type == 3 ? 157 - width : type == 4 ? 161 : 1 + (320 - width) / 2;
        rows.push_back({std::move(line), x, y, largeFont});
        textHeight = std::max(textHeight, y + font.height);
        const int separator = at < data.size() ? data[at++] : 0;
        if(separator != 5) y += font.height + font.height / 8;
        if(separator == 0) break;
    }
    auto pixels = sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0, 320, textHeight, 32, SDL_PIXELFORMAT_RGBA32)};
    if(!pixels) THROW(sdl_error, "Credits text surface: %s", SDL_GetError());
    SDL_FillRect(pixels.get(), nullptr, 0);
    for(const auto& row : rows) (row.large ? large : small).draw(pixels.get(), row.text, row.x, row.y);
    auto doubled = Scaler::doubleSurfaceNN(pixels.get());
    text.reset(SDL_CreateTextureFromSurface(renderer, doubled.get()));
    if(!text) THROW(sdl_error, "Credits text texture: %s", SDL_GetError());
    SDL_SetTextureBlendMode(text.get(), SDL_BLENDMODE_BLEND);

    const auto originalPalette = LoadPalette_RW(pFileManager->openFile("IBM.PAL").get());
    auto planet=LoadCPS_RW(pFileManager->openFile("MAPPLAN.CPS").get());
    if(!planet)THROW(std::runtime_error,"Missing original credits planet");
    Palette planetPalette=originalPalette;
    for(int shade=0;shade<8;++shade)planetPalette[houseToPaletteIndex[HOUSE_HARKONNEN]+shade]=getHouseColorSDL(getHouseVisualHouse(house),shade);
    planetPalette.applyToSurface(planet.get());
    auto scaledPlanet=Scaler::doubleSurfaceNN(planet.get());
    initialPlanet.reset(SDL_CreateTextureFromSurface(renderer,scaledPlanet.get()));
    if(!initialPlanet)THROW(sdl_error,"Original credits planet: %s",SDL_GetError());
    SDL_SetTextureBlendMode(initialPlanet.get(),SDL_BLENDMODE_BLEND);
    for(int i = 1; i <= 11; ++i) {
        Shpfile file(pFileManager->openFile("CREDIT" + std::to_string(i) + ".SHP").get());
        auto image = file.getPicture(0);
        originalPalette.applyToSurface(image.get());
        auto scaled = Scaler::doubleSurfaceNN(image.get());
        pictures.emplace_back(SDL_CreateTextureFromSurface(renderer, scaled.get()));
        if(!pictures.back()) THROW(sdl_error, "Credits picture texture: %s", SDL_GetError());
        SDL_SetTextureBlendMode(pictures.back().get(), SDL_BLENDMODE_BLEND);
    }
}

OriginalCreditsVideoEvent::~OriginalCreditsVideoEvent() {
    if(channel>=0 && Mix_GetChunk(channel)==music.get()) {
        Mix_HaltChannel(channel);Mix_Volume(channel,previousVolume);
    }
}
int OriginalCreditsVideoEvent::draw() {
    if(!audioStarted && music) {
        audioStarted=true;
        const int available=Mix_GroupAvailable(-1);
        if(available>=0) {
            previousVolume=Mix_Volume(available,-1);Mix_Volume(available,settings.audio.musicVolume);
            channel=Mix_PlayChannel(available,music.get(),-1);
            if(channel<0)Mix_Volume(available,previousVolume);
        }
    }
    // The original picture cycle fades separately from the white text.
    switch(stage) {
        case 0: pictureAlpha = 0; stage = 1; counter = 2; break;
        case 1: case 4:
            if(counter-- <= 0) { ++stage; counter = 0; }
            break;
        case 2:
            pictureIndex = (pictureIndex + 1) % static_cast<int>(pictures.size());
            positionIndex = (positionIndex + 1) % 6;
            pictureAlpha = 0; stage = 3; counter = 8;
            break;
        case 3:
            pictureAlpha = 255 * (9 - counter) / 9;
            if(counter-- == 0) { stage = 4; counter = 20; }
            break;
        case 5:
            pictureAlpha = 255 * (9 - counter) / 9;
            if(counter++ >= 8) stage = 0;
            break;
    }

    const auto screen = getRendererSize();
    const int x = screen.x + (screen.w - 640) / 2, y = screen.y + (screen.h - 400) / 2;
    const SDL_Rect clip{x + 32, y + 48, 576, 304};
    SDL_Rect oldClip{};
    const bool hadClip = SDL_RenderIsClipEnabled(renderer);
    SDL_RenderGetClipRect(renderer, &oldClip);
    SDL_RenderSetClipRect(renderer, &clip);

    if(pictureAlpha > 0) {
        auto* picture = pictureIndex<0?initialPlanet.get():pictures[pictureIndex].get();
        int w, h; SDL_QueryTexture(picture, nullptr, nullptr, &w, &h);
        const bool right = positionIndex == 0 || positionIndex == 2 || positionIndex == 4;
        const int vertical = positionIndex % 3;
        const SDL_Rect destination=pictureIndex<0?SDL_Rect{x,y,w,h}
            :SDL_Rect{clip.x + (right ? clip.w - w : 0),clip.y + vertical * (clip.h - h) / 2,w,h};
        SDL_SetTextureAlphaMod(picture, static_cast<Uint8>(pictureAlpha));
        SDL_RenderCopy(renderer, picture, nullptr, &destination);
    }
    const SDL_Rect destination{x, clip.y + clip.h - 2 * scroll, 640, textHeight * 2};
    SDL_RenderCopy(renderer, text.get(), nullptr, &destination);
    SDL_RenderSetClipRect(renderer, hadClip ? &oldClip : nullptr);
    if(++scroll > textHeight + 152) scroll = 0;
    return 100; // One original pixel every six 60 Hz ticks.
}
