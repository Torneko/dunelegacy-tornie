#pragma once
#include <structures/WindTrap.h>
#include <MapEditor/MapEditor.h>
#include <MapEditor/MapEditorInterface.h>
#include <fstream>
#include "ChaosDefenceUpgradeSmoke.h"

struct EditorWindtrapFixture {
    static void verify(const std::string& output,const std::string& mod) {
        const auto require=[](bool ok,const char* why) {
            if(!ok)throw std::runtime_error(std::string("Editor Windtrap: ")+why);
        };
        const bool full=mod!="vanilla" && !isLiteMod(mod);
        const auto house=getRuntimeHouseForIdentity(full?HOUSE_REBELS:HOUSE_HARKONNEN);
        const auto path=(std::filesystem::path(output)/(mod+"-editor-power.ini")).string();
        std::string map;
        {
            MapEditor editor;MapInfo info;info.techLevel=2;
            editor.setMap(MapData(32,32,Terrain_Rock),info);
            for(auto& player:editor.getPlayers()) {
                player.bActive=player.house==house;player.bAnyHouse=false;
                player.credits=1234;
            }
            MapEditorStructurePlaceOperation(Coord(4,4),house,Structure_WindTrap,256).perform(&editor);
            MapEditorStructurePlaceOperation(Coord(8,4),house,Structure_WindTrap,256).perform(&editor);
            const auto id=editor.structures.front().id;
            MapEditorInterface ui(&editor);editor.selectedStructureID=id;ui.onObjectSelected();
            require(ui.structureDetailsPowerUpgradeCheckbox.isVisible()==full,"checkbox faction/Vanilla visibility");
            if(full) {
                ui.structureDetailsPowerUpgradeCheckbox.handleMouseLeft(5,5,true);
                ui.structureDetailsPowerUpgradeCheckbox.handleMouseLeft(5,5,false);
                require(editor.getStructure(id)->powerUpgraded,"checkbox does not upgrade selected Windtrap");
                editor.undoLastOperation();require(!editor.getStructure(id)->powerUpgraded,"upgrade undo");
                editor.redoLastOperation();require(editor.getStructure(id)->powerUpgraded,"upgrade redo");
                editor.startOperation();
                editor.addUndoOperation(MapEditorRemoveStructureOperation(id).perform(&editor));
                editor.undoLastOperation();require(editor.getStructure(id)->powerUpgraded,"delete/undo lost upgrade");
                auto undoHealth=MapEditorEditStructureOperation(id,128).perform(&editor);
                require(editor.getStructure(id)->powerUpgraded,"health edit cleared power upgrade");
                undoHealth->perform(&editor);require(editor.getStructure(id)->health==256,"health undo");
            } else {
                // A manually forged flag must not enable the feature in Vanilla/Lite.
                editor.getStructure(id)->powerUpgraded=true;
            }
            editor.saveMap(path);
            std::ifstream input(path);map.assign(std::istreambuf_iterator<char>(input),{});
            require(map.find("WINDTRAPUPGRADES")!=std::string::npos,"map has no persistent upgrade property");
            editor.loadMap(path);
            require(editor.getStructure(id)->powerUpgraded==full,"map roundtrip or unsupported flag policy");
            if(mod!="vanilla") {
                MapEditorStructurePlaceOperation(Coord(2,2),house,Structure_Slowpost,256).perform(&editor);
                const int oldZoom=currentZoomlevel;
                for(int zoom=0;zoom<3;++zoom) {
                    currentZoomlevel=zoom;const int tile=world2zoomedWorld(TILESIZE);
                    SDL_Rect area{0,0,12*tile,8*tile};ScreenBorder view(area);
                    view.adjustScreenBorderToMapsize(32,32);view.setNewScreenCenter(Coord(6*TILESIZE,4*TILESIZE));
                    SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);
                    editor.drawMap(&view,true);
                    auto scene=renderReadSurface(renderer);
                    require(scene!=nullptr,"placed structure capture");
                    require(SavePNG(scene.get(),(std::filesystem::path(output)/(mod+"-editor-post-power-"+std::to_string(zoom+1)+".png")).string().c_str())==0,"placed structure preview");
                    const auto compare=[&](SDL_Texture* atlas,SDL_Rect source,Coord pos) {
                        const int w=source.w,h=source.h;
                        SDL_RenderClear(renderer);SDL_Rect dst{0,0,w,h};SDL_RenderCopy(renderer,atlas,&source,&dst);
                        auto expected=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32)};
                        require(SDL_RenderReadPixels(renderer,&dst,expected->format->format,expected->pixels,expected->pitch)==0,"expected structure capture");
                        const int sx=view.world2screenX(pos.x*TILESIZE),sy=view.world2screenY(pos.y*TILESIZE);
                        for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
                            Uint8 r,g,b,a;SDL_GetRGBA(getPixel(expected.get(),x,y),expected->format,&r,&g,&b,&a);
                            if(r==7 && g==11 && b==13)continue;
                            Uint8 ar,ag,ab,aa;SDL_GetRGBA(getPixel(scene.get(),sx+x,sy+y),scene->format,&ar,&ag,&ab,&aa);
                            require(r==ar && g==ag && b==ab,"structure pixels differ from clean atlas or upgraded animation");
                        }
                    };
                    compare(pGFXManager->getZoomedObjPic(ObjPic_Slowpost,house,zoom),SDL_Rect{2*tile,0,tile,tile},Coord(2,2));
                    if(full)compare(pGFXManager->getObjPic(ObjPic_Windtrap,house,true)[zoom],SDL_Rect{18*tile,0,2*tile,2*tile},Coord(4,4));
                    if(zoom==0) {
                        SDL_RenderClear(renderer);
                        auto clean=pGFXManager->getZoomedObjPic(ObjPic_Slowpost,house,0);SDL_Rect src{32,0,16,16},dst{0,0,16,16};
                        SDL_RenderCopy(renderer,clean,&src,&dst);auto expected=renderReadSurface(renderer);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer,pGFXManager->getUIGraphic(UI_MapEditor_Slowpost,house),nullptr,&dst);auto actual=renderReadSurface(renderer);
                        for(int y=0;y<16;++y)for(int x=0;x<16;++x)
                            require(getPixel(expected.get(),x,y)==getPixel(actual.get(),x,y),"Slowpost palette contains a marker");
                    }
                }
                currentZoomlevel=oldZoom;
            }
            if(full) {
                editor.getStructure(id)->powerUpgraded=false;editor.saveMap(path);
                std::ifstream plain(path);const std::string normal((std::istreambuf_iterator<char>(plain)),{});
                require(normal.find("WINDTRAPUPGRADES")==std::string::npos,"unchecked flag left stale map metadata");
            }
        }
        {
            GameInitSettings init("editor-power",map,false,effectiveGameOptions);
            GameInitSettings::HouseInfo info(house,1);
            info.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));init.addHouseInfo(info);
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            auto* owner=game->getHouse(house);require(owner && owner->getCredits()==1234,"initial upgrade charged credits");
            int upgraded=0;WindTrap* first=nullptr;
            for(auto* building:structureList)if(auto* trap=dynamic_cast<WindTrap*>(building)) {
                if(!first)first=trap;
                if(trap->isPowerUpgraded())++upgraded;
            }
            require(first && upgraded==(full?1:0),"game start ignored or forged the map upgrade");
            const int power=std::abs(game->objectData.data[Structure_WindTrap][owner->getFactionID()].power);
            require(owner->getProducedPower()==power*(full?3:2),"wrong initial house power");
            require(game->techLevel==2 && owner->getNumItems(Structure_IX)==0,"fixture must start below ordinary upgrade requirements");
            const auto save=(std::filesystem::path(output)/(mod+"-editor-power.dls")).string();
            require(game->saveGame(save),"save pre-upgraded game");
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
            game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(save));
            upgraded=0;for(auto* building:structureList)if(auto* trap=dynamic_cast<WindTrap*>(building))upgraded+=trap->isPowerUpgraded();
            require(upgraded==(full?1:0),"save reload lost initial power upgrade");
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        }
        verifyChaosDefenceUpgrades(output,mod);
        SDL_Log("EDITOR WINDTRAP PASS: %s, checkbox, map persistence, undo, initial power/credits and saved upgrade",mod.c_str());
    }
};
