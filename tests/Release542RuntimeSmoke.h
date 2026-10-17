#pragma once
#include <structures/WindTrap.h>
#include <GUI/CompletionButton.h>
#include <GUI/HorizontalScrollView.h>
#include <misc/FileSystem.h>
#include <misc/SaveGameLobbySetup.h>

inline void exportRebelsWindtrapPreview(const std::string& output,int house) {
    auto require=[](bool ok,const std::string& message){if(!ok)throw std::runtime_error("Windtrap preview: "+message);};
                auto blue=pGFXManager->getObjPic(ObjPic_Windtrap,house);
                auto green=pGFXManager->getObjPic(ObjPic_Windtrap,house,true);
                const auto size=getTextureSize(blue[2]);const int w=size.x/NUM_WINDTRAP_ANIMATIONS_PER_ROW;
                const int rows=(2+NUM_WINDTRAP_ANIMATIONS+NUM_WINDTRAP_ANIMATIONS_PER_ROW-1)/NUM_WINDTRAP_ANIMATIONS_PER_ROW;
                const int h=size.y/rows;SDL_Rect area{0,0,2*w+48,h+32};
                for(int frame=0;frame<NUM_WINDTRAP_ANIMATIONS;++frame){
                    SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,30,30,30,255);SDL_RenderClear(renderer);
                    SDL_Rect src{((frame+2)%NUM_WINDTRAP_ANIMATIONS_PER_ROW)*w,((frame+2)/NUM_WINDTRAP_ANIMATIONS_PER_ROW)*h,w,h};
                    SDL_Rect dst{16,16,w,h};SDL_RenderCopy(renderer,blue[2],&src,&dst);dst.x=w+32;SDL_RenderCopy(renderer,green[2],&src,&dst);
                    auto pixels=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,area.w,area.h,32,SDL_PIXELFORMAT_RGBA32)};
                    require(SDL_RenderReadPixels(renderer,&area,pixels->format->format,pixels->pixels,pixels->pitch)==0,"preview render");
                    require(SavePNG(pixels.get(),(std::filesystem::path(output)/fmt::sprintf("rebels-windtrap-preview-%03d.png",frame)).string().c_str())==0,"preview PNG");
                }
}

inline void verifyRelease542Rules(const std::string& output,const std::string& mod) {
    auto require=[](bool ok,const std::string& message){if(!ok)throw std::runtime_error("Release 542: "+message);};
    if(mod=="Tornie") {
        struct Probe : Widget {
            int clicked=-1;
            Point getMinimumSize() const override { return Point(1200,100); }
            bool handleMouseLeft(Sint32 x,Sint32,bool pressed) override { if(!pressed)clicked=x;return true; }
        } probe;
        HorizontalScrollView scroll;scroll.setContent(&probe);scroll.resize(320,160);
        require(scroll.getMaximumOffset()==880,"horizontal overflow range");
        scroll.handleMouseLeft(310,150,true);scroll.handleMouseLeft(310,150,false);
        require(scroll.getScrollOffset()>0,"right arrow does not scroll");
        scroll.scrollBy(10000);scroll.handleMouseLeft(20,20,true);scroll.handleMouseLeft(20,20,false);
        require(probe.clicked==900,"scrolled click targets the wrong content position");
        scroll.resize(1600,160);require(scroll.getMaximumOffset()==0&&scroll.getScrollOffset()==0,"wide window must remove scrolling");
        CompletionButton badge;badge.setText("HAUTS FAITS");badge.resize(182,26);
        int yellow[2]={0,0};
        for(int completed=0;completed<2;++completed) {
            SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);
            badge.setCompleted(completed);badge.draw(Point(10,10));auto image=renderReadSurface(renderer);
            for(int y=10;y<36;++y)for(int x=10;x<40;++x){Uint8 r,g,b;SDL_GetRGB(getPixel(image.get(),x,y),image->format,&r,&g,&b);if(r==255&&g==220&&b==0)++yellow[completed];}
        }
        require(yellow[1]>yellow[0]+50,"yellow completion star is missing: "+std::to_string(yellow[0])+" / "+std::to_string(yellow[1]));
    }
    verifyEditorSpiceDefaults(output,mod);
    const auto options=effectiveGameOptions;
    require(options.content.spiceMask()==ModContentOptions::legacy(mod).spiceMask(),"installed spice palette");
    if(mod=="Jericho"||mod=="JerichoLite")require(!options.sandwormsRespawn && options.maximumNumberOfHarvestersOverride==0,"Jericho default options");
    if(isArcaKevMod(mod))require(!options.rocketTurretsNeedPower,"ArcaKev Need power option");
    if(const char* oldDirectory=std::getenv("DUNELEGACY_LEGACY_SMOKE_DIR")) {
        const auto file=(std::filesystem::path(oldDirectory)/(mod+"-expanded-0-3.dls")).string();
        const auto bytes=readCompleteFile(file);const auto setup=readSaveGameLobbySetup(bytes);
        GameInitSettings load(getBasename(file,true),bytes,"Previous 541 save");
        for(const auto& house:setup.houses)load.addHouseInfo(house);
        const auto playerName=settings.general.playerName;
        for(const auto& house:setup.houses)for(const auto& player:house.playerInfoList)if(player.playerClass==HUMANPLAYERCLASS){settings.general.playerName=player.playerName;break;}
        auto previous=std::make_unique<Game>();currentGame=previous.get();previous->initGame(load);
        require(previous->getLoadedSavegameVersion()==9833,"legacy fixture is not a published 541 save");
        int checked=0;for(auto* building:structureList)if(auto* trap=dynamic_cast<WindTrap*>(building)){require(!trap->isPowerUpgraded(),"legacy Windtrap acquired an upgrade");++checked;}
        require(checked>0,"legacy save has no Windtraps");
        previous.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        settings.general.playerName=playerName;
        effectiveGameOptions=options;
        SDL_Log("LEGACY 541 WINDTRAP PASS: %s, %d ordinary Windtraps loaded",mod.c_str(),checked);
    }
    auto init=chaosSmokeInit(getRuntimeHouseForIdentity(isLiteMod(mod) ? HOUSE_ATREIDES : HOUSE_REBELS),options);
    auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
    auto* owner=pLocalHouse;
    if(!isLiteMod(mod)) {
        auto* trap=dynamic_cast<WindTrap*>(owner->placeStructure(NONE_ID,Structure_WindTrap,4,4,true,true));
        auto* second=dynamic_cast<WindTrap*>(owner->placeStructure(NONE_ID,Structure_WindTrap,8,4,true,true));
        require(trap&&second,"Windtrap placement");
        game->techLevel=7;require(!trap->doPowerUpgrade(),"upgrade without IX");
        owner->placeStructure(NONE_ID,Structure_IX,12,4,true,true);
        game->techLevel=6;require(!trap->doPowerUpgrade(),"upgrade below technology 7");
        game->techLevel=7;
        if(mod=="vanilla")require(!trap->doPowerUpgrade(),"Vanilla must not offer this upgrade");
        else {
            const auto funds=owner->getCredits();const int before=owner->getProducedPower();
            Command unauthorized(255,CMD_WINDTRAP_POWER_UPGRADE,trap->getObjectID());unauthorized.executeCommand();
            require(!trap->isPowerUpgraded(),"unauthorized player upgraded a Windtrap");
            Command upgrade(pLocalPlayer->getPlayerID(),CMD_WINDTRAP_POWER_UPGRADE,trap->getObjectID());
            OMemoryStream wire;wire.open();upgrade.save(wire);IMemoryStream received(wire.getData(),wire.getDataLength());
            Command replayed(received);replayed.executeCommand();
            require(trap->isPowerUpgraded(),"serialized Rebels power command failed");
            require(owner->getCredits()==funds-150,"upgrade price must be 150");
            require(owner->getProducedPower()==before+before/2,"first of two Windtraps must double its output");
            require(!trap->doPowerUpgrade() && !second->isPowerUpgraded(),"upgrade applied twice or to another Windtrap");
            const int full=owner->getProducedPower();trap->setHealth(trap->getMaxHealth()/2);
            require(owner->getProducedPower()==before,"damage must scale upgraded power");trap->setHealth(trap->getMaxHealth());
            require(owner->getProducedPower()==full,"repair must restore upgraded power");
            owner->takeCredits(owner->getCredits()-149);require(!second->doPowerUpgrade(),"insufficient funds accepted");owner->addCredits(1000);
            for(int z=0;z<NUM_ZOOMLEVEL;++z)require(pGFXManager->getObjPic(ObjPic_Windtrap,owner->getHouseID(),true)[z]!=pGFXManager->getObjPic(ObjPic_Windtrap,owner->getHouseID())[z],"green and blue atlases share a texture");
            if(mod=="ArcaKev")exportRebelsWindtrapPreview(output,owner->getHouseID());
            const auto id=trap->getObjectID();const auto file=(std::filesystem::path(output)/(mod+"-windtrap-upgrade.dls")).string();
            require(game->saveGame(file),"upgrade save");game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
            GameInitSettings load(file);
            game=std::make_unique<Game>();currentGame=game.get();game->initGame(load);
            auto* restored=dynamic_cast<WindTrap*>(game->getObjectManager().getObject(id));
            require(restored&&restored->isPowerUpgraded()&&!restored->doPowerUpgrade(),"upgrade not preserved by save/reload");
        }
    }
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    SDL_Log("RELEASE 542 PASS: %s, spice/options and per-Windtrap power upgrade",mod.c_str());
}
