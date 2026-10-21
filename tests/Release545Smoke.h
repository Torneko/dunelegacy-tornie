#pragma once
#include <Menu/MainMenu.h>
#include <Menu/SinglePlayerMenu.h>
#include <Menu/MainMenuButtonColor.h>
#include <Menu/HouseChoiceInfoMenu.h>
#include <Menu/HouseChoiceMenu.h>
#include <Menu/CampaignStatsMenu.h>
#include <Menu/MapChoice.h>
#include <Campaign/CoopCampaignRuntime.h>
#include <Achievements/AchievementEvents.h>
#include <Achievements/AchievementManager.h>
#include <structures/Palace.h>
#include <units/Saboteur.h>
#include <FileClasses/LoadSavePNG.h>

struct Release545Smoke {
    static void require(bool value,const std::string& text) {
        if(!value) throw std::runtime_error("Release 545: "+text);
    }
    static void verifyFrames(SDL_Surface* background,SDL_Surface* artwork,unsigned int screen) {
        auto expected=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,106,106,32,SDL_PIXELFORMAT_ARGB8888));
        SDL_Rect crop{0,8,166,166};
        require(SDL_BlitScaled(artwork,&crop,expected.get(),nullptr)==0,"gold frame scaling");
        const int cx=background->w/2,cy=background->h/2;
        const bool scores=screen==UI_GameStatsBackground;
        const int top=scores?cy-184:cy+90;
        if(scores) {
            // PictureFactory right-aligns the original 106px logo at cx+317,
            // inclusively. Check that last column, outside the former cleanup.
            auto* tile=pGFXManager->getBackgroundSurface();
            const int edge=cx+317;
            for(int y=top;y<top+108;++y)
                require((getPixel(background,edge,y)&0xffffff)==(getPixel(tile,edge,y)&0xffffff),
                    "old right score logo border remains at its inclusive edge");
        }
        for(const int left: {scores?cx-318:cx-316,cx+212}) {
            // The engraved plate, border and four studs retain original art;
            // its rectangular outer backing must now show the real menu tile.
            for(int y=0;y<106;++y)for(int x=0;x<106;++x) {
                if((x>=24&&x<82&&y>=14&&y<92)||(x>=14&&x<92&&y>=24&&y<82))
                    require((getPixel(background,left+x,top+1+y)&0xffffff)==(getPixel(expected.get(),x,y)&0xffffff),
                        "wrong faction, cropped border or changed colours in score/region frame");
            }
            for(int y: {13,93})for(int x: {13,93})
                require((getPixel(background,left+x,top+1+y)&0xffffff)==(getPixel(expected.get(),x,y)&0xffffff),"frame stud changed");
            auto* tile=pGFXManager->getBackgroundSurface();
            for(int x=0;x<106;++x)
                require((getPixel(background,left+x,top)&0xffffff)==(getPixel(tile,left+x,top)&0xffffff)
                    &&(getPixel(background,left+x,top+107)&0xffffff)==(getPixel(tile,left+x,top+107)&0xffffff),"frame backing differs from menu texture");
            for(int y=0;y<108;++y)
                require((getPixel(background,left,top+y)&0xffffff)==(getPixel(tile,left,top+y)&0xffffff)
                    &&(getPixel(background,left+105,top+y)&0xffffff)==(getPixel(tile,left+105,top+y)&0xffffff),"rectangular gold/black frame fringe remains");
        }
    }
    static sdl2::surface_ptr capturePortrait(SDL_Texture* texture) {
        require(texture,"missing portrait texture");
        SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);
        SDL_Rect area{0,0,91,55};SDL_RenderCopy(renderer,texture,nullptr,&area);
        auto pixels=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,91,55,32,SDL_PIXELFORMAT_ARGB8888));
        require(SDL_RenderReadPixels(renderer,&area,pixels->format->format,pixels->pixels,pixels->pitch)==0,"portrait capture");return pixels;
    }
    static void verifyTrooperPortraits(const std::string& mod,const std::string& output) {
        const char* files[2][3]={
            {"Troopers_Fremen_alone.png","Troopers_Fremen_3.png","Troopers_Fremen_5.png"},
            {"Troopers_Sardaukar_Alone.png","Troopers_Sardaukar_3.png","Troopers_Sardaukar_5.png"}};
        const char* selected[2]={"Troopers_Fremen_unit_selection.png","Troopers_Sardaukar_unit_selection.png"};
        const int items[3]={Unit_Trooper,Unit_Troopers,Unit_Troopers5};
        const auto reference=[](const char* name) {
            auto raw=LoadPNG_RW(pFileManager->openFile(name).get());
            require(raw&&raw->w==91&&raw->h==55,"original Trooper portrait dimensions");
            auto texture=convertSurfaceToTexture(std::move(raw));return capturePortrait(texture.get());
        };
        const auto same=[](SDL_Surface* actual,SDL_Surface* expected) {
            for(int y=0;y<55;++y)for(int x=0;x<91;++x)
                require(getPixel(actual,x,y)==getPixel(expected,x,y),"Trooper portrait wrong context, size, tint or added star");
        };
        for(int faction=0;faction<2;++faction) {
            const auto runtime=getRuntimeHouseForIdentity(faction==0?HOUSE_FREMEN:HOUSE_SARDAUKAR);
            auto options=effectiveGameOptions;options.chaosMode=false;
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(chaosSmokeInit(runtime,options));
            pLocalHouse->createUnit(Unit_MCV,true);
            const int previousColour=getHouseVisualHouse(runtime);
            setHouseVisualHouse(runtime,HOUSECOLOR_GUEST_1);
            auto expectedSelection=reference(selected[faction]);
            for(const int item:items) {
                auto actual=capturePortrait(resolveItemPicture(item,runtime));same(actual.get(),expectedSelection.get());
                require(pGFXManager->getFactionTrooperPortrait(item,HOUSE_ATREIDES)==nullptr
                    &&pGFXManager->getFactionTrooperProductionPortrait(item,HOUSE_ATREIDES)==nullptr,"faction artwork leaked to Atreides");
            }
            auto* unit=pLocalHouse->createUnit(Unit_Trooper,true);require(unit,"selected Trooper fixture");
            {
                std::unique_ptr<ObjectInterface> panel(unit->getInterfaceContainer());require(panel!=nullptr,"selected Trooper interface");
                int checked=0;
                std::function<void(Widget*)> inspect=[&](Widget* widget) {
                    if(auto* picture=dynamic_cast<PictureLabel*>(widget);picture&&picture->getMinimumSize()==Point(91,55)) {
                        SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);picture->draw(Point(0,0));
                        auto actual=renderReadSurface(renderer);same(actual.get(),expectedSelection.get());++checked;
                        SavePNG(actual.get(),(std::filesystem::path(output)/(mod+"-trooper-selected-"+std::to_string(faction)+".png")).string().c_str());
                    }
                    widget->forEachChildWidget(inspect);
                };
                inspect(panel.get());require(checked==1,"selected-unit portrait widget not verified");
            }
            for(const int kind:{Structure_WOR,Structure_Worfinery}) {
                if(kind==Structure_Worfinery&&mod=="vanilla")continue;
                auto* building=pLocalHouse->placeStructure(NONE_ID,kind,2,kind==Structure_WOR?2:8,true,true);require(building,"Trooper production building");
                StaticContainer panel;auto* list=BuilderList::create(building->getObjectID());panel.addWidget(list,Point(0,0),list->getMinimumSize());
                for(int size=0;size<3;++size) {
                    auto actual=capturePortrait(list->getProductionPortrait(items[size]));auto expected=reference(files[faction][size]);same(actual.get(),expected.get());
                    SavePNG(actual.get(),(std::filesystem::path(output)/(mod+"-trooper-production-"+std::to_string(faction)+"-"+std::to_string(size)+".png")).string().c_str());
                }
            }
            setHouseVisualHouse(runtime,previousColour);
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        }
        SDL_Log("RELEASE 545 TROOPER PORTRAITS PASS: %s, six production variants and two distinct selected-unit portraits, original pixels and bonus colours",mod.c_str());
    }
    static void run(const std::string& output) {
        auto& mods=ModManager::instance();const auto previous=mods.getActiveModName();
        MainMenu menuPreview;
        SinglePlayerMenu sectionPreview;
        const char* names[]={"Harkonnen","Atreides","Ordos","Fremen","Sardaukar","Mercenary","Neutral","Rebels","Corruptique","Wildspade","Kleshmersh","Tharpique"};
        for(const std::string mod:{"Tornie","Jericho","ArcaKev","TornieLite","JerichoLite","ArcaKevLite","vanilla"}) {
            require(mods.setActiveMod(mod),"mod switch");
            effectiveGameOptions=mods.loadEffectiveGameOptions(settings.gameOptions);
            menuPreview.refreshModVersionLabel();menuPreview.draw();
            SavePNG(renderReadSurface(renderer).get(),(std::filesystem::path(output)/(mod+"-main-menu.png")).string().c_str());
            const Uint32 expectedColour=MainMenuButtonColor::colorForMod(mod);
            std::function<void(Widget*)> verifyColour=[&](Widget* widget) {
                if(auto* button=dynamic_cast<TextButton*>(widget))
                    require(button->getTextColor()==expectedColour,"stale/wrong menu button colour after switching to "+mod);
                widget->forEachChildWidget(verifyColour);
            };
            verifyColour(&menuPreview);
            sectionPreview.draw();verifyColour(&sectionPreview);
            SavePNG(renderReadSurface(renderer).get(),(std::filesystem::path(output)/(mod+"-single-menu.png")).string().c_str());
            if(mod=="Tornie")require(expectedColour==getHouseColorRGB(HOUSE_REBELS,3),"menu dark green differs from Tornie Rebels");
            if(mod=="Jericho")require(expectedColour==COLOR_RGB(52,52,52),"Jericho menu must be dark grey");
            if(mod=="ArcaKev")require(expectedColour==COLOR_RGB(176,64,24),"ArcaKev menu must be dark reddish orange");
            for(int runtime=0;runtime<NUM_HOUSES;++runtime) {
                const auto house=static_cast<HOUSETYPE>(runtime);if(!isCampaignHouseAvailable(house))continue;
                const auto identity=getHouseFactionIdentity(house);
                auto png=LoadPNG_RW(pFileManager->openFile(std::string("CampaignHerald")+names[identity]+".png").get());
                auto expected=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,166,182,32,SDL_PIXELFORMAT_ARGB8888));
                SDL_FillRect(expected.get(),nullptr,SDL_MapRGB(expected->format,0,0,0));SDL_Rect square{0,8,166,166};
                SDL_BlitScaled(png.get(),nullptr,expected.get(),&square);
                auto* actual=pGFXManager->getUIGraphicSurface(UI_FactionFrame,runtime);
                require(actual&&actual->w==166&&actual->h==182,"faction frame dimensions");
                for(int y=0;y<182;++y)for(int x=0;x<166;++x)
                    require((getPixel(actual,x,y)&0xffffff)==(getPixel(expected.get(),x,y)&0xffffff),"wrong identity after switch: "+mod+names[identity]);
                const int previousColour=getHouseVisualHouse(runtime);
                setHouseVisualHouse(house,NUM_HOUSES+runtime%(NUM_HOUSE_COLOR_SLOTS-NUM_HOUSES));
                for(const unsigned int screen: {UI_GameStatsBackground,UI_MapChoiceScreen}) {
                    auto background=pGFXManager->createFactionFrameBackground(screen,runtime);
                    verifyFrames(background.get(),expected.get(),screen);
                }
                setHouseVisualHouse(house,previousColour);
                HouseChoiceInfoMenu menu(runtime);auto* anim=menu.getPlanetAnimation();
                require(anim&&!anim->getFrames().empty(),"campaign confirmation animation");
                auto* frame=anim->getFrames().front().get();
                auto* banner=pGFXManager->getUIGraphicSurface(UI_Herald_ColoredLarge,runtime);
                auto ordinary=sdl2::surface_ptr(SDL_ConvertSurfaceFormat(banner,SDL_PIXELFORMAT_ARGB8888,0));
                auto confirmation=sdl2::surface_ptr(SDL_ConvertSurfaceFormat(frame,SDL_PIXELFORMAT_ARGB8888,0));
                for(int y=0;y<126;++y)for(int x=0;x<166;++x)
                    require((getPixel(confirmation.get(),12+x,66+y)&0xffffff)==(getPixel(ordinary.get(),x,y)&0xffffff),
                        "ordinary campaign banner replaced by a golden frame or another faction: "+mod+names[identity]);
                if(identity==HOUSE_FREMEN) {
                    SDL_RenderClear(renderer);menu.setText("");menu.onMentatTextFinished();menu.draw();
                    auto screen=renderReadSurface(renderer);SavePNG(screen.get(),(std::filesystem::path(output)/(mod+"-ordinary-campaign.png")).string().c_str());
                    MapChoice regions(runtime,1,0);regions.draw();screen=renderReadSurface(renderer);
                    verifyFrames(screen.get(),expected.get(),UI_MapChoiceScreen);
                    SavePNG(screen.get(),(std::filesystem::path(output)/(mod+"-gold-regions.png")).string().c_str());
                }
            }
            verifyTrooperPortraits(mod,output);
            SDL_Log("RELEASE 545 PRESENTATION PASS: %s, score/region identity frames, ordinary campaign banners and eight original Trooper portraits",mod.c_str());
        }
        require(mods.setActiveMod("Tornie"),"co-op fixture mod");
        const auto playerName=settings.general.playerName;
        auto& manager=achievements::AchievementManager::instance();
        AchievementEvents::initializeProfile();
        const auto previousLosses=manager.statistic("UnitsLost");
        const auto previousSabotages=manager.statistic("PalacesSabotaged");
        unsigned profileSerial=0;
        const auto beginIsolated=[&](achievements::MatchInfo info) {
            // begin(Game) has switched back to the user's smoke profile. Each
            // explicit achievement fixture gets its own independent profile.
            const auto path=std::filesystem::path(output)/("achievements-545-"+std::to_string(++profileSerial)+".ini");
            std::error_code error;std::filesystem::remove(path,error);
            require(!error,"reset isolated achievement fixture");
            require(manager.configure(getDuneLegacyDataDir()+"/config/Achievements.ini",path.string()),"isolated achievement fixture");
            info.enabled=true;manager.begin(info);
        };
        const auto roster=getFactionSelectionOrder();
        for(int count=2;count<=4;++count) {
            auto session=coop::CoopCampaignSession::create("545-achievement-"+std::to_string(count),"Tornie",roster,
                {roster[0],roster[1]},{playerName,"Rescue Two"},181245,false);
            session.enableExpandedAllies(false);
            if(count>=3)session.configureAlly(2,roster[2],HUMANPLAYERCLASS,"Rescue Three");
            if(count>=4)session.configureAlly(3,roster[3],HUMANPLAYERCLASS,"Rescue Four");
            auto init=coop::makeGameSettings(session,effectiveGameOptions,"Co-op rescue check");
            const auto context=coop::readContext(init.getFiledata());require(context.has_value(),"co-op context");
            const auto indices=context->alliedSlots();
            for(int perspective=0;perspective<count;++perspective) {
                settings.general.playerName=context->slots[indices[perspective]].playerName;
                {
                    auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
                    game->winFlags=0;game->loseFlags=0;
                    auto* defeated=game->getHouse(context->slots[indices[0]].house);
                    std::vector<UnitBase*> units;for(auto* unit:unitList)if(unit->getOwner()==defeated)units.push_back(unit);
                    std::vector<StructureBase*> structures;for(auto* structure:structureList)if(structure->getOwner()==defeated)structures.push_back(structure);
                    for(auto* structure:structures){structure->setHealth(0);structure->destroy();}
                    // Destroy any surviving crew spawned by vehicle/building deaths.
                    for(int wave=0;wave<4;++wave) {
                        units.clear();for(auto* unit:unitList)if(unit->getOwner()==defeated)units.push_back(unit);
                        if(units.empty())break;
                        for(auto* unit:units){unit->setHealth(0);unit->destroy();}
                    }
                    require(!defeated->isAlive(),"defeated co-op army still alive");
                    AchievementEvents::begin(*game,"",false,true);auto info=manager.match().info;
                    require(info.cooperative,"active allied context not detected");beginIsolated(info);
                    game->setGameWon();
                    const auto& counters=manager.match().counts;
                    require(counters.count(perspective==0?"CoopVictoriesRescued":"CoopVictoriesRescuing"),"wrong local co-op outcome");
                    require(!counters.count(perspective!=0?"CoopVictoriesRescued":"CoopVictoriesRescuing"),"opposite co-op achievement also counted");
                }
                currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
            }
            SDL_Log("RELEASE 545 COOP PASS: %d allied factions, every local perspective",count);
        }
        settings.general.playerName=playerName;
        auto init=coop::makeGameSettings(coop::CoopCampaignSession::create("545-sabotage","Tornie",roster,
            {roster[0],roster[1]},{playerName,"Sabotage Ally"},181246,false),effectiveGameOptions,"Palace sabotage check");
        {
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);game->winFlags=0;game->loseFlags=0;
            House* enemy=nullptr;for(int i=0;i<NUM_HOUSES;++i)if(auto* h=game->getHouse(i);h&&h->getTeamID()!=pLocalHouse->getTeamID()){enemy=h;break;}
            require(enemy,"sabotage enemy");
            auto* palace=enemy->placeStructure(NONE_ID,Structure_Palace,22,22,true,true);require(palace,"sabotage palace");
            auto* saboteur=pLocalHouse->createUnit(Unit_Saboteur);require(saboteur,"sabotage attacker");
            saboteur->deploy(Coord(21,22));palace->setVisible(pLocalHouse->getTeamID(),true);
            AchievementEvents::begin(*game,"",false,true);auto info=manager.match().info;beginIsolated(info);
            const auto before=manager.statistic("PalacesSabotaged");saboteur->doAttackObject(palace,true);saboteur->update();
            require(manager.statistic("PalacesSabotaged")==before+1,"real Saboteur palace detonation not credited");
            require(manager.match().counts.at("EnemyStructuresDestroyed")==1,"sabotage destruction attribution");
            CampaignStatsMenu scores(1);scores.draw();auto screen=renderReadSurface(renderer);
            verifyFrames(screen.get(),pGFXManager->getUIGraphicSurface(UI_FactionFrame,pLocalHouse->getHouseID()),UI_GameStatsBackground);
            SavePNG(screen.get(),(std::filesystem::path(output)/"gold-scores.png").string().c_str());
        }
        currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        {
            MainMenu menu;
            require(menu.windowWidget.getWidgetPosition(&menu.replayIntroButton).x==menu.windowWidget.getWidgetPosition(&menu.cooperativeCampaignButton).x
                &&menu.windowWidget.getWidgetPosition(&menu.replayIntroButton).y+menu.replayIntroButton.getSize().y<menu.windowWidget.getWidgetPosition(&menu.cooperativeCampaignButton).y,"intro button placement");
            menu.draw();auto screen=renderReadSurface(renderer);SavePNG(screen.get(),(std::filesystem::path(output)/"main-menu-replay-intro.png").string().c_str());
            const auto enabled=settings.audio.playMusic;const auto volume=settings.audio.musicVolume;
            SDL_Event escape{};escape.type=SDL_KEYDOWN;escape.key.keysym.sym=SDLK_ESCAPE;SDL_PushEvent(&escape);
            menu.onReplayIntro();require(settings.audio.playMusic==enabled&&settings.audio.musicVolume==volume,"replay changed audio preferences");
        }
        require(manager.statistic("UnitsLost")==previousLosses && manager.statistic("PalacesSabotaged")==previousSabotages,
            "native achievement fixtures modified the shared profile");
        require(mods.setActiveMod(previous),"restore mod");
        SDL_Log("RELEASE 545 COMPLETE: portraits, 63 faction score/region frames, original campaign banners, 2/3/4-player rescue, real palace sabotage and intro replay");
    }
};
