#pragma once
#include <units/SuperTank.h>
#include <UnitSelection.h>
#include "EditorWindtrapSmoke.h"

struct Release547Fixture {
    static void require(bool ok,const std::string& why) {
        if(!ok) throw std::runtime_error("Release 547: "+why);
    }
    static void reset(std::unique_ptr<Game>& game) {
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    }
    static void selections(const std::string& output,const std::string& mod) {
        for(bool shared:{false,true}) {
            auto options=effectiveGameOptions;options.chaosMode=false;options.alliedControl=shared;
            const auto roster=getFactionSelectionOrder();
            GameInitSettings init("547-selection",chaosSmokeMap()+"[Player2]\nCredits=1000000\n[Player3]\nCredits=1000000\n",false,options);
            for(int i=0;i<3;++i) {
                GameInitSettings::HouseInfo info(static_cast<HOUSETYPE>(roster[i]),i==2?2:1);
                info.addPlayerInfo(GameInitSettings::PlayerInfo(i==0?settings.general.playerName:"Selection "+std::to_string(i),i==0?HUMANPLAYERCLASS:"qBotEasy"));
                init.addHouseInfo(info);
            }
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            for(int i=0;i<3;++i) {
                auto* owner=game->getHouse(roster[i]);owner->createUnit(Unit_MCV,true);
                int row=3;
                for(int item:{Unit_Tank,Unit_Soldier,Unit_Ornithopter,Unit_Harvester,Unit_RebelHarvester,Unit_CapacityHarvester,Unit_FastHarvester,Unit_Carryall,Unit_ChemicalCarryall,Unit_MCV})
                    require(owner->placeUnit(item,3+i*7,row++*2,true),"selection fixture unit");
            }
            auto* inactive=pLocalHouse->placeUnit(Unit_Harvester,3,25,true);inactive->setRespondable(false);
            const size_t groups=shared?2:1;
            {
                GameInterface ui;ui.updateObjectInterface();
                require(ui.armySelectButton.getText()==_("Army")+" ("+std::to_string(groups*3)+")","army count");
                require(ui.harvestersSelectButton.getText()==_("Harvesters")+" ("+std::to_string(groups*4)+")","four Harvester variants count");
                SDL_RenderSetClipRect(renderer,nullptr);SDL_RenderClear(renderer);ui.draw(Point(0,0));
                SavePNG(renderReadSurface(renderer).get(),(std::filesystem::path(output)/(mod+"-547-selection-"+(shared?"allied":"own")+".png")).string().c_str());
                ui.armySelectButton.handleMouseLeft(5,5,true);ui.armySelectButton.handleMouseLeft(5,5,false);
                require(game->getSelectedList().size()==groups*3,"army click ownership or noncombat filter");
                for(auto id:game->getSelectedList())require(isArmySelectionItem(game->getObjectManager().getObject(id)->getItemID()),"army includes utility unit");
                ui.updateObjectInterface();require(!ui.armySelectButton.isVisible() && !ui.harvestersSelectButton.isVisible(),"selection buttons overlap object controls");
                game->selectAllHarvesters();require(game->getSelectedList().size()==groups*4,"Harvester selection ownership/variants/respondable");
                for(auto id:game->getSelectedList())require(isHarvesterSelectionItem(game->getObjectManager().getObject(id)->getItemID()),"Harvester selection includes combat unit");
                game->unselectAll(game->getSelectedList());game->getSelectedList().clear();ui.updateObjectInterface();
                require(ui.armySelectButton.isVisible() && ui.harvestersSelectButton.isVisible(),"buttons do not return after deselection");
            }
            // Keep the real selection observer alive during bulk unit teardown.
            CaptureHuntGameFixture::start(*game);
            game->selectAllArmy();
            require(!game->getSelectedList().empty(),"selected-army teardown fixture");
            reset(game);
        }
    }
    static void superTank(const std::string& output,const std::string& mod) {
        if(mod!="Tornie" && mod!="Jericho" && mod!="ArcaKev") return;
        auto options=effectiveGameOptions;options.chaosMode=false;options.instantBuild=true;
        auto game=std::make_unique<Game>();currentGame=game.get();
        const auto house=getRuntimeHouseForIdentity(HOUSE_KLESHMERSH);
        GameInitSettings init("547-kleshmersh",chaosSmokeMap()+"[UNITS]\nID000=Player1,Elite Siege Tank,256,336,0,Area Guard\n[REINFORCEMENTS]\n1=Player1,Elite Siege Tank,Homebase,1\n",false,options);
        GameInitSettings::HouseInfo info(house,1);
        info.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));init.addHouseInfo(info);
        game->initGame(init);
        auto* owner=pLocalHouse;owner->createUnit(Unit_MCV,true);
        bool mapped=false;
        for(auto* unit:unitList) {
            require(unit->getItemID()!=Unit_EliteSiegeTank,"Kleshmersh scenario keeps an Elite Siege Tank");
            mapped|=unit->getItemID()==Unit_SuperTank;
        }
        require(mapped,"scenario Super Tank replacement");
        bool reinforcement=false;
        for(const auto& trigger:game->getTriggerManager().getTriggers())if(auto* delivery=dynamic_cast<ReinforcementTrigger*>(trigger.get()))
            for(int item:delivery->getDroppedUnits()){require(item!=Unit_EliteSiegeTank,"Kleshmersh reinforcement keeps an Elite Siege Tank");reinforcement|=item==Unit_SuperTank;}
        require(reinforcement,"Super Tank reinforcement replacement");
        require(game->objectData.data[Unit_SuperTank][house].price==925,"Super Tank price");
        require(game->objectData.data[Unit_SuperTank][house].hitpoints==game->objectData.data[Unit_SiegeTank][house].hitpoints,"Super Tank Siege Tank health");
        require(game->objectData.data[Unit_EliteSiegeTank][house].builder==ItemID_Invalid,"Kleshmersh still produces Elite Siege Tank");
        for(int h=0;h<NUM_HOUSES;++h) if(h!=house)
            require(game->objectData.data[Unit_SuperTank][h].builder==ItemID_Invalid,"Super Tank replaces another faction's production");
        int column=0;
        for(int item:{Structure_WindTrap,Structure_Radar,Structure_LightFactory,Structure_Refinery,Structure_StarPort,Structure_IX})
            require(owner->placeStructure(NONE_ID,item,1+column++*5,1,true,true),"Super Tank prerequisite");
        auto* heavy=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_HeavyFactory,5,15,true,true));
        require(heavy,"heavy factory");
        while(heavy->isAllowedToUpgrade()){require(heavy->doUpgrade(),"heavy upgrade");heavy->update();}
        heavy->updateBuildList();
        require(heavy->isAvailableToBuild(Unit_SuperTank) && !heavy->isAvailableToBuild(Unit_EliteSiegeTank),"Kleshmersh factory replacement");
        heavy->doProduceItem(Unit_SuperTank);require(heavy->getCurrentProducedItem()==Unit_SuperTank,"Super Tank production order");
        auto* tank=dynamic_cast<SuperTank*>(owner->placeUnit(Unit_SuperTank,15,16,true));
        require(tank && tank->getMaxHealth()==300 && tank->bulletType==Bullet_Flame,"new unit identity, health or weapon");
        auto* target=owner->placeUnit(Unit_SiegeTank,15,18,true);require(target,"fire target");
        tank->setTarget(target);const auto bullets=bulletList.size();require(tank->attack(),"first fire rocket");
        require(bulletList.size()==bullets+1 && bulletList.back()->getBulletID()==Bullet_Flame,"first projectile is not fire");
        tank->secondaryWeaponTimer=0;require(tank->attack(),"second fire rocket");
        require(bulletList.size()==bullets+2 && bulletList.back()->getBulletID()==Bullet_Flame,"Launcher-style second fire projectile");
        for(int zoom=0;zoom<3;++zoom) {
            int w=0,h=0;auto* turret=pGFXManager->getZoomedObjPic(ObjPic_SuperTankGun,house,zoom);
            require(SDL_QueryTexture(turret,nullptr,nullptr,&w,&h)==0 && w==128*(zoom+1) && h==16*(zoom+1),"Super Tank turret zoom");
            require(pGFXManager->getUIGraphic(UI_MapEditor_SuperTank,house),"Super Tank editor icon");
        }
        require(pGFXManager->getSmallDetailPic(Picture_SuperTank),"Super Tank portrait");
        const auto saved=(std::filesystem::path(output)/(mod+"-547-super.dls")).string();
        const auto id=tank->getObjectID();require(game->saveGame(saved),"Super Tank save");reset(game);
        game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(saved));
        tank=dynamic_cast<SuperTank*>(game->getObjectManager().getObject(id));require(tank && tank->getMaxHealth()==300,"saved Super Tank restored as a different unit");
        auto* neighbour=pLocalHouse->placeUnit(Unit_SiegeTank,16,16,true);require(neighbour,"blast target");
        const auto health=neighbour->getHealth();
        const auto explosions=game->getExplosionList().size();
        tank->setVisible(pLocalHouse->getTeamID(),true);tank->destroy();
        require(game->getExplosionList().size()>=explosions+9 && neighbour->getHealth()<health,"Devastator-style damaging destruction");
        reset(game);
    }
    static void windtraps(const std::string& output,const std::string& mod) {
        const auto roster=getFactionSelectionOrder();
        for(int house:roster) for(bool chaos:{false,true}) {
            auto options=effectiveGameOptions;options.chaosMode=chaos;
            const std::string map=chaosSmokeMap()+"[STRUCTURES]\nID000=Player1,Windtrap,256,132\n[WINDTRAPUPGRADES]\nID000=true\n";
            GameInitSettings init("547-editor-power",map,false,options);
            GameInitSettings::HouseInfo info(static_cast<HOUSETYPE>(house),1);
            info.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));init.addHouseInfo(info);
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            WindTrap* trap=nullptr;for(auto* item:structureList)if(auto* wind=dynamic_cast<WindTrap*>(item))trap=wind;
            require(trap && trap->isPowerUpgraded()==(mod!="vanilla"),"editor flag requires Rebels/Chaos or leaks to Vanilla");
            const int base=std::abs(game->objectData.data[Structure_WindTrap][pLocalHouse->getFactionID()].power);
            require(pLocalHouse->getProducedPower()==base*(mod=="vanilla"?1:2),"editor initial power for faction");
            reset(game);
        }
        EditorWindtrapFixture::verify(output,mod);
    }
    static void run(const char* directory) {
        require(directory,"missing smoke directory");const std::string output=directory;
        auto& mods=ModManager::instance();const auto previous=mods.getActiveModName();
        for(const std::string mod:{"Tornie","TornieLite","Jericho","JerichoLite","ArcaKev","ArcaKevLite","vanilla"}) {
            require(mods.setActiveMod(mod),"mod activation");effectiveGameOptions=mods.loadEffectiveGameOptions(settings.gameOptions);
            selections(output,mod);superTank(output,mod);windtraps(output,mod);
            SDL_Log("RELEASE 547 MOD PASS: %s selections, Super Tank, editor Windtraps",mod.c_str());
        }
        mods.setActiveMod(previous);
        SDL_Log("RELEASE 547 PASS: seven modes, army/Harvester selection and allied control; Kleshmersh Super Tank 925, fire salvos, blast, production and saves; all-faction editor Windtraps with/without Chaos");
    }
};
