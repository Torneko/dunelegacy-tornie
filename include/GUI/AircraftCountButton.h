// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <GUI/TextButton.h>
#include <FileClasses/FontManager.h>
#include <globals.h>
#include <algorithm>

/// Selection shortcut: keep the translated name and parenthesized count legible
/// in the narrow sidebar, without changing the button's footprint.
class AircraftCountButton : public TextButton {
public:
    void setCountedText(const std::string& name, int count) {
        const auto value = name + " (" + std::to_string(count) + ")";
        if(getText() == value) return;
        caption = name;
        counter = "(" + std::to_string(count) + ")";
        setText(value);
    }
    Point getMinimumSize() const override { return Point(104,36); }

protected:
    void updateTextures() override {
        Button::updateTextures();
        if(pUnpressedTexture) return;
        const bool split = pFontManager->getTextWidth(getText(),12) > getSize().x-8;
        const auto label = split ? caption : getText();
        int font = 12;
        while(font > 8 && pFontManager->getTextWidth(label,font) > getSize().x-8) --font;
        const auto surface = [&](bool pressed,bool active) {
            auto image = GUIStyle::getInstance().createButtonSurface(
                getSize().x,getSize().y,"",pressed,active,getTextColor(),getTextShadowColor());
            Uint32 colour = getTextColor();
            if(active) {
                const auto c = RGBA2SDL(colour);
                colour = COLOR_RGB(std::min(255,int(c.r)*3/2),std::min(255,int(c.g)*3/2),std::min(255,int(c.b)*3/2));
            }
            const auto shadow = getTextShadowColor()==COLOR_DEFAULT ? COLOR_LIGHTYELLOW : getTextShadowColor();
            const auto text = [&](const std::string& value,int y,int size) {
                auto dark = pFontManager->createSurfaceWithText(value,shadow,size);
                auto light = pFontManager->createSurfaceWithText(value,colour,size);
                const int x = (getSize().x-light->w)/2 + (pressed?1:0);
                SDL_Rect back{x+1,y+1+(pressed?1:0),dark->w,dark->h};
                SDL_Rect front{x,y+(pressed?1:0),light->w,light->h};
                SDL_BlitSurface(dark.get(),nullptr,image.get(),&back);
                SDL_BlitSurface(light.get(),nullptr,image.get(),&front);
            };
            if(split) {
                text(label,2,font);
                text(counter,getSize().y-pFontManager->getTextHeight(12)-3,12);
            } else text(label,(getSize().y-pFontManager->getTextHeight(font))/2,font);
            return image;
        };
        setSurfaces(surface(false,false),surface(true,true),surface(false,true));
    }

private:
    std::string caption, counter;
};
