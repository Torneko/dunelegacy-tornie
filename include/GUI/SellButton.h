// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <GUI/PictureButton.h>
#include <FileClasses/GFXManager.h>
#include <globals.h>
#include <misc/draw_util.h>

// Reuse the repair button's native frame and pressed state, with a pixel dollar.
class SellButton : public PictureButton {
public:
    SellButton() {
        auto normal=makeFace(UI_Repair,false),pressed=makeFace(UI_Repair_Pressed,true);
        setSurfaces(std::move(normal),std::move(pressed));
    }
private:
    static sdl2::surface_ptr makeFace(int graphic,bool pressed) {
        auto* original=pGFXManager->getUIGraphicSurface(graphic);
        sdl2::surface_ptr face{SDL_ConvertSurfaceFormat(original,SDL_PIXELFORMAT_RGBA32,0)};
        if(!face)return face;
        SDL_Rect inside{2,2,face->w-5,face->h-5};
        SDL_FillRect(face.get(),&inside,SDL_MapRGB(face->format,202,141,16));
        constexpr const char* dollar[]={"00100","01111","10100","10100","01110","00101","00101","11110","00100"};
        const int x=(face->w-5)/2+(pressed?1:0),y=(face->h-9)/2+(pressed?1:0);
        for(int pass=0;pass<2;++pass)for(int row=0;row<9;++row)for(int col=0;col<5;++col)if(dollar[row][col]=='1') {
            SDL_Rect pixel{x+col+(pass==0),y+row+(pass==0),1,1};
            SDL_FillRect(face.get(),&pixel,pass==0?SDL_MapRGB(face->format,64,64,89):SDL_MapRGB(face->format,224,224,201));
        }
        return face;
    }
};
