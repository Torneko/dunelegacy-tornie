#pragma once
// SPDX-License-Identifier: GPL-2.0-or-later
#include <misc/Scaler.h>
#include <cstdlib>

inline void verifyRelease544Artwork(const std::string& output, const std::string& mod) {
    const auto require=[](bool ok,const char* why) {
        if(!ok){SDL_Log("RELEASE 544 ARTWORK FAIL: %s",why);std::exit(EXIT_FAILURE);}
    };
    struct Atlas { int id,width,height; };
    for(const auto& atlas:{Atlas{ObjPic_RaiderQuad,128,16},{ObjPic_Slowpost,160,16},
                          {ObjPic_CapacityHarvester,192,24},{ObjPic_FastHarvester,192,24}}) {
        for(const int house:{HOUSE_HARKONNEN,HOUSE_ATREIDES,HOUSE_ORDOS}) {
            SDL_Surface* base=nullptr;
            for(int zoom=0;zoom<3;++zoom) {
                auto* texture=pGFXManager->getZoomedObjPic(atlas.id,house,zoom);
                int w=0,h=0;require(texture && SDL_QueryTexture(texture,nullptr,nullptr,&w,&h)==0,"missing atlas");
                require(w==atlas.width*(zoom+1) && h==atlas.height*(zoom+1),"incorrect native atlas or zoom dimensions");
                SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);
                const SDL_Rect rect{0,0,w,h};SDL_RenderCopy(renderer,texture,nullptr,&rect);
                auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32)};
                require(SDL_RenderReadPixels(renderer,&rect,image->format->format,image->pixels,image->pitch)==0,"atlas readback");
                int team=0;
                std::array<int,8> fixedHalf{},houseHalf{};
                for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
                    Uint8 r,g,b,a;SDL_GetRGBA(getPixel(image.get(),x,y),image->format,&r,&g,&b,&a);
                    require(!(r==255 && g==0 && b==255),"opaque magenta in new artwork");
                    for(int shade=0;shade<7;++shade){const auto c=getHouseColorSDL(house,shade);if(r==c.r&&g==c.g&&b==c.b){++team;break;}}
                    if(atlas.id==ObjPic_CapacityHarvester || atlas.id==ObjPic_FastHarvester) {
                        const int frame=x/(24*(zoom+1));
                        for(int shade=0;shade<7;++shade) {
                            const auto c=getHouseColorSDL(house,shade);
                            if(r==c.r && g==c.g && b==c.b) {++houseHalf[frame];break;}
                        }
                        if(atlas.id==ObjPic_CapacityHarvester ? (r>b && b>g && g>0) : (r==0 && b>g && g>0))
                            ++fixedHalf[frame];
                    }
                    if(base)require(getPixel(image.get(),x,y)==getPixel(base,x/(zoom+1),y/(zoom+1)),"non-integer sprite zoom");
                }
                require(team>0,"new atlas lacks house tint");
                if(atlas.id==ObjPic_CapacityHarvester || atlas.id==ObjPic_FastHarvester)
                    for(int frame=0;frame<8;++frame)
                        require(fixedHalf[frame]>0 && houseHalf[frame]>0,"Harvester direction lost a fixed or house-colour half");
                if(zoom==0) {
                    base=image.release();
                    require(SavePNG(base,(std::filesystem::path(output)/(mod+"-art544-atlas-"+std::to_string(atlas.id)+"-"+std::to_string(house)+".png")).string().c_str())==0,"atlas export");
                }
            }
            SDL_FreeSurface(base);
        }
    }
    struct Portrait {int item;const char* file;};
    for(const auto& portrait:{Portrait{Unit_RaiderQuad,"RaiderQuadIcon.png"},{Unit_RangedTank,"RangedCombatTankIcon.png"},
        {Unit_SlowTank,"SlowTankIcon.png"},{Unit_CapacityHarvester,"CapacityHarvesterIcon.png"},
        {Unit_FastHarvester,"SpeedHarvesterIcon.png"},{Structure_Slowpost,"SlowpostIcon.png"}}) {
        auto raw=LoadPNG_RW(pFileManager->openFile(portrait.file).get());require(raw!=nullptr,"missing original portrait");
        if(raw->w!=91 || raw->h!=55) {
            auto native=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,91,55,32,SDL_PIXELFORMAT_RGBA32)};
            SDL_SetSurfaceBlendMode(raw.get(),SDL_BLENDMODE_NONE);
            require(SDL_BlitScaled(raw.get(),nullptr,native.get(),nullptr)==0,"native portrait sizing");
            raw=std::move(native);
        }
        auto reference=convertSurfaceToTexture(raw.get());
        const auto capture=[&](SDL_Texture* texture) {
            require(texture!=nullptr,"missing production/selection portrait");
            SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);
            SDL_Rect rect{0,0,91,55};SDL_RenderCopy(renderer,texture,nullptr,&rect);
            auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,91,55,32,SDL_PIXELFORMAT_RGBA32)};
            require(SDL_RenderReadPixels(renderer,&rect,image->format->format,image->pixels,image->pitch)==0,"portrait readback");
            return image;
        };
        auto expected=capture(reference.get());auto actual=capture(resolveItemPicture(portrait.item,HOUSE_HARKONNEN));
        for(int y=0;y<55;++y)for(int x=0;x<91;++x)
            require(getPixel(actual.get(),x,y)==getPixel(expected.get(),x,y),"portrait differs from original or contains an added marker");
        require(SavePNG(actual.get(),(std::filesystem::path(output)/(mod+"-art544-portrait-"+std::to_string(portrait.item)+".png")).string().c_str())==0,"portrait export");
    }
    SDL_Log("RELEASE 544 ARTWORK PASS: %s, four native atlases, three house colours, exact zooms and six original clean portraits",mod.c_str());
}
