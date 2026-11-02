#pragma once
#include <units/SuperTank.h>
#include <MapEditor/MapEditor.h>
#include <FileClasses/LoadSavePNG.h>
#include <fstream>

struct SuperTankAlignmentFixture {
    static void require(bool value,const std::string& reason) {
        if(!value)throw std::runtime_error("Super Tank alignment: "+reason);
    }
    struct CameraOverride {
        ScreenBorder* previous;
        explicit CameraOverride(ScreenBorder* value):previous(screenborder) {screenborder=value;}
        ~CameraOverride() {screenborder=previous;}
    };
    static void clear() {
        SDL_RenderSetClipRect(renderer,nullptr);
        SDL_SetRenderDrawColor(renderer,17,21,26,255);SDL_RenderClear(renderer);
    }
    static sdl2::surface_ptr capture() {
        auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,64,64,32,SDL_PIXELFORMAT_RGBA32)};
        SDL_Rect area{16,16,64,64};
        require(image && SDL_RenderReadPixels(renderer,&area,image->format->format,image->pixels,image->pitch)==0,"frame capture");
        return image;
    }
    static sdl2::surface_ptr sheet() {
        auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,1024,384,32,SDL_PIXELFORMAT_RGBA32)};
        require(image!=nullptr,"contact sheet allocation");
        SDL_FillRect(image.get(),nullptr,SDL_MapRGBA(image->format,17,21,26,255));return image;
    }
    static void addFrame(SDL_Surface* target,SDL_Surface* frame,int angle,int zoom) {
        SDL_Rect destination{angle*128,zoom*128,128,128};
        require(SDL_BlitScaled(frame,nullptr,target,&destination)==0,"contact sheet frame");
    }
    static void save(SDL_Surface* image,const std::filesystem::path& path) {
        require(SavePNG(image,path.string().c_str())==0,"PNG export "+path.filename().string());
    }
    static void run(const char* directory) {
        require(directory,"missing output directory");const std::filesystem::path output(directory);
        std::filesystem::create_directories(output);
        const auto previousMod=ModManager::instance().getActiveModName();const int previousZoom=currentZoomlevel;
        std::ofstream csv(output/"super-tank-alignment.csv");
        csv<<"mod,zoom,angle,game_editor_mismatches,palette_mismatches\n";
        int gameEditorMismatches=0,paletteMismatches=0,frames=0;
        for(const std::string mod:{"Tornie","Jericho","ArcaKev","TornieLite","JerichoLite","ArcaKevLite"}) {
            require(ModManager::instance().setActiveMod(mod),"mod activation "+mod);
            effectiveGameOptions=ModManager::instance().loadEffectiveGameOptions(settings.gameOptions);
            const auto house=isLiteMod(mod)?static_cast<HOUSETYPE>(getFactionSelectionOrder().front()):getRuntimeHouseForIdentity(HOUSE_KLESHMERSH);
            auto options=effectiveGameOptions;options.chaosMode=false;
            GameInitSettings init("super-tank-alignment",chaosSmokeMap(),false,options);
            GameInitSettings::HouseInfo info(house,1);
            info.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));init.addHouseInfo(info);
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            auto* tank=dynamic_cast<SuperTank*>(pLocalHouse->placeUnit(Unit_SuperTank,10,10,true));
            require(tank && tank->getMaxHealth()==300 && game->objectData.data[Unit_SuperTank][house].price==925,"unit identity/stats changed");
            auto gameSheet=sheet(),editorSheet=sheet(),turretSweepSheet=sheet();
            {
                // The editor owns a temporary global camera of its own.
                CameraOverride restoreGameCamera(screenborder);
                MapEditor editor;MapInfo mapInfo;mapInfo.techLevel=9;
                editor.setMap(MapData(32,32,Terrain_Rock),mapInfo);
                for(auto& player:editor.getPlayers()){player.bActive=player.house==house;player.bAnyHouse=false;}
                MapEditor::Unit placed(0,house,Unit_SuperTank,256,Coord(10,10),0,AREAGUARD);
                for(int zoom=0;zoom<3;++zoom) {
                    currentZoomlevel=zoom;
                    // Divisible by all three zoom ratios, keeping integer and
                    // fixed-point camera conversions on the same pixel grid.
                    SDL_Rect area{0,0,96,96};ScreenBorder view(area);
                    view.adjustScreenBorderToMapsize(32,32);
                    view.setNewScreenCenter(Coord(lround(tank->getRealX()),lround(tank->getRealY())));
                    CameraOverride camera(&view);
                    for(int angle=0;angle<8;++angle) {
                        tank->setAngle(angle);tank->setTurretAngle(angle);
                        clear();tank->blitToScreen();auto runtime=capture();
                        addFrame(gameSheet.get(),runtime.get(),angle,zoom);
                        tank->setAngle(0);
                        clear();tank->blitToScreen();auto sweep=capture();
                        addFrame(turretSweepSheet.get(),sweep.get(),angle,zoom);
                        tank->setAngle(angle);
                        editor.getUnitList().clear();
                        clear();editor.drawMap(&view,true);tank->blitToScreen();auto terrainRuntime=capture();
                        placed.angle=static_cast<unsigned char>(angle);editor.getUnitList().push_back(placed);
                        clear();editor.drawMap(&view,true);auto edited=capture();
                        addFrame(editorSheet.get(),edited.get(),angle,zoom);
                        const int radius=zoom==0?2:zoom==1?3:5;
                        const int markerRight=view.world2screenX(10*TILESIZE+TILESIZE/2)-16+8*(zoom+1)-1;
                        const int markerBottom=view.world2screenY(10*TILESIZE+TILESIZE/2)-16+8*(zoom+1)-1;
                        int difference=0,iconDifference=-1;
                        for(int y=0;y<64;++y)for(int x=0;x<64;++x) {
                            if(x>=markerRight-2*radius && x<=markerRight && y>=markerBottom-2*radius && y<=markerBottom)continue;
                            if(getPixel(terrainRuntime.get(),x,y)!=getPixel(edited.get(),x,y))++difference;
                        }
                        if(zoom==0 && angle==0) {
                            auto* icon=pGFXManager->getUIGraphicSurface(UI_MapEditor_SuperTank,house);
                            require(icon && icon->w==16 && icon->h==16,"native editor palette dimensions");
                            auto texture=sdl2::texture_ptr{SDL_CreateTextureFromSurface(renderer,icon)};
                            require(texture!=nullptr,"palette texture");
                            SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_BLEND);
                            clear();SDL_Rect dest{40,40,16,16};SDL_RenderCopy(renderer,texture.get(),nullptr,&dest);auto palette=capture();
                            save(palette.get(),output/(mod+"-super-tank-alignment-palette.png"));
                            iconDifference=0;
                            for(int y=0;y<16;++y)for(int x=0;x<16;++x) {
                                if(x>=9 && y>=9)continue; // editor-only 7x7 custom-unit star
                                if(getPixel(runtime.get(),24+x,24+y)!=getPixel(palette.get(),24+x,24+y))++iconDifference;
                            }
                            paletteMismatches+=iconDifference;
                        }
                        csv<<mod<<','<<zoom+1<<','<<angle<<','<<difference<<','<<iconDifference<<'\n';csv.flush();
                        gameEditorMismatches+=difference;++frames;
                    }
                }
            }
            save(gameSheet.get(),output/(mod+"-super-tank-alignment-game.png"));
            save(editorSheet.get(),output/(mod+"-super-tank-alignment-editor.png"));
            save(turretSweepSheet.get(),output/(mod+"-super-tank-alignment-turret-sweep.png"));
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        }
        currentZoomlevel=previousZoom;ModManager::instance().setActiveMod(previousMod);
        require(gameEditorMismatches==0,"game/editor pixels differ: "+std::to_string(gameEditorMismatches));
        require(paletteMismatches==0,"angle-zero palette/runtime pixels differ: "+std::to_string(paletteMismatches));
        SDL_Log("SUPER TANK ALIGNMENT PASS: %d game/editor poses, 8 directions, 3 zooms, six custom modes; native palette matched; artwork/stats preserved",frames);
    }
};
