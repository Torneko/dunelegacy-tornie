#pragma once

#include <FileClasses/GFXManager.h>
#include <misc/DrawingRectHelper.h>
#include <filesystem>
#include <stdexcept>

// Exercise the final GPU textures, including lazy house-colour remapping.
inline void verifyCustomUnitZooms(const std::string& output, const std::string& mod) {
    if(!ModManager::instance().isTornieContentActive()) return;
    auto require = [](bool ok, const std::string& message) {
        if(!ok) throw std::runtime_error("Custom turret zoom: " + message);
    };
    const unsigned graphics[] = {
        ObjPic_Tank_Base, ObjPic_Siegetank_Base, ObjPic_Harvester, ObjPic_RebelHarvester,
        ObjPic_DeviatorGunTornie, ObjPic_FlameTankGunTornie,
        ObjPic_EliteLauncherGunTornie, ObjPic_EliteSiegeTankGunTornie,
        ObjPic_ChemicalSiegeTankGunTornie, ObjPic_HarvestankGunTornie,
        ObjPic_RebelSonicTankGun
    };
    for(const unsigned id : graphics) {
        for(int house = 0; house < NUM_HOUSES; ++house) {
            int nativeW = 0, nativeH = 0;
            require(SDL_QueryTexture(pGFXManager->getZoomedObjPic(id,house,0),
                    nullptr,nullptr,&nativeW,&nativeH)==0,"native texture");
            for(unsigned zoom = 0; zoom < NUM_ZOOMLEVEL; ++zoom) {
                int w = 0, h = 0;
                require(SDL_QueryTexture(pGFXManager->getZoomedObjPic(id,house,zoom),
                        nullptr,nullptr,&w,&h)==0,"zoom texture");
                require(w==nativeW*static_cast<int>(zoom+1)
                        && h==nativeH*static_cast<int>(zoom+1),
                        mod+" sprite "+std::to_string(id)+" has inconsistent 1x/2x/3x proportions");
            }
        }
    }
    // Contact sheet: each row is a vehicle, columns are 1x / 2x / 3x,
    // with four headings per zoom. Offsets match the runtime blit functions.
    const unsigned turrets[] = {ObjPic_DeviatorGunTornie,ObjPic_FlameTankGunTornie,
        ObjPic_EliteLauncherGunTornie,ObjPic_EliteSiegeTankGunTornie,
        ObjPic_ChemicalSiegeTankGunTornie,ObjPic_HarvestankGunTornie};
    SDL_RenderSetClipRect(renderer,nullptr);
    SDL_SetRenderDrawColor(renderer,48,48,48,255);
    SDL_RenderClear(renderer);
    for(int row=0; row<6; ++row) {
        const unsigned base=row==5 ? ObjPic_RebelHarvester
            : row>=3 ? ObjPic_Siegetank_Base : ObjPic_Tank_Base;
        for(unsigned zoom=0; zoom<NUM_ZOOMLEVEL; ++zoom) {
            for(int heading=0; heading<4; ++heading) {
                const int angle=heading*2, x=24+zoom*224+heading*52, y=40+row*72;
                auto* body=pGFXManager->getZoomedObjPic(base,HOUSE_HARKONNEN,zoom);
                auto src=calcSpriteSourceRect(body,angle,NUM_ANGLES);
                auto dst=calcSpriteDrawingRect(body,x,y,NUM_ANGLES,1,HAlign::Center,VAlign::Center);
                require(SDL_RenderCopy(renderer,body,&src,&dst)==0,"body render");
                auto* turret=pGFXManager->getZoomedObjPic(turrets[row],HOUSE_HARKONNEN,zoom);
                src=calcSpriteSourceRect(turret,angle,NUM_ANGLES);
                const Coord offsets[] = {Coord(8,-12),Coord(0,-20),Coord(-8,-12),Coord(-4,-12)};
                const Coord offset=row>=3 && row<5 ? offsets[heading]
                    : row==5 ? Coord(0,0) : Coord(0,angle==0 || angle==4 ? -12 : -8);
                dst=calcSpriteDrawingRect(turret,x+offset.x*static_cast<int>(zoom+1)/4,
                    y+offset.y*static_cast<int>(zoom+1)/4,NUM_ANGLES,1,HAlign::Center,VAlign::Center);
                require(SDL_RenderCopy(renderer,turret,&src,&dst)==0,"turret render");
            }
        }
    }
    auto pixels=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,680,432,32,SDL_PIXELFORMAT_ARGB8888));
    SDL_Rect area{0,0,680,432};
    require(pixels && SDL_RenderReadPixels(renderer,&area,pixels->format->format,
            pixels->pixels,pixels->pitch)==0,"contact sheet capture");
    require(SDL_SaveBMP(pixels.get(),(std::filesystem::path(output)/(mod+"-turrets-1x-2x-3x.bmp")).string().c_str())==0,
            "contact sheet output");
    SDL_Log("CUSTOM ZOOM PASS: %s, vehicle bases and turrets at 1x/2x/3x for all house slots",mod.c_str());
}
