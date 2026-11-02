#pragma once
#include <units/SuperTank.h>
#include <MapEditor/MapEditor.h>
#include <FileClasses/FileManager.h>
#include <FileClasses/LoadSavePNG.h>
#include <FileClasses/Shpfile.h>
#include <array>
#include <cstdint>
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
    static int pixelDifferences(SDL_Surface* first,SDL_Surface* second) {
        require(first && second && first->w==second->w && first->h==second->h,"comparison dimensions");
        int result=0;
        for(int y=0;y<first->h;++y)for(int x=0;x<first->w;++x)
            if(getPixel(first,x,y)!=getPixel(second,x,y))++result;
        return result;
    }
    static std::uint64_t fingerprint(SDL_Surface* image) {
        std::uint64_t result=14695981039346656037ULL;
        for(int y=0;y<image->h;++y)for(int x=0;x<image->w;++x) {
            const Uint32 pixel=getPixel(image,x,y);
            for(int byte=0;byte<4;++byte){result^=(pixel>>(byte*8))&255U;result*=1099511628211ULL;}
        }
        return result;
    }
    static sdl2::surface_ptr spriteFrame(SDL_Texture* texture,int angle,int zoom,bool nativeReference=false) {
        require(texture!=nullptr,"sprite reference texture");
        int width=0,height=0;require(SDL_QueryTexture(texture,nullptr,nullptr,&width,&height)==0,"sprite dimensions query");
        const int sourceScale=nativeReference?1:zoom+1,size=16*(zoom+1);
        require(width==128*sourceScale && height==16*sourceScale,"eight native sprite frames");
        SDL_Rect source{angle*16*sourceScale,0,16*sourceScale,16*sourceScale};
        SDL_Rect destination{48-size/2,48-size/2,size,size};
        clear();require(SDL_RenderCopy(renderer,texture,&source,&destination)==0,"sprite reference draw");
        return capture();
    }
    static sdl2::texture_ptr referenceTexture(SDL_Surface* image) {
        auto texture=sdl2::texture_ptr{SDL_CreateTextureFromSurface(renderer,image)};
        require(texture!=nullptr,"independent reference texture");
        SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture.get(),SDL_ScaleModeNearest);
        return texture;
    }
    struct LayerReference {
        sdl2::surface_ptr ordinarySiege,superGun;
        std::array<int,8> visibleOrange{};
    };
    static LayerReference independentLayers() {
        // Decode the original SHP again rather than reading GFXManager's cache.
        // This catches recolouring the shared Siege sheet in place.
        auto packed=pFileManager->openFile("UNITS2.SHP");Shpfile source(packed.get());
        auto stock=source.getPictureArray(8,1,GROUNDUNIT_ROW(15));
        require(stock && stock->w==128 && stock->h==16 && stock->format->BytesPerPixel==1,"original Siege SHP atlas");
        SDL_SetColorKey(stock.get(),SDL_TRUE,0);
        auto upper=LoadPNG_RW(pFileManager->openFile("SuperTankGun.png").get());
        require(upper && upper->w==128 && upper->h==16,"authored upper turret PNG");
        auto upperRgba=sdl2::surface_ptr{SDL_ConvertSurfaceFormat(upper.get(),SDL_PIXELFORMAT_RGBA32,0)};
        LayerReference result;
        result.ordinarySiege.reset(SDL_ConvertSurfaceFormat(stock.get(),SDL_PIXELFORMAT_RGBA32,0));
        result.superGun.reset(SDL_CreateRGBSurfaceWithFormat(0,128,16,32,SDL_PIXELFORMAT_RGBA32));
        require(upperRgba && result.ordinarySiege && result.superGun,"independent layer allocation");
        SDL_FillRect(result.superGun.get(),nullptr,0);
        // Fixed visual specification, deliberately independent of the production
        // anchor table and tint helper: campaign-orange shades 0,1,2,3,5.
        const Coord underlayOffsets[]={{-1,-1},{0,-1},{1,0},{0,-1},{1,-1},{0,0},{0,-2},{0,0}};
        const SDL_Color orange[]={{255,168,48,255},{240,144,32,255},{220,120,20,255},
                                  {196,96,12,255},{132,52,4,255}};
        for(int angle=0;angle<8;++angle)for(int y=0;y<16;++y)for(int x=0;x<16;++x) {
            Uint32 expected=0;bool orangeUnderlay=false;
            const int sx=x-underlayOffsets[angle].x,sy=y-underlayOffsets[angle].y;
            if(sx>=0 && sx<16 && sy>=0 && sy<16) {
                const auto index=getPixel(stock.get(),angle*16+sx,sy);
                if(index!=0) {
                    if(index>=128 && index<=132) {
                        const auto color=orange[index-128];orangeUnderlay=true;
                        expected=SDL_MapRGBA(result.superGun->format,color.r,color.g,color.b,255);
                    } else {
                        require(index==12,"unexpected original Siege metal/outline index");
                        expected=SDL_MapRGBA(result.superGun->format,0,0,0,255);
                    }
                }
            }
            Uint8 r,g,b,a;SDL_GetRGBA(getPixel(upperRgba.get(),angle*16+x,y),upperRgba->format,&r,&g,&b,&a);
            require(a==0 || a==255,"authored upper turret alpha changed");
            if(a)expected=SDL_MapRGBA(result.superGun->format,r,g,b,a);
            else if(orangeUnderlay)++result.visibleOrange[angle];
            putPixel(result.superGun.get(),angle*16+x,y,expected);
        }
        for(int count:result.visibleOrange)require(count>0,"orange underlay completely hidden in one direction");
        SDL_SetSurfaceBlendMode(result.superGun.get(),SDL_BLENDMODE_BLEND);
        return result;
    }
    static void run(const char* directory) {
        require(directory,"missing output directory");const std::filesystem::path output(directory);
        std::filesystem::create_directories(output);
        const auto previousMod=ModManager::instance().getActiveModName();const int previousZoom=currentZoomlevel;
        std::ofstream csv(output/"super-tank-alignment.csv");
        csv<<"mod,zoom,angle,game_editor_mismatches,palette_mismatches,super_layers_mismatches,visible_orange_pixels,ordinary_siege_source_mismatches,ordinary_siege_before_hash,ordinary_siege_after_hash\n";
        int gameEditorMismatches=0,paletteMismatches=0,layerMismatches=0,ordinarySiegeMismatches=0,frames=0;
        for(const std::string mod:{"Tornie","Jericho","ArcaKev","TornieLite","JerichoLite","ArcaKevLite"}) {
            require(ModManager::instance().setActiveMod(mod),"mod activation "+mod);
            effectiveGameOptions=ModManager::instance().loadEffectiveGameOptions(settings.gameOptions);
            const auto house=isLiteMod(mod)?static_cast<HOUSETYPE>(getFactionSelectionOrder().front()):getRuntimeHouseForIdentity(HOUSE_KLESHMERSH);
            auto options=effectiveGameOptions;options.chaosMode=false;
            GameInitSettings init("super-tank-alignment",chaosSmokeMap(),false,options);
            GameInitSettings::HouseInfo info(house,1);
            info.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));init.addHouseInfo(info);
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            auto layers=independentLayers();
            auto expectedSuper=referenceTexture(layers.superGun.get()),expectedStock=referenceTexture(layers.ordinarySiege.get());
            std::array<sdl2::surface_ptr,24> stockBefore;
            for(int zoom=0;zoom<3;++zoom)for(int angle=0;angle<8;++angle)
                stockBefore[zoom*8+angle]=spriteFrame(pGFXManager->getZoomedObjPic(ObjPic_Siegetank_Gun,house,zoom),angle,zoom);
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
                        auto actualLayers=spriteFrame(pGFXManager->getZoomedObjPic(ObjPic_SuperTankGun,house,zoom),angle,zoom);
                        auto expectedLayers=spriteFrame(expectedSuper.get(),angle,zoom,true);
                        const int layerDifference=pixelDifferences(actualLayers.get(),expectedLayers.get());
                        layerMismatches+=layerDifference;
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
                        auto ordinaryAfter=spriteFrame(pGFXManager->getZoomedObjPic(ObjPic_Siegetank_Gun,house,zoom),angle,zoom);
                        const auto beforeHash=fingerprint(stockBefore[zoom*8+angle].get()),afterHash=fingerprint(ordinaryAfter.get());
                        int stockDifference=pixelDifferences(stockBefore[zoom*8+angle].get(),ordinaryAfter.get());
                        if(zoom==0) {
                            auto original=spriteFrame(expectedStock.get(),angle,0,true);
                            stockDifference+=pixelDifferences(ordinaryAfter.get(),original.get());
                        }
                        ordinarySiegeMismatches+=stockDifference;
                        csv<<mod<<','<<zoom+1<<','<<angle<<','<<difference<<','<<iconDifference<<','<<layerDifference<<','
                           <<layers.visibleOrange[angle]*(zoom+1)*(zoom+1)<<','<<stockDifference<<','<<beforeHash<<','<<afterHash<<'\n';csv.flush();
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
        require(layerMismatches==0,"independent orange Siege/upper PNG composition differs: "+std::to_string(layerMismatches));
        require(ordinarySiegeMismatches==0,"ordinary Siege pixels/colours changed: "+std::to_string(ordinarySiegeMismatches));
        SDL_Log("SUPER TANK ALIGNMENT PASS: %d game/editor poses, 8 directions, 3 zooms, six custom modes; independent orange Siege underlay and upper PNG matched; ordinary Siege unchanged; native palette matched; artwork/stats preserved",frames);
    }
};
