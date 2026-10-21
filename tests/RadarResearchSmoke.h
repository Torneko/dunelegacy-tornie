#pragma once
#include <structures/Radar.h>
#include <structures/WOR.h>
#include <structures/RepairYard.h>
#include <SoundPlayer.h>
#include <structures/BuilderBase.h>
#include <GUI/ObjectInterfaces/RadarInterface.h>
#include <mod/RadarResearch.h>
#include <Explosion.h>
#include <misc/OMemoryStream.h>
#include <misc/IMemoryStream.h>

struct RadarResearchFixture {
    static void verifyButtons(StructureBase* building,const std::string& output,const std::string& mod,int house,const std::string& stage) {
        if(!building)throw std::runtime_error("Radar building missing for interface preview");
        std::unique_ptr<ObjectInterface> ui(building->getInterfaceContainer());
        auto* radarUI=dynamic_cast<RadarInterface*>(ui.get());
        if(!radarUI || !ui->update())throw std::runtime_error("Radar interface missing");
        const auto colour=getHouseColorRGB(getHouseVisualHouse(pLocalHouse->getHouseID()),3);
        for(int kind=0;kind<radar::Count;++kind) {
            auto& button=radarUI->researchButtons[kind];
            if(button.isVisible()!=(mod!="vanilla" && currentGame->techLevel>=radar::level(kind)) || button.getPrice()!=radar::cost(kind) || button.getTextColor()!=colour)
                throw std::runtime_error("Radar price/house colour/technology visibility mismatch");
            if(stage=="level-7-no-ix" && kind==radar::Specialization && button.isEnabled())
                throw std::runtime_error("Radar specialization enabled without IX");
            if(!button.isVisible())continue;
            button.resize(119,38);
            if(button.getLabelFontSize()<10)throw std::runtime_error("Radar label no longer readable at sidebar width");
            if(stage=="complete" && button.getStatus()!="OK")throw std::runtime_error("Radar completion not displayed separately");
            if(stage=="active" && kind==radar::BaseExpansion && button.getStatus().find('%')==std::string::npos)
                throw std::runtime_error("Radar progress not displayed separately");
            SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,20,20,20,255);SDL_RenderClear(renderer);
            button.draw(Point(0,0));auto screen=renderReadSurface(renderer);
            const auto quote=std::to_string(radar::cost(kind));
            auto glyphs=pFontManager->createSurfaceWithText(quote,COLOR_WHITE,12);
            auto rgba=sdl2::surface_ptr(SDL_ConvertSurfaceFormat(glyphs.get(),SDL_PIXELFORMAT_ARGB8888,0));
            const int left=119-pFontManager->getTextWidth(quote,12)-5,top=38-pFontManager->getTextHeight(12)-3;
            int checked=0;
            for(int y=0;y<rgba->h;++y)for(int x=0;x<rgba->w;++x) {
                auto pixel=*(reinterpret_cast<Uint32*>(static_cast<Uint8*>(rgba->pixels)+y*rgba->pitch)+x);
                Uint8 r,g,b,a;SDL_GetRGBA(pixel,rgba->format,&r,&g,&b,&a);
                if(a==255&&r==255&&g==255&&b==255) {
                    if((getPixel(screen.get(),left+x,top+y)&0xffffff)!=0xffffff)
                        throw std::runtime_error("Radar price overlay clipped, overlapped or missing");
                    ++checked;
                }
            }
            if(checked<10)throw std::runtime_error("Radar price overlay has too few readable pixels");
        }
        ui->resize(SIDEBARWIDTH,300);
        SDL_SetRenderDrawColor(renderer,20,20,20,255);SDL_RenderClear(renderer);ui->draw(Point(0,0));
        auto picture=renderReadSurface(renderer);
        if(SavePNG(picture.get(),(std::filesystem::path(output)/(mod+"-radar-ui-"+std::to_string(house)+"-"+stage+".png")).string().c_str())!=0)
            throw std::runtime_error("Radar interface preview failed");
    }
    static void verify(const std::string& output,const std::string& mod) {
        const auto require=[](bool ok,const char* why) {
            if(!ok)throw std::runtime_error(std::string("Radar research: ")+why);
        };
        auto options=effectiveGameOptions;options.chaosMode=false;options.immortalHumanPlayer=false;
        options.instantBuild=true;
        auto houses=getFactionSelectionOrder();
        if(mod=="vanilla")houses={HOUSE_HARKONNEN};
        int cases=0;
        for(const int house:houses) {
            const int enemyHouse=house==HOUSE_HARKONNEN?HOUSE_ATREIDES:HOUSE_HARKONNEN;
            GameInitSettings init("radar-research",chaosSmokeMap()+"[Player2]\nCredits=1000000\n",false,options);
            GameInitSettings::HouseInfo human(static_cast<HOUSETYPE>(house),1),enemy(static_cast<HOUSETYPE>(enemyHouse),2);
            human.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));
            enemy.addPlayerInfo(GameInitSettings::PlayerInfo("Radar opponent","qBotEasy"));
            init.addHouseInfo(human);init.addHouseInfo(enemy);
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            auto* owner=pLocalHouse;owner->createUnit(Unit_MCV,true);
            for(const Coord pos:{Coord(1,1),Coord(1,5),Coord(1,9)})
                require(owner->placeStructure(NONE_ID,Structure_WindTrap,pos.x,pos.y,true,true)!=nullptr,"power prerequisite");
            auto* building=owner->placeStructure(NONE_ID,Structure_Radar,5,1,true,true);require(building,"Radar placement");
            auto radarID=building->getObjectID();
            game->techLevel=3;
            require(!owner->startRadarResearch(radar::BaseExpansion),"research below level four");
            verifyButtons(building,output,mod,house,"level-3-hidden");
            for(int level:{4,6}) {
                game->techLevel=level;
                verifyButtons(building,output,mod,house,"level-"+std::to_string(level));
            }
            game->techLevel=7;
            verifyButtons(building,output,mod,house,"level-7-no-ix");
            require(!owner->startRadarResearch(radar::Specialization),"specialization without IX");
            if(mod=="vanilla") {
                verifyButtons(building,output,mod,house,"hidden");
                require(!owner->startRadarResearch(radar::BaseExpansion),"Vanilla acquired Radar research");
                game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                ++cases;continue;
            }
            require(owner->placeStructure(NONE_ID,Structure_IX,8,1,true,true)!=nullptr,"IX placement");
            verifyButtons(building,output,mod,house,"available");
            require(!owner->startRadarResearch(-1) && !owner->startRadarResearch(3),"invalid research index accepted");
            const int funds=owner->getCredits();
            Command forged(255,CMD_RADAR_RESEARCH,radarID,radar::BaseExpansion);forged.executeCommand();
            require(owner->getRadarResearchActive()<0 && owner->getCredits()==funds,"forged research command");
            Command begin(pLocalPlayer->getPlayerID(),CMD_RADAR_RESEARCH,radarID,radar::BaseExpansion);
            OMemoryStream wire;wire.open();begin.save(wire);IMemoryStream received(wire.getData(),wire.getDataLength());Command(received).executeCommand();
            require(owner->getRadarResearchActive()==radar::BaseExpansion && owner->getCredits()==funds-500,"command cost or start");
            verifyButtons(building,output,mod,house,"active");
            require(!owner->startRadarResearch(radar::BaseExpansion) && !owner->startRadarResearch(radar::ExtendedVision),"parallel or duplicate house research");
            const int initial=owner->getRadarResearchRemaining();
            std::vector<WindTrap*> traps;
            for(auto* item:structureList)if(auto* trap=dynamic_cast<WindTrap*>(item)){traps.push_back(trap);trap->setHealth(0);}
            for(int i=0;i<10;++i)owner->updateRadarResearch();
            require(owner->getRadarResearchRemaining()==initial,"research advances without power");
            for(auto* trap:traps)trap->setHealth(trap->getMaxHealth());
            if(cases==0) {
                for(int i=0;i<20;++i)owner->updateRadarResearch();
                const int remaining=owner->getRadarResearchRemaining();
                const auto save=(std::filesystem::path(output)/(mod+"-radar-active.dls")).string();
                require(game->saveGame(save),"active research save");
                game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(save));owner=pLocalHouse;
                require(owner->getRadarResearchActive()==radar::BaseExpansion && owner->getRadarResearchRemaining()==remaining
                    && owner->getCredits()==funds-500,"active research save state or double charge");
            }
            owner->placeStructure(NONE_ID,Structure_Slab1,24,24,true,true);
            require(currentGameMap->getTile(24,24)->getOwner()==owner->getHouseID(),"range anchor slab");
            require(!currentGameMap->isWithinBuildRange(24+BUILDRANGE+2,24,owner),"range already extended before completion");
            while(owner->getRadarResearchActive()>=0)owner->updateRadarResearch();
            require(owner->hasRadarResearch(radar::BaseExpansion) && currentGameMap->isWithinBuildRange(24+BUILDRANGE+2,24,owner),"base expansion not applied");
            auto* trooper=owner->placeUnit(Unit_Trooper,16,16,true);
            auto* tank=owner->placeUnit(Unit_Tank,18,16,true);
            auto* scout=owner->placeUnit(Unit_Quad,20,16,true);
            auto* artillery=owner->placeUnit(Unit_Launcher,22,16,true);
            auto* air=owner->placeUnit(Unit_Ornithopter,18,20,true);
            require(trooper && tank && scout && artillery && air,"stat sample units");
            const int troopSight=trooper->getViewRange(),airSight=air->getViewRange(),artySight=artillery->getViewRange();
            const auto troopSpeed=trooper->getMaxSpeed(),scoutSpeed=scout->getMaxSpeed(),tankSpeed=tank->getMaxSpeed();
            const int tankRange=tank->getWeaponRange(),airHP=air->getMaxHealth();
            air->setHealth(airHP/2);
            require(owner->startRadarResearch(radar::ExtendedVision),"vision start");
            while(owner->getRadarResearchActive()>=0)owner->updateRadarResearch();
            require(trooper->getViewRange()==troopSight+2 && air->getViewRange()==airSight,"common sight or aircraft exclusion");
            const auto faction=getHouseFactionIdentity(static_cast<HOUSETYPE>(owner->getFactionID()));
            BuilderBase* mercFactory=nullptr;int mercItem=ItemID_Invalid,quoted=0;
            if(faction==HOUSE_MERCENARY) {
                mercFactory=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_WOR,12,1,true,true));
                require(mercFactory,"Mercenary WOR");
                for(const auto& item:mercFactory->getBuildList())if(isInfantryUnit(item.itemID)) {mercItem=item.itemID;quoted=item.price;break;}
                require(mercItem!=ItemID_Invalid,"Mercenary infantry offer");mercFactory->doProduceItem(mercItem);
            }
            UnitBase* burnTarget=nullptr;int baselineBurn=0;
            const auto burnLife=[&](bool saveActive) {
                auto* hazard=new Explosion(Explosion_FlameImpact,burnTarget->getCenterPoint(),house,NONE_ID,6,TILESIZE/2);
                game->getExplosionList().push_back(hazard);int elapsed=0,lastHit=-1,hits=0;
                const auto targetID=burnTarget->getObjectID();
                const auto alive=[&]{for(auto* item:game->getExplosionList())if(item==hazard)return true;return false;};
                while(alive()) {
                    require(++elapsed<1000,"flame never expires");const auto hp=burnTarget->getHealth();hazard->update();
                    if(burnTarget->getHealth()<hp) {if(lastHit>=0)require(elapsed-lastHit==18,"research changed persistent fire damage cadence");lastHit=elapsed;++hits;}
                    burnTarget->setHealth(burnTarget->getMaxHealth());
                    if(saveActive && elapsed==110) {
                        require(game->getExplosionList().size()==1,"flame save fixture has other explosions");
                        const auto save=(std::filesystem::path(output)/(mod+"-radar-fire-active.dls")).string();
                        require(game->saveGame(save),"active extended-fire save");
                        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                        game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(save));owner=pLocalHouse;
                        require(game->getExplosionList().size()==1,"saved extended flame lost");hazard=game->getExplosionList().front();
                        burnTarget=dynamic_cast<UnitBase*>(game->getObjectManager().getObject(targetID));require(burnTarget,"saved fire victim lost");
                    }
                }
                require(hits>0,"flame fixture did not apply damage");return elapsed;
            };
            if(faction==HOUSE_KLESHMERSH) {
                burnTarget=game->getHouse(enemyHouse)->placeUnit(Unit_Tank,28,28,true);require(burnTarget,"fire target");baselineBurn=burnLife(false);
            }
            const int beforeSpecial=owner->getCredits();
            require(owner->startRadarResearch(radar::Specialization),"specialization start");
            while(owner->getRadarResearchActive()>=0)owner->updateRadarResearch();
            require(owner->getCredits()==beforeSpecial-750 && !owner->startRadarResearch(radar::Specialization),"special price or repeat purchase");
            const auto identity=getHouseFactionIdentity(static_cast<HOUSETYPE>(owner->getFactionID()));
            verifyButtons(game->getObjectManager().getObject(radarID)?dynamic_cast<StructureBase*>(game->getObjectManager().getObject(radarID)):nullptr,output,mod,house,"complete");
            {
                std::unique_ptr<ObjectInterface> ui(game->getObjectManager().getObject(radarID)->getInterfaceContainer());
                ui->resize(SIDEBARWIDTH,220);require(ui->update(),"Radar interface update");
                SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,20,20,20,255);SDL_RenderClear(renderer);ui->draw(Point(0,0));
                auto picture=renderReadSurface(renderer);
                require(SavePNG(picture.get(),(std::filesystem::path(output)/(mod+"-radar-ui-"+std::to_string(house)+".png")).string().c_str())==0,"Radar interface preview");
            }
            require(artillery->getViewRange()==artySight+(identity==HOUSE_THARPIQUE?4:2),"Tharpique sight must stack");
            require(trooper->getMaxSpeed()==troopSpeed*((identity==HOUSE_ATREIDES || identity==HOUSE_FREMEN)?1.15_fix:1_fix),"Trooper movement specialization");
            require(scout->getMaxSpeed()==scoutSpeed*(identity==HOUSE_ORDOS?1.15_fix:1_fix),"Ordos light movement specialization");
            require(tank->getMaxSpeed()==tankSpeed*(identity==HOUSE_CUSTOM?0.9_fix:1_fix),"Corruptique speed penalty scope");
            require(tank->getWeaponRange()==tankRange+(identity==HOUSE_REBELS?1:0),"Rebels vehicle weapon range");
            require(air->getMaxHealth()==(identity==HOUSE_WILDSPADE?airHP*125/100:airHP),"Wildspade durability");
            require(std::abs(lround(air->getHealth())-air->getMaxHealth()/2)<=1,"existing aircraft damage ratio changed");
            scout->deviate(game->getHouse(enemyHouse));
            require(scout->getMaxSpeed()==scoutSpeed,"deviation retained previous owner's research");
            scout->quitDeviation();
            require(scout->getMaxSpeed()==scoutSpeed*(identity==HOUSE_ORDOS?1.15_fix:1_fix),"reversion lost owner's research");
            if(identity==HOUSE_WILDSPADE) {
                air->deviate(game->getHouse(enemyHouse));require(air->getMaxHealth()==airHP,"deviated aircraft keeps old armour");
                air->quitDeviation();require(air->getMaxHealth()==airHP*125/100,"returned aircraft loses armour");
                auto* future=owner->placeUnit(Unit_Ornithopter,20,20,true);
                require(future && future->getHealth()==airHP*125/100,"future aircraft missed research");
            }
            require(owner->radarAttackDamage(Unit_Tank,100)==(identity==HOUSE_HARKONNEN?120:100)
                && owner->radarAttackDamage(Unit_Devastator,100)==(identity==HOUSE_HARKONNEN?120:100)
                && owner->radarAttackDamage(Structure_Palace,100)==(identity==HOUSE_HARKONNEN?120:100)
                && owner->radarAttackDamage(Unit_SiegeTank,100)==100,"Harkonnen damage scope");
            require(owner->radarUnitPrice(Unit_Trooper,101)==(identity==HOUSE_MERCENARY?90:101)
                && owner->radarUnitPrice(Unit_Tank,101)==101 && owner->radarUnitPrice(Structure_WindTrap,101)==101,"Mercenary non-heavy price scope");
            const auto health=tank->getHealth();tank->handleDamage(20,NONE_ID,owner);
            require(health-tank->getHealth()==(identity==HOUSE_CUSTOM?16:20),"actual tank armour damage");
            if(identity==HOUSE_HARKONNEN) {
                auto* victim=game->getHouse(enemyHouse)->placeUnit(Unit_Tank,18,18,true);require(victim,"combat research target");
                victim->setVisible(VIS_ALL,true);tank->setTarget(victim);tank->primaryWeaponTimer=tank->secondaryWeaponTimer=0;
                const auto count=bulletList.size();require(tank->attack() && bulletList.size()>count,"researched Tank did not fire");
                const int base=game->objectData.data[Unit_Tank][tank->getProductionHouseID()].weapondamage;
                require(FlameAccuracyFixture::directDamage(*bulletList.back())==base*120/100,"actual Tank projectile lacks Harkonnen bonus");
                tank->setTarget(nullptr);
            }
            if(identity==HOUSE_MERCENARY) {
                require(mercFactory->getCurrentProductionPrice()==quoted,"existing production quote changed");
                int newPrice=0;for(const auto& item:mercFactory->getBuildList())if(item.itemID==mercItem)newPrice=item.price;
                require(newPrice==quoted*90/100,"Mercenary factory catalogue price");mercFactory->doProduceItem(mercItem);
                const int startFunds=owner->getCredits();
                for(int tick=0;tick<500 && mercFactory->getProductionQueueSize()==2;++tick)mercFactory->update();
                require(mercFactory->getProductionQueueSize()==1 && owner->getCredits()==startFunds-quoted-lround(mercFactory->getProductionProgress()),"first production quote or charge");
                for(int tick=0;tick<500 && mercFactory->getProductionQueueSize()==1;++tick)mercFactory->update();
                require(mercFactory->getProductionQueueSize()==0 && owner->getCredits()==startFunds-quoted-newPrice,"discounted next production quote or charge");
                auto* star=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_StarPort,26,1,true,true));require(star,"Mercenary Starport");
                owner->getChoam().addItem(Unit_Quad,5);owner->getChoam().update();star->updateBuildList();
                int offers=0;for(const auto& item:star->getBuildList())if(radar::nonHeavyUnit(item.itemID)) {
                    const int raw=owner->getChoam().getPrice(item.itemID,false);
                    require(item.price==std::max(1,raw*90/100) && owner->getChoam().getPrice(item.itemID)==item.price,"Starport discount applied twice or inconsistent quote");++offers;
                }
                require(offers>0,"no Starport discount offers");
            }
            if(identity==HOUSE_NEUTRAL) {
                auto* silo=owner->placeStructure(NONE_ID,Structure_Silo,12,5,true,true);require(silo,"repair silo");
                silo->setHealth(silo->getMaxHealth()/2);const auto before=silo->getHealth();silo->doRepair();
                for(int i=0;i<120;++i)silo->update();
                require(silo->getHealth()==before+(5_fix/30_fix*1.25_fix)*120,"Neutral building repair throughput");
                auto* yard=dynamic_cast<RepairYard*>(owner->placeStructure(NONE_ID,Structure_RepairYard,22,4,true,true));require(yard,"repair yard");
                tank->setHealth(tank->getMaxHealth()/2);const auto hp=tank->getHealth();
                currentGameMap->removeObjectFromMap(tank->getObjectID());tank->setActive(false);
                ObjectPointer cargo;cargo.pointTo(tank->getObjectID());yard->assignUnit(cargo);
                const auto funds=owner->getStartingCredits()+owner->getStoredCredits()+owner->getCityCredits();
                const auto cycle=game->gameCycleCount;
                for(int tick=0;tick<4;++tick){game->gameCycleCount=tick;yard->update();}
                game->gameCycleCount=cycle;
                require(tank->getHealth()==hp+5,"Neutral yard does not repair five HP in four ticks");
                const auto spent=funds-owner->getStartingCredits()-owner->getStoredCredits()-owner->getCityCredits();
                require(spent==UNIT_REPAIRCOST*5,"Neutral repair changed credits per HP");yard->deployRepairUnit();
            }
            if(identity==HOUSE_KLESHMERSH)require(burnLife(true)==baselineBurn+MILLI2CYCLES(2000),"extended flame duration or active save changed");
            if(identity==HOUSE_SARDAUKAR) {
                const int soldiers=owner->getNumItems(Unit_Soldier);
                for(int i=0;i<40;++i) {
                    UnitBase* doomed=nullptr;
                    for(int y=20;y<30 && !doomed;++y)for(int x=10;x<30 && !doomed;++x)
                        if(!currentGameMap->getTile(x,y)->hasAGroundObject())doomed=owner->placeUnit(Unit_Tank,x,y,true);
                    require(doomed,"crew tank placement");doomed->setHealth(0);
                    auto expected=game->randomGen;const bool rescue=expected.rand(1,100)<=35;
                    const int before=owner->getNumItems(Unit_Trooper);doomed->UnitBase::destroy();
                    require(owner->getNumItems(Unit_Trooper)==before+(rescue?1:0),"Sardaukar death roll or squad instead of single Trooper");
                }
                require(owner->getNumItems(Unit_Soldier)==soldiers,"Sardaukar research stacks ordinary Soldier spawns");
            }
            building=game->getObjectManager().getObject(radarID)?dynamic_cast<StructureBase*>(game->getObjectManager().getObject(radarID)):nullptr;
            require(building,"restored Radar");building->setHealth(0);building->destroy();
            require(owner->hasRadarResearch(radar::BaseExpansion) && owner->hasRadarResearch(radar::ExtendedVision)
                && owner->hasRadarResearch(radar::Specialization),"completed research lost with Radar");
            if(cases==0) {
                const auto save=(std::filesystem::path(output)/(mod+"-radar-complete.dls")).string();
                require(game->saveGame(save),"completed research save");game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(save));owner=pLocalHouse;
                for(int kind=0;kind<radar::Count;++kind)require(owner->hasRadarResearch(kind),"completed research not preserved");
            }
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;++cases;
        }
        const auto legacy=(std::filesystem::path(output).parent_path()/"legacy9838"/(mod+"-editor-power.dls")).string();
        if(std::filesystem::exists(legacy)) {
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(legacy));
            require(game->getLoadedSavegameVersion()==9838,"previous-save fixture version");
            require(pLocalHouse->getRadarResearchActive()==-1,"old save has active research");
            for(int kind=0;kind<radar::Count;++kind)require(!pLocalHouse->hasRadarResearch(kind),"old save acquired research");
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        }
        SDL_Log("RADAR RESEARCH PASS: %s, %d houses, real command, prerequisites, progress, bonuses, completed/active and 9838 saves",mod.c_str(),cases);
    }
};
