#pragma once

#include <Bullet.h>
#include <units/MCV.h>
#include <units/Trooper.h>
#include <structures/Scoutpost.h>
#include <GUI/dune/StructureSizePreview.h>
#include <fstream>
#include <misc/OFileStream.h>
#include "EditorTeamsSmoke.h"

inline void verifyRelease541Rules(const std::string& output,const std::string& mod) {
    auto require=[](bool ok,const std::string& message) {
        if(!ok) throw std::runtime_error("Release 541 rules: "+message);
    };
    Release541EditorFixture::verify(output,mod);
    const auto roster=getFactionSelectionOrder();
    auto options=effectiveGameOptions;options.chaosMode=false;options.instantBuild=true;
    std::string data="[BASIC]\nVersion=2\nTechLevel=9\n[MAP]\nSizeX=64\nSizeY=48\n";
    for(int y=0;y<48;++y) data+=fmt::sprintf("%03d=",y)+std::string(64,'%')+"\n";
    for(std::size_t i=0;i<roster.size();++i) data+="[Player"+std::to_string(i+1)+"]\nCredits=1000000\n";
    GameInitSettings init("release541-rules",data,false,options);
    for(std::size_t i=0;i<roster.size();++i) {
        GameInitSettings::HouseInfo house(static_cast<HOUSETYPE>(roster[i]),i==0 ? 1 : 2);
        house.addPlayerInfo(GameInitSettings::PlayerInfo(i==0 ? settings.general.playerName : "Rules AI "+std::to_string(i),
            i==0 ? HUMANPLAYERCLASS : DEFAULTAIPLAYERCLASS));init.addHouseInfo(house);
    }
    auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
    const auto runtime=[](int identity){return getRuntimeHouseForIdentity(static_cast<HOUSETYPE>(identity));};
    if(mod!="vanilla") {
        for(const int identity:{HOUSE_WILDSPADE,HOUSE_SARDAUKAR})
            require(game->objectData.data[Structure_LightFactory][runtime(identity)].techLevel==2,"Light Factory level two");
        for(const int identity:{HOUSE_ORDOS,HOUSE_NEUTRAL}) {
            const auto& siege=game->objectData.data[Unit_SiegeTank][runtime(identity)];
            require(siege.techLevel==6 && siege.upgradeLevel==3,"Ordos/Neutral Siege Tank third upgrade at six");
        }
        for(int faction=0;faction<NUM_HOUSES;++faction) {
            require(game->objectData.data[Unit_SonicTrike][faction].techLevel!=2,"Sonic Trike still unlocks at two");
            for(int item:{Structure_Scoutpost,Structure_Flamepost,Structure_Chemipost})
                require(game->objectData.data[item][faction].weaponrange==7,"post range is not seven tiles");
            require(game->objectData.data[Structure_Flamepost][faction].weapondamage==12,"Flamepost damage is not twelve");
        }
    }
    // Export the actual engine's Palace choice and recharge for the FR/EN site.
    std::ofstream powers(std::filesystem::path(output)/(mod+"-palace-powers.json"));
    powers << "[";
    for(std::size_t i=0;i<roster.size();++i) {
        auto* owner=game->getHouse(roster[i]);
        auto* palace=dynamic_cast<Palace*>(owner->placeStructure(NONE_ID,Structure_Palace,1+7*(i%6),1+8*(i/6),true,true));
        require(palace!=nullptr,"Palace fixture placement");
        std::string power;
        if(palace->usesTornieMainRebelsRandomSpecial()) power=mod=="vanilla" ? "random-classic" : "random-expanded";
        else if(palace->usesJerichoOrnithopterStrike()) power="ornithopters";
        else if(palace->usesJerichoKleshmershFremenCall()) power="fremen";
        else if(palace->usesLightVehicleCall()) power="light-vehicles";
        else {
            const int faction=getHouseFallbackHouse(static_cast<HOUSETYPE>(palace->getTechnologyHouseID()));
            power=faction==HOUSE_HARKONNEN || faction==HOUSE_SARDAUKAR ? "death-hand"
                : faction==HOUSE_ATREIDES || faction==HOUSE_FREMEN ? "fremen" : "saboteur";
        }
        const int identity=getHouseFactionIdentity(static_cast<HOUSETYPE>(roster[i]));
        const int expectedMinutes=identity==HOUSE_REBELS || (mod=="vanilla" && (identity==HOUSE_NEUTRAL || identity==HOUSE_KLESHMERSH)) ? 450
            : identity==HOUSE_HARKONNEN || identity==HOUSE_SARDAUKAR || identity==HOUSE_WILDSPADE ? 600 : 300;
        require(palace->getMaxSpecialWeaponTimer()==MILLI2CYCLES(expectedMinutes*1000),"Palace recharge duration");
        powers << (i ? "," : "") << "{\"house\":\"" << getHouseNameByNumber(static_cast<HOUSETYPE>(roster[i]))
            << "\",\"power\":\"" << power << "\",\"seconds\":" << expectedMinutes << "}";
    }
    powers << "]\n";powers.close();
    require(powers.good(),"Palace export write");
    if(mod!="vanilla") {
        const int ids[]={Structure_Scoutpost,Structure_Flamepost,Structure_Chemipost};
        const int bullets[]={Bullet_SmallRocket,Bullet_Flame,Bullet_Heal};
        for(int i=0;i<3;++i) {
            auto* owner=game->getHouse(roster[0]);
            if(roster.size()==12 && i>0) owner=game->getHouse(runtime(i==1 ? HOUSE_KLESHMERSH : HOUSE_THARPIQUE));
            const bool upgrade=roster.size()==12 && i>0;
            if(upgrade) require(owner->placeStructure(NONE_ID,Structure_IX,42+i*4,20,true,true)!=nullptr,"post upgrade IX");
            auto* post=dynamic_cast<Scoutpost*>(owner->placeStructure(NONE_ID,upgrade ? Structure_Scoutpost : ids[i],3,28+i*5,true,true));
            require(post!=nullptr,"post fixture");
            if(upgrade) {
                if(i==1) { require(post->canUpgradeToFlamepost(),"Flamepost upgrade unavailable");post->doUpgradeToFlamepost(); }
                else { require(post->canUpgradeToChemipost(),"Chemipost upgrade unavailable");post->doUpgradeToChemipost(); }
            }
            require(post->getItemID()==static_cast<unsigned>(ids[i]) && post->getWeaponRange()==7,"upgraded post range/type");
            auto* targetOwner=i==2 ? owner : game->getHouse(roster[1]);
            auto* target=targetOwner->placeUnit(Unit_Tank,10,28+i*5,true);
            require(target!=nullptr,"post target placement");target->doSetAttackMode(STOP);target->setVisible(-1,true);
            if(i==2) target->setHealth(target->getMaxHealth()/2);
            const auto before=bulletList.size();post->doAttackObject(target);
            for(int tick=0;tick<1000 && bulletList.size()==before;++tick) post->update();
            require(bulletList.size()>before && bulletList.back()->getBulletID()==bullets[i],"post did not launch the expected missile at seven tiles");
        }
    }
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    if(!roster.empty()) {
        auto flameOptions=options;flameOptions.chaosMode=mod!="vanilla";
        GameInitSettings flameInit("release541-flame",data,false,flameOptions);
        for(const auto& info:init.getHouseInfoList()) flameInit.addHouseInfo(info);
        const int recipient=mod!="vanilla" ? runtime(HOUSE_ORDOS) : runtime(HOUSE_KLESHMERSH);
        const int kleshmersh=runtime(HOUSE_KLESHMERSH);
        if(mod!="vanilla") {
            ObjectData base;base.loadFromINIFile(ModManager::instance().getActiveObjectDataPath(),false);
            bool found=false;
            for(Uint32 seed=1;seed<=1000 && !found;++seed) {
                auto candidate=std::make_unique<ObjectData>(base);ChaosMode draw;
                draw.generate(*candidate,true,seed);
                if(draw.getTechnologyHouse(recipient,Structure_WOR)==kleshmersh) {
                    flameInit.setRandomSeed(seed);found=true;
                }
            }
            require(found,"no borrowed Kleshmersh WOR test seed");
        }
        game=std::make_unique<Game>();currentGame=game.get();game->initGame(flameInit);
        auto* owner=game->getHouse(recipient);
        if(mod!="vanilla") {
            auto* wor=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_WOR,3,3,true,true));
            require(wor && wor->getTechnologyHouseID()==kleshmersh,"Chaos WOR lost its donor");
            owner->placeStructure(NONE_ID,Structure_WindTrap,8,3,true,true);
            owner->placeStructure(NONE_ID,Structure_Radar,12,3,true,true);
            while(wor->isAllowedToUpgrade()) { require(wor->doUpgrade(),"borrowed WOR upgrade");wor->update(); }
            wor->updateBuildList();
            const auto product=std::find_if(wor->getBuildList().begin(),wor->getBuildList().end(),
                [](const BuildItem& item){return item.itemID==Unit_Trooper;});
            require(product!=wor->getBuildList().end() && product->price==200 && product->price==game->objectData.data[Unit_Trooper][kleshmersh].price,
                "borrowed WOR Trooper price differs from Kleshmersh");
            wor->doProduceItem(Unit_Trooper);
            for(int tick=0;tick<1000 && owner->getNumItems(Unit_Trooper)==0;++tick) wor->update();
        }
        UnitBase* trooper=nullptr;
        if(mod=="vanilla") trooper=owner->placeUnit(Unit_Trooper,4,10,true);
        else for(auto* unit:unitList) if(unit->getOwner()==owner && unit->getItemID()==Unit_Trooper) {trooper=unit;break;}
        require(trooper && trooper->getProductionHouseID()==kleshmersh,"WOR product donor inheritance");
        auto* targetOwner=game->getHouse(runtime(HOUSE_HARKONNEN));
        const auto position=trooper->getLocation();
        auto* target=targetOwner->placeUnit(Unit_Tank,position.x+1,position.y,true);
        require(target!=nullptr,"Trooper point-blank target");target->setVisible(-1,true);target->doSetAttackMode(STOP);
        const auto before=bulletList.size();trooper->doAttackObject(target,false);
        for(int tick=0;tick<1000 && bulletList.size()==before;++tick) trooper->update();
        require(bulletList.size()>before,"Trooper never fired");
        require((bulletList.back()->getBulletID()==Bullet_Flame)==(mod!="vanilla"),
            "Vanilla Kleshmersh / Chaos borrowed WOR flame policy");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    }
    // The old 19x19 atlas must cover all five columns without stretching.
    SDL_SetRenderDrawColor(renderer,48,48,48,255);SDL_RenderClear(renderer);
    drawStructureSizePreview(renderer,pGFXManager->getUIGraphic(UI_StructureSizeLattice),
        pGFXManager->getUIGraphic(UI_StructureSizeConcrete),4,4,5,2);
    auto pixels=renderReadSurface(renderer);require(pixels!=nullptr,"footprint capture");
    for(int y=0;y<19;++y) for(int x=0;x<6;++x)
        require(getPixel(pixels.get(),4+x,4+y)==getPixel(pixels.get(),28+x,4+y),"Doublefinery fifth preview column is clipped");
    require(SavePNG(pixels.get(),(std::filesystem::path(output)/(mod+"-doublefinery-footprint.png")).string().c_str())==0,"footprint output");
    // Exercise the actual campaign/skirmish initialization, not just the level helper.
    GameInitSettings campaignStart(static_cast<HOUSETYPE>(roster[0]),options);
    for(auto finalInit:{GameInitSettings(static_cast<HOUSETYPE>(roster[0]),22,options),
            GameInitSettings(campaignStart,22,0,0)}) {
        finalInit.clearHouseInfo();
        for(const auto& house:init.getHouseInfoList()) finalInit.addHouseInfo(house);
        const auto replayPath=(std::filesystem::path(output)/(mod+"-final-"+std::to_string(static_cast<int>(finalInit.getGameType()))+".rpl")).string();
        { OFileStream replay;require(replay.open(replayPath),"final mission replay setup");
          replay.writeString(settings.general.playerName);finalInit.save(replay); }
        game=std::make_unique<Game>();currentGame=game.get();game->initReplay(replayPath);
        require(game->techLevel==9,"scenario 22 did not unlock technology nine");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    }
    SDL_Log("RELEASE 541 RULES PASS: %s, technology, seven-tile missiles/upgrades, Flamepost damage, Palace powers and 5x2 preview",mod.c_str());
}
