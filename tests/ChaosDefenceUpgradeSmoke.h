#pragma once
#include <structures/Scoutpost.h>
#include <structures/WindTrap.h>

inline void verifyChaosDefenceUpgrades(const std::string& output,const std::string& mod) {
    const auto require=[](bool ok,const char* why){if(!ok)throw std::runtime_error(std::string("Chaos defence upgrades: ")+why);};
    if(mod=="vanilla") {SDL_Log("CHAOS DEFENCE UPGRADE PASS: Vanilla excluded");return;}
    auto options=effectiveGameOptions;options.chaosMode=true;options.sellingBuildings=true;
    const auto recipient=getRuntimeHouseForIdentity(HOUSE_ATREIDES);
    ObjectData base;base.loadFromINIFile(ModManager::instance().getActiveObjectDataPath(),false);
    for(const auto identity:{HOUSE_KLESHMERSH,HOUSE_THARPIQUE,HOUSE_NEUTRAL,HOUSE_REBELS}) {
        const int type=identity==HOUSE_REBELS?Structure_WindTrap:Structure_Scoutpost;
        Uint32 seed=0;int donor=-1;
        for(;seed<10000;++seed) {
            auto data=base;ChaosMode exchange;exchange.generate(data,true,seed);
            const auto candidate=exchange.getTechnologyHouse(recipient,type);
            if(isHouseFaction(static_cast<HOUSETYPE>(candidate),identity)){donor=candidate;break;}
        }
        require(donor>=0 && donor!=recipient,"no actual borrowed upgrade plan");
        auto init=chaosSmokeInit(recipient,options);init.setRandomSeed(seed);
        auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
        auto* owner=pLocalHouse;owner->createUnit(Unit_MCV,true);
        require(owner->placeStructure(NONE_ID,Structure_IX,12,1,true,true),"IX prerequisite");
        auto* object=owner->placeStructure(NONE_ID,type,4,1,true,true);require(object,"upgrade building");
        require(object->getTechnologyHouseID()==donor,"seed changed during actual game setup");
        const auto id=object->getObjectID();const auto colour=getHouseVisualHouse(recipient);
        int price=0,converted=type;
        if(auto* wind=dynamic_cast<WindTrap*>(object)) {
            price=150;const auto power=owner->getProducedPower();const int credits=owner->getCredits();
            require(wind->doPowerUpgrade() && wind->isPowerUpgraded(),"borrowed Rebels power upgrade");
            require(owner->getProducedPower()==2*power && owner->getCredits()==credits-price,"borrowed power or price");
            require(!wind->doPowerUpgrade(),"duplicate power upgrade");
        } else {
            auto* post=dynamic_cast<Scoutpost*>(object);require(post,"Scoutpost type");
            const int credits=owner->getCredits();
            if(identity==HOUSE_KLESHMERSH) {
                require(post->canUpgradeToFlamepost(),"borrowed Flamepost plan");price=post->getFlamepostUpgradeCost();
                post->doUpgradeToFlamepost();converted=Structure_Flamepost;
            } else if(identity==HOUSE_THARPIQUE) {
                require(post->canUpgradeToChemipost(),"borrowed Chemipost plan");price=post->getChemipostUpgradeCost();
                post->doUpgradeToChemipost();converted=Structure_Chemipost;
            } else {
                require(post->canUpgradeToSlowpost(),"borrowed Slowpost plan");price=post->getSlowpostUpgradeCost();
                post->doUpgradeToSlowpost();converted=Structure_Slowpost;
            }
            require(post->getItemID()==converted && owner->getCredits()==credits-price,"borrowed conversion or price");
        }
        require(object->getTechnologyHouseID()==donor && getHouseVisualHouse(recipient)==colour,"conversion changed donor or faction colour");
        const auto refund=object->getSaleRefund();
        const auto path=(std::filesystem::path(output)/(mod+"-chaos-defence-"+std::to_string(identity)+".dls")).string();
        require(game->saveGame(path),"borrowed upgrade save");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(path));
        object=dynamic_cast<StructureBase*>(game->getObjectManager().getObject(id));
        require(object && object->getItemID()==converted && object->getTechnologyHouseID()==donor && object->getSaleRefund()==refund,"borrowed conversion/donor/refund save state");
        if(auto* wind=dynamic_cast<WindTrap*>(object))require(wind->isPowerUpgraded(),"borrowed power save state");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    }
    SDL_Log("CHAOS DEFENCE UPGRADE PASS: %s, three borrowed Scoutpost conversions, 2x2 Windtrap power, colours and saved plans",mod.c_str());
}
