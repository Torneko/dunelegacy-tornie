#pragma once
#include <MapEditor/MapEditor.h>

// Render placed units through the real editor path, separately from its palette.
struct Release543EditorFixture {
    static void verify(const std::string& output,const std::string& mod) {
        const auto require=[](bool ok,const char* message) {
            if(!ok)throw std::runtime_error(std::string("Release 543 editor: ")+message);
        };
        MapEditor editor;editor.setMap(MapData(14,12,Terrain_Rock),MapInfo());
        const int previousZoom=currentZoomlevel;
        const int items[]={Unit_FlameQuad,Unit_GasInfantry,Unit_FlameTrooper,Unit_EliteOrnithopter,Unit_RaiderQuad,Unit_RangedTank,Unit_SlowTank,Unit_CapacityHarvester,Unit_FastHarvester,Unit_TameSandworm};
        const int graphics[]={ObjPic_FlameQuad,ObjPic_GasInfantry,ObjPic_Trooper,ObjPic_EliteOrnithopter,ObjPic_RaiderQuad,ObjPic_RangedTankBase,ObjPic_SlowTankBase,ObjPic_CapacityHarvester,ObjPic_FastHarvester,ObjPic_TameSandworm};
        for(int i=0;i<10;++i)
            editor.units.emplace_back(100+i,HOUSE_HARKONNEN,items[i],256,Coord(2+3*(i%4),2+3*(i/4)),0,GUARD);
        for(int zoom=0;zoom<3;++zoom) {
            currentZoomlevel=zoom;
            const int tile=world2zoomedWorld(TILESIZE);
            SDL_Rect area{0,0,14*tile,12*tile};
            ScreenBorder view(area);view.adjustScreenBorderToMapsize(14,12);
            view.setNewScreenCenter(Coord(7*TILESIZE,6*TILESIZE));
            SDL_RenderSetClipRect(renderer,nullptr);
            SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);
            editor.drawMap(&view,true);
            auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,area.w,area.h,32,SDL_PIXELFORMAT_RGBA32)};
            require(SDL_RenderReadPixels(renderer,&area,image->format->format,image->pixels,image->pitch)==0,"terrain readback");
            for(int i=0;i<10;++i) {
                auto* sprite=pGFXManager->getZoomedObjPic(graphics[i],HOUSE_HARKONNEN,zoom);
                int width,height;SDL_QueryTexture(sprite,nullptr,nullptr,&width,&height);
                const int framesX=i==9?1:(i==1 || i==2)?4:8,framesY=i==9?9:(i>=1 && i<=3)?3:1;
                const int right=view.world2screenX((2+3*(i%4))*TILESIZE+TILESIZE/2)+width/framesX/2-1;
                const int bottom=view.world2screenY((2+3*(i/4))*TILESIZE+TILESIZE/2)+height/framesY/2-1;
                int blue=0;
                for(int y=std::max(0,bottom-12);y<=bottom && y<area.h;++y)
                    for(int x=std::max(0,right-12);x<=right && x<area.w;++x) {
                        Uint8 r,g,b,a;SDL_GetRGBA(getPixel(image.get(),x,y),image->format,&r,&g,&b,&a);
                        if((r==0 && g==0 && b==170)||(r==85 && g==255 && b==255))++blue;
                    }
                require(blue>0,"new unit has no blue star on the editor terrain");
            }
            const auto file=(std::filesystem::path(output)/(mod+"-editor-terrain-zoom"+std::to_string(zoom+1)+".png")).string();
            require(SavePNG(image.get(),file.c_str())==0,"terrain preview export");
        }
        currentZoomlevel=previousZoom;
        SDL_Log("RELEASE 543 EDITOR TERRAIN PASS: %s, Flame Quad, Gas Infantry and Flame Trooper blue stars at 1x/2x/3x",mod.c_str());
    }
};
