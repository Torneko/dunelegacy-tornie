#pragma once
#include <Menu/MainMenu.h>
#include <Menu/HouseChoiceInfoMenu.h>
#include <Menu/HouseChoiceMenu.h>
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
    static void run(const std::string& output) {
        auto& mods=ModManager::instance();const auto previous=mods.getActiveModName();
        const char* names[]={"Harkonnen","Atreides","Ordos","Fremen","Sardaukar","Mercenary","Neutral","Rebels","Corruptique","Wildspade","Kleshmersh","Tharpique"};
        const auto capture=[](SDL_Texture* texture) {
            require(texture,"missing portrait texture");
            SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);
            SDL_Rect area{0,0,91,55};SDL_RenderCopy(renderer,texture,nullptr,&area);
            auto pixels=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,91,55,32,SDL_PIXELFORMAT_ARGB8888));
            require(SDL_RenderReadPixels(renderer,&area,pixels->format->format,pixels->pixels,pixels->pitch)==0,"portrait capture");return pixels;
        };
        for(const std::string mod:{"Tornie","Jericho","ArcaKev","TornieLite","JerichoLite","ArcaKevLite","vanilla"}) {
            require(mods.setActiveMod(mod),"mod switch");
            effectiveGameOptions=mods.loadEffectiveGameOptions(settings.gameOptions);
            for(int runtime=0;runtime<NUM_HOUSES;++runtime) {
                const auto house=static_cast<HOUSETYPE>(runtime);if(!isCampaignHouseAvailable(house))continue;
                const auto identity=getHouseFactionIdentity(house);
                auto png=LoadPNG_RW(pFileManager->openFile(std::string("CampaignHerald")+names[identity]+".png").get());
                auto expected=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,166,182,32,SDL_PIXELFORMAT_ARGB8888));
                SDL_FillRect(expected.get(),nullptr,SDL_MapRGB(expected->format,0,0,0));SDL_Rect square{0,8,166,166};
                SDL_BlitScaled(png.get(),nullptr,expected.get(),&square);
                auto* actual=pGFXManager->getUIGraphicSurface(UI_CampaignHeraldLarge,runtime);
                require(actual&&actual->w==166&&actual->h==182,"campaign frame dimensions");
                for(int y=0;y<182;++y)for(int x=0;x<166;++x)
                    require((getPixel(actual,x,y)&0xffffff)==(getPixel(expected.get(),x,y)&0xffffff),"wrong identity after switch: "+mod+names[identity]);
                HouseChoiceInfoMenu menu(runtime);auto* anim=menu.getPlanetAnimation();
                require(anim&&!anim->getFrames().empty(),"campaign confirmation animation");
                auto* frame=anim->getFrames().front().get();
                auto plaque=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,126,126,32,SDL_PIXELFORMAT_ARGB8888));
                SDL_Rect crop{0,8,166,166};SDL_BlitScaled(actual,&crop,plaque.get(),nullptr);
                for(int y=0;y<126;++y)for(int x=0;x<126;++x)
                    require((getPixel(frame,32+x,66+y)&0xffffff)==(getPixel(plaque.get(),x,y)&0xffffff),"campaign gold frame cropped or absent");
                for(int y=0;y<126;++y)for(int x: {12,31,158,177})
                    require((getPixel(frame,x,66+y)&0xffffff)==0,"old campaign plaque still visible beside golden frame");
                if(identity==HOUSE_FREMEN) {
                    SDL_RenderClear(renderer);menu.setText("");menu.onMentatTextFinished();menu.draw();
                    auto screen=renderReadSurface(renderer);SavePNG(screen.get(),(std::filesystem::path(output)/(mod+"-gold-campaign.png")).string().c_str());
                }
            }
            struct Portrait {int identity,item;const char* filename;};
            for(const Portrait p: {Portrait{HOUSE_FREMEN,Unit_Trooper,"Troopers_Fremen_alone.png"},
                    {HOUSE_FREMEN,Unit_Troopers5,"Troopers_Fremen_5.png"},
                    {HOUSE_SARDAUKAR,Unit_Troopers,"Troopers_Sardaukar_3.png"},
                    {HOUSE_SARDAUKAR,Unit_Troopers5,"Troopers_Sardaukar_5.png"}}) {
                const auto runtime=getRuntimeHouseForIdentity(static_cast<HOUSETYPE>(p.identity));
                auto original=LoadPNG_RW(pFileManager->openFile(p.filename).get());
                auto texture=convertSurfaceToTexture(std::move(original));auto expected=capture(texture.get());
                auto actual=capture(resolveItemPicture(p.item,runtime));
                for(int y=0;y<55;++y)for(int x=0;x<91;++x)
                    require(getPixel(actual.get(),x,y)==getPixel(expected.get(),x,y),"portrait altered/incorrect/starred");
                require(pGFXManager->getFactionTrooperPortrait(p.item,HOUSE_ATREIDES)==nullptr,"faction portrait leaked to Atreides");
            }
            SDL_Log("RELEASE 545 PRESENTATION PASS: %s, identity frames, complete confirmations and four original portraits",mod.c_str());
        }
        require(mods.setActiveMod("Tornie"),"co-op fixture mod");
        const auto playerName=settings.general.playerName;
        auto& manager=achievements::AchievementManager::instance();
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
                    require(info.cooperative,"active allied context not detected");info.enabled=true;manager.begin(info);
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
            AchievementEvents::begin(*game,"",false,true);auto info=manager.match().info;info.enabled=true;manager.begin(info);
            const auto before=manager.statistic("PalacesSabotaged");saboteur->doAttackObject(palace,true);saboteur->update();
            require(manager.statistic("PalacesSabotaged")==before+1,"real Saboteur palace detonation not credited");
            require(manager.match().counts.at("EnemyStructuresDestroyed")==1,"sabotage destruction attribution");
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
        require(mods.setActiveMod(previous),"restore mod");
        SDL_Log("RELEASE 545 COMPLETE: portraits, 63 campaign frames, 2/3/4-player rescue, real palace sabotage and intro replay");
    }
};
