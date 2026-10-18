#pragma once
#include <units/GasInfantry.h>
#include <SpecialVehicle.h>

inline void verifyRelease543Units(const std::string& output,const std::string& mod) {
    auto require=[](bool ok,const char* message){if(!ok)throw std::runtime_error(std::string("Release 543 units: ")+message);};
    auto options=effectiveGameOptions;options.chaosMode=false;
    auto game=std::make_unique<Game>();currentGame=game.get();
    game->initGame(chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_HARKONNEN),options));
    auto* owner=pLocalHouse;owner->createUnit(Unit_MCV,true);
    auto* wor=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_WOR,2,2,true,true));
    const auto available=[](BuilderBase* builder,int item){builder->updateBuildList();return std::any_of(builder->getBuildList().begin(),builder->getBuildList().end(),[&](const BuildItem& b){return b.itemID==item;});};
    game->techLevel=7;require(!available(wor,Unit_FlameTrooper),"flame infantry available without IX");
    owner->placeStructure(NONE_ID,Structure_IX,8,2,true,true);
    game->techLevel=6;require(!available(wor,Unit_FlameTrooper),"flame infantry available before level seven");
    game->techLevel=7;require(available(wor,Unit_FlameTrooper)==(mod!="vanilla"),"Harkonnen flame infantry unlock");
    require(available(wor,Unit_Trooper),"ordinary Harkonnen Trooper was removed");
    for(int house=0;house<NUM_HOUSES;++house) {
        const auto pool=discoverHouseSpecialVehicleCandidates([&](int item) {
            const auto& data=game->objectData.data[item][house];
            return HouseSpecialVehicleCandidateData{data.enabled,data.builder,data.prerequisiteStructuresSet[Structure_IX]};
        });
        require(std::find(pool.begin(),pool.end(),Unit_FlameTrooper)==pool.end(),"flame infantry in Special spawn pool");
        require(std::find(pool.begin(),pool.end(),Unit_GasInfantry)==pool.end(),"gas infantry in Special spawn pool");
    }
    game->gameType=GameType::CustomMultiplayer;
    auto* harkCombo=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_Worfinery,2,8,true,true));
    require(harkCombo && !available(harkCombo,Unit_Saboteur),"non-Neutral house can produce Saboteurs");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    if(mod=="vanilla")return;
    if(!isLiteMod(mod)) {
        game=std::make_unique<Game>();currentGame=game.get();
        game->initGame(chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_NEUTRAL),options));owner=pLocalHouse;owner->createUnit(Unit_MCV,true);
        auto* combo=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_Worfinery,2,2,true,true));
        require(combo,"Neutral Worfinery placement");game->techLevel=7;
        require(!available(combo,Unit_Saboteur),"Neutral Saboteur available without IX in single player");
        game->gameType=GameType::CustomMultiplayer;
        require(!available(combo,Unit_Saboteur),"Neutral Saboteur available without IX");
        owner->placeStructure(NONE_ID,Structure_IX,8,2,true,true);game->techLevel=6;
        require(!available(combo,Unit_Saboteur),"Neutral Saboteur available below level 7");
        game->techLevel=7;require(available(combo,Unit_Saboteur),"Neutral multiplayer Saboteur missing at tier 7 with IX");
        const auto entry=std::find_if(combo->getBuildList().begin(),combo->getBuildList().end(),[](const BuildItem& b){return b.itemID==Unit_Saboteur;});
        require(entry!=combo->getBuildList().end() && entry->price==500,"Neutral Saboteur price is not 500");
        auto* neutralWor=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_WOR,2,8,true,true));
        require(neutralWor && !available(neutralWor,Unit_Saboteur),"Saboteur available from ordinary WOR");
        combo->doProduceItem(Unit_Saboteur);require(combo->getCurrentProducedItem()==Unit_Saboteur,"Saboteur order rejected");combo->doCancelItem(Unit_Saboteur);
        game->gameType=GameType::CustomGame;require(available(combo,Unit_Saboteur),"Neutral Saboteur unavailable in single player");
        game->gameType=GameType::Campaign;require(available(combo,Unit_Saboteur),"Neutral Saboteur unavailable in campaign");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        if(mod=="Tornie" || mod=="ArcaKev") {
            const auto white=getHouseColorSDL(HOUSE_NEUTRAL,0),mid=getHouseColorSDL(HOUSE_NEUTRAL,3);
            require(white.r==255 && white.g==255 && white.b==255 && mid.r>=180,"Neutral white ramp was not brightened");
        }
        SDL_Log("RELEASE 543 NEUTRAL SABOTEUR PASS: %s, campaign and multiplayer, Neutral owner, Worfinery level 7 + IX, 500 credits",mod.c_str());
        ObjectData data;data.loadFromINIFile(ModManager::instance().getActiveObjectDataPath(),false);
        const int k=getRuntimeHouseForIdentity(HOUSE_KLESHMERSH),t=getRuntimeHouseForIdentity(HOUSE_THARPIQUE);
        require(data.data[Unit_RocketTrike][k].builder==Structure_LightFactory
            && data.data[Unit_RocketTrike][k].techLevel==3 && data.data[Unit_RocketTrike][k].upgradeLevel==1,"Kleshmersh Rocket Trike progression");
        require(data.data[Unit_FlameQuad][k].builder==Structure_LightFactory
            && data.data[Unit_FlameQuad][k].techLevel==5 && data.data[Unit_FlameQuad][k].upgradeLevel==2,"Kleshmersh Flame Quad progression");
        require(data.data[Unit_SonicTrike][k].builder==ItemID_Invalid,"Kleshmersh still builds Sonic Trikes");
        require(data.data[Unit_GasInfantry][t].builder==Structure_WOR,"Tharpique gas infantry unavailable");
    }

    game=std::make_unique<Game>();currentGame=game.get();
    auto gasInit=chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_ORDOS),options);
    gasInit.setMapData(gasInit.getFiledata()+"[Player2]\nCredits=1000000\n[Player3]\nCredits=1000000\n");
    for(int i=0;i<2;++i) {
        GameInitSettings::HouseInfo info(getRuntimeHouseForIdentity(i==0?HOUSE_HARKONNEN:HOUSE_ATREIDES),i==0?1:2);
        info.addPlayerInfo(GameInitSettings::PlayerInfo("Gas test "+std::to_string(i),"qBotEasy"));gasInit.addHouseInfo(info);
    }
    game->initGame(gasInit);owner=pLocalHouse;
    owner->createUnit(Unit_MCV,true);
    wor=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_WOR,2,2,true,true));
    auto* combo=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_Worfinery,8,2,true,true));
    require(wor&&combo,"gas production buildings");
    require(!available(wor,Unit_GasInfantry)&&!available(combo,Unit_GasInfantry),"gas infantry available without IX");
    owner->placeStructure(NONE_ID,Structure_IX,14,2,true,true);
    require(available(wor,Unit_GasInfantry)&&available(combo,Unit_GasInfantry),"gas infantry missing from WOR/Worfinery with IX");
    require(!available(combo,Unit_FlameTrooper),"Ordos Worfinery gained Harkonnen flame production without Chaos");
    auto* gas=dynamic_cast<GasInfantry*>(owner->placeUnit(Unit_GasInfantry,5,12,true));
    auto* damaged=game->getHouse(getRuntimeHouseForIdentity(HOUSE_HARKONNEN))->placeUnit(Unit_Tank,6,12,true);require(gas&&damaged,"gas infantry/target creation");
    damaged->setHealth(damaged->getMaxHealth()/2);const auto before=damaged->getHealth();
    require(gas->canHeal(),"gas infantry heal action unavailable");
    Command heal(pLocalPlayer->getPlayerID(),CMD_UNIT_HEAL,gas->getObjectID(),damaged->getObjectID());heal.executeCommand();const auto bulletsBefore=bulletList.size();
    gas->update();require(bulletList.size()>bulletsBefore && bulletList.back()->getBulletID()==Bullet_Heal,"gas infantry did not heal ally");
    // Resolve the actual missile, rather than directly applying health in the test.
    auto* missile=bulletList.back();
    for(int tick=0;tick<300;++tick) {bool exists=false;for(auto* b:bulletList)if(b==missile)exists=true;if(!exists)break;missile->update();}
    require(damaged->getHealth()>before && damaged->getOwner()!=owner,"healing missile did not restore a different allied house");
    auto* enemy=game->getHouse(getRuntimeHouseForIdentity(HOUSE_ATREIDES))->placeUnit(Unit_Tank,5,13,true);require(enemy,"enemy gas target");
    enemy->setVisible(-1,true);enemy->doSetAttackMode(STOP);
    gas->doAttackObject(enemy->getObjectID(),false);const auto beforeEnemy=bulletList.size();
    for(int tick=0;tick<1000 && bulletList.size()==beforeEnemy;++tick)gas->update();
    require(bulletList.size()>beforeEnemy && bulletList.back()->getBulletID()==Bullet_DRocket,"gas infantry did not use Deviator gas on enemy");
    auto* flame=owner->placeUnit(Unit_FlameTrooper,8,12,true);auto* quad=owner->placeUnit(Unit_FlameQuad,7,13,true);
    require(flame&&quad,"new unit factories");
    require(flame->canCaptureStructures(),"separate flame Trooper lost ordinary infantry capture ability");
    for(auto* unit:{flame,quad}) {
        const auto count=bulletList.size();unit->doAttackObject(enemy,false);
        for(int tick=0;tick<1000 && bulletList.size()==count;++tick)unit->update();
        require(bulletList.size()>count,unit->getItemID()==Unit_FlameQuad?"Flame Quad never fired":"Flame Trooper never fired");
        require(bulletList.back()->getBulletID()==Bullet_Flame,"new flame unit launched a non-flame weapon");
    }

    const auto gasID=gas->getObjectID(),flameID=flame->getObjectID(),quadID=quad->getObjectID();
    for(int zoom=0;zoom<3;++zoom)for(const int graphic:{ObjPic_FlameQuad,ObjPic_GasInfantry}) {
        auto textures=pGFXManager->getObjPic(graphic,owner->getHouseID());int w,h,baseW,baseH;
        SDL_QueryTexture(textures[0],nullptr,nullptr,&baseW,&baseH);SDL_QueryTexture(textures[zoom],nullptr,nullptr,&w,&h);
        require(w==baseW*(zoom+1)&&h==baseH*(zoom+1),"new sprite zoom ratio");
        SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);
        SDL_Rect rect{0,0,w,h};SDL_RenderCopy(renderer,textures[zoom],nullptr,&rect);
        auto pixels=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32)};
        require(SDL_RenderReadPixels(renderer,&rect,pixels->format->format,pixels->pixels,pixels->pitch)==0,"new unit atlas readback");
        int magenta=0,background=0,visible=0,black=0;
        for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
            Uint8 r,g,b,a;SDL_GetRGBA(getPixel(pixels.get(),x,y),pixels->format,&r,&g,&b,&a);
            if(r==255 && g==0 && b==255)++magenta;
            if(r==7 && g==11 && b==13)++background;else ++visible;
            if(r==0 && g==0 && b==0)++black;
        }
        require(magenta==0 && background>0 && visible>0,"magenta visible or transparency missing in rendered atlas");
        if(graphic==ObjPic_GasInfantry)require(black>0,"opaque black was erased with magenta");
        if(graphic==ObjPic_FlameQuad)for(int direction=0;direction<8;++direction) {
            const int cell=w/8;int left=cell,top=h,right=0,bottom=0;
            for(int y=0;y<h;++y)for(int x=0;x<cell;++x) {
                Uint8 r,g,b,a;SDL_GetRGBA(getPixel(pixels.get(),direction*cell+x,y),pixels->format,&r,&g,&b,&a);
                if(r==7 && g==11 && b==13)continue;
                left=std::min(left,x);top=std::min(top,y);right=std::max(right,x+1);bottom=std::max(bottom,y+1);
            }
            require(right>left && bottom>top,"empty Flame Quad direction");
            require(std::abs(left+right-cell)<=zoom+1 && std::abs(top+bottom-h)<=zoom+1,
                "Flame Quad visible bounds are off centre");
        }
        const auto preview=(std::filesystem::path(output)/(mod+"-"+std::to_string(graphic)+"-zoom"+std::to_string(zoom+1)+".png")).string();
        require(SavePNG(pixels.get(),preview.c_str())==0,"new unit rendered atlas preview");

    }
    for(const int house:{HOUSE_HARKONNEN,HOUSE_ATREIDES,HOUSE_ORDOS}) {
        const int color=getHouseVisualHouse(house);
        for(const int graphic:{ObjPic_FlameQuad,ObjPic_GasInfantry}) {
            auto* texture=pGFXManager->getZoomedObjPic(graphic,house,0);int w,h;
            SDL_QueryTexture(texture,nullptr,nullptr,&w,&h);
            SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);
            SDL_Rect area{0,0,w,h};SDL_RenderCopy(renderer,texture,nullptr,&area);
            auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32)};
            require(SDL_RenderReadPixels(renderer,&area,image->format->format,image->pixels,image->pitch)==0,"house colour readback");
            int teamPixels=0;
            for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
                Uint8 r,g,b,a;SDL_GetRGBA(getPixel(image.get(),x,y),image->format,&r,&g,&b,&a);
                for(int shade=0;shade<7;++shade) {
                    const auto expected=getHouseColorSDL(color,shade);
                    if(r==expected.r && g==expected.g && b==expected.b){++teamPixels;break;}
                }
            }
            require(teamPixels>0,"supplied unit has no pixels in its house colour ramp");
            const auto filename=(std::filesystem::path(output)/(mod+"-colour-"+std::to_string(house)+"-"+std::to_string(graphic)+".png")).string();
            require(SavePNG(image.get(),filename.c_str())==0,"colour preview export");
        }
        for(const int ui:{UI_MapEditor_FlameQuad,UI_MapEditor_GasInfantry,UI_MapEditor_FlameTrooper}) {
            auto* icon=pGFXManager->getUIGraphicSurface(ui,house);require(icon && icon->w==2*D2_TILESIZE && icon->h==2*D2_TILESIZE,"new unit editor icon canvas");
            int blue=0;
            for(int y=0;y<icon->h;++y)for(int x=0;x<icon->w;++x) {
                Uint8 r,g,b,a;SDL_GetRGBA(getPixel(icon,x,y),icon->format,&r,&g,&b,&a);
                if(a && r==24 && g==152 && b==255){++blue;require(x>=icon->w-8 && y>=icon->h-8,"editor star covers the unit centre");}
            }
            require(blue>0,"new unit editor blue star missing");
            const auto filename=(std::filesystem::path(output)/(mod+"-editor-"+std::to_string(house)+"-"+std::to_string(ui)+".png")).string();require(SavePNG(icon,filename.c_str())==0,"editor marker preview");
        }
    }
    for(const int picture:{Picture_FlameTrooper,Picture_GasInfantry}) {
        auto* texture=pGFXManager->getSmallDetailPic(picture);int w,h;SDL_QueryTexture(texture,nullptr,nullptr,&w,&h);
        SDL_SetRenderDrawColor(renderer,7,11,13,255);SDL_RenderClear(renderer);SDL_Rect area{0,0,w,h};SDL_RenderCopy(renderer,texture,nullptr,&area);
        auto image=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32)};
        require(SDL_RenderReadPixels(renderer,&area,image->format->format,image->pixels,image->pitch)==0,"infantry portrait readback");
        int blue=0;
        for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
            Uint8 r,g,b,a;SDL_GetRGBA(getPixel(image.get(),x,y),image->format,&r,&g,&b,&a);
            if(r==24 && g==152 && b==255){++blue;require(x>=w-10 && y>=h-10,"portrait star is too near the centre");}
        }
        require(blue>20,"infantry portrait marker missing or clipped");
        const auto filename=(std::filesystem::path(output)/(mod+"-portrait-"+std::to_string(picture)+".png")).string();require(SavePNG(image.get(),filename.c_str())==0,"portrait marker preview");
    }
    const auto save=(std::filesystem::path(output)/(mod+"-units543.dls")).string();require(game->saveGame(save),"new units save");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(save));
    require(dynamic_cast<GasInfantry*>(game->getObjectManager().getObject(gasID)) && game->getObjectManager().getObject(flameID)->getItemID()==Unit_FlameTrooper && game->getObjectManager().getObject(quadID)->getItemID()==Unit_FlameQuad,"new unit types lost after reload");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    SDL_Log("RELEASE 543 GRAPHICS PASS: %s, source VGA team colours, blue editor stars and bottom-right infantry portrait markers",mod.c_str());
    SDL_Log("RELEASE 543 UNITS PASS: %s, separate Harkonnen tier-seven IX flame Troopers, gas WOR/Worfinery IX, real healing missile, exact zoom ratios and save/reload",mod.c_str());
}
