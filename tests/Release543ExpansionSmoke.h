#pragma once
#include <units/Quad.h>
#include <units/Tank.h>
#include <units/Ornithopter.h>
#include <units/SandWorm.h>
#include <structures/Worfinery.h>
#include <structures/GunTurret.h>
#include <misc/OFileStream.h>
#include <Explosion.h>
#include "PalaceNapalmSmoke.h"

inline void verifyRelease543Expansion(const std::string& output,const std::string& mod) {
    verifyRandomPalaceNapalm(output,mod);
    const auto require=[](bool ok,const std::string& why){if(!ok)throw std::runtime_error("Release 543 expansion: "+why);};
    auto options=effectiveGameOptions;options.chaosMode=false;options.instantBuild=true;
    const auto roster=getFactionSelectionOrder();
    const auto runtime=[](int identity){return getRuntimeHouseForIdentity(static_cast<HOUSETYPE>(identity));};
    std::string map="[BASIC]\nVersion=2\nTechLevel=9\n[MAP]\nSizeX=64\nSizeY=48\n";
    for(int y=0;y<48;++y)map+=fmt::sprintf("%03d=",y)+std::string(64,'%')+"\n";
    for(std::size_t i=0;i<roster.size();++i)map+="[Player"+std::to_string(i+1)+"]\nCredits=1000000\n";
    GameInitSettings init("expansion-543",map,false,options);
    for(std::size_t i=0;i<roster.size();++i){
        GameInitSettings::HouseInfo info(static_cast<HOUSETYPE>(roster[i]),i==0?1:2);
        info.addPlayerInfo(GameInitSettings::PlayerInfo(i==0?settings.general.playerName:"Expansion AI "+std::to_string(i),i==0?HUMANPLAYERCLASS:DEFAULTAIPLAYERCLASS));init.addHouseInfo(info);
    }
    auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
    for(int faction:roster)game->getHouse(faction)->createUnit(Unit_MCV,true);
    const auto available=[](BuilderBase* b,int item){b->updateBuildList();return b->isAvailableToBuild(item);};
    if(mod!="vanilla") {
        struct Product {int faction,item,builder,level,upgrade;bool ix;};
        const Product products[]={
            {HOUSE_ORDOS,Unit_RaiderQuad,Structure_LightFactory,4,1,false},{HOUSE_SARDAUKAR,Unit_RaiderQuad,Structure_LightFactory,4,1,false},
            {HOUSE_ATREIDES,Unit_RangedTank,Structure_HeavyFactory,5,2,false},{HOUSE_FREMEN,Unit_RangedTank,Structure_HeavyFactory,5,2,false},
            {HOUSE_CUSTOM,Unit_SlowTank,Structure_HeavyFactory,7,0,true},{HOUSE_NEUTRAL,Structure_Slowpost,Structure_ConstructionYard,7,0,true},
            {HOUSE_ATREIDES,Unit_FastHarvester,Structure_HeavyFactory,7,0,true},{HOUSE_KLESHMERSH,Unit_FastHarvester,Structure_HeavyFactory,7,0,true},
            {HOUSE_WILDSPADE,Unit_CapacityHarvester,Structure_HeavyFactory,7,0,true},{HOUSE_MERCENARY,Unit_CapacityHarvester,Structure_HeavyFactory,7,0,true},
            {HOUSE_REBELS,Unit_RebelHarvester,Structure_HeavyFactory,7,0,true},
            {HOUSE_CUSTOM,Unit_RebelHarvester,Structure_HeavyFactory,7,0,true},{HOUSE_WILDSPADE,Unit_EliteOrnithopter,Structure_HighTechFactory,8,3,true}};
        for(const auto& product:products) {
            auto* owner=game->getHouse(runtime(product.faction));if(!owner)continue;
            const int index=std::find(roster.begin(),roster.end(),owner->getHouseID())-roster.begin(),x=2+5*index;
            auto* builder=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,product.builder,x,20,true,true));require(builder,"production building");
            game->techLevel=product.level-1;require(!available(builder,product.item),"unit unlocked below its level");
            game->techLevel=product.level;
            for(int upgrade=0;upgrade<product.upgrade;++upgrade){require(!available(builder,product.item),"unit unlocked before its upgrade");require(builder->doUpgrade(),"required upgrade refused");builder->update();}
            if(product.ix && owner->getNumItems(Structure_IX)==0){require(!available(builder,product.item),"unit unlocked without IX");require(owner->placeStructure(NONE_ID,Structure_IX,x,26,true,true),"IX placement");}
            require(available(builder,product.item),"configured unit missing: "+getItemNameByID(product.item));
            if(product.faction==HOUSE_KLESHMERSH && product.item==Unit_FastHarvester)
                require(!available(builder,Unit_RebelHarvester),"Kleshmersh still produces Harvestank outside Chaos");
            builder->doProduceItem(product.item);require(builder->getCurrentProducedItem()==static_cast<Uint32>(product.item),"production order refused");builder->doCancelItem(product.item);
            if(isUnit(product.item)) {
                auto* instance=owner->createUnit(product.item,true);require(instance && instance->getItemID()==static_cast<Uint32>(product.item),"unit factory identity");
                instance->destroy();
            }
            if(product.item==Unit_EliteOrnithopter){auto* elite=owner->createUnit(Unit_EliteOrnithopter,true);require(dynamic_cast<Ornithopter*>(elite) && elite->getMaxHealth()==32,"elite aircraft identity/stats");}
            builder->destroy();
        }
        auto* owner=pLocalHouse;
        auto* capacity=dynamic_cast<Harvester*>(owner->placeUnit(Unit_CapacityHarvester,4,36,true));
        auto* fast=dynamic_cast<Harvester*>(owner->placeUnit(Unit_FastHarvester,7,36,true));
        require(capacity && fast && capacity->getSpiceCapacity()==1050 && fast->getSpiceCapacity()==700,"harvester capacities");
        capacity->setAmountOfSpice(1000);require(harvesterGetAmountOfSpice(capacity)==1000,"large cargo truncated to vanilla limit");
        require(game->objectData.data[Unit_FastHarvester][owner->getFactionID()].maxspeed==3.2_fix,"fast harvester speed");
        auto* normal=owner->placeUnit(Unit_Tank,10,36,true);require(normal,"slow target");normal->doSetAttackMode(STOP);
        auto* slow=owner->placeUnit(Unit_SlowTank,11,36,true);require(slow,"slow shooter");
        auto start=slow->getCenterPoint(),impact=normal->getCenterPoint();const auto health=normal->getHealth();
        auto* missile=new Bullet(slow->getObjectID(),&start,&impact,Bullet_Slow,0,false,normal);bulletList.push_back(missile);
        for(int tick=0;tick<300 && normal->getSlowTimer()==0;++tick)missile->update();
        require(normal->getSlowTimer()>0 && normal->getMovementSpeedFactor()==0.5_fix && normal->getHealth()==health,"slow missile did not slow an allied target without damage");
        auto* aircraft=owner->placeUnit(Unit_Carryall,19,36,true);require(aircraft,"aircraft slow target");aircraft->applySlow();
        require(aircraft->getSlowTimer()>0 && aircraft->getMovementSpeedFactor()==0.5_fix,"aircraft hit by slowing cannot be slowed");
        for(int faction:{HOUSE_KLESHMERSH,HOUSE_REBELS})if(auto* house=game->getHouse(runtime(faction))){
            auto* target=house->placeUnit(Unit_Tank,14+(faction==HOUSE_REBELS?3:0),36,true);target->applySlow();
            require(target->getMovementSpeedFactor()==(faction==HOUSE_KLESHMERSH?1.0_fix:0.25_fix),"faction slow resistance");
            require(target->getSlowTimer()==(faction==HOUSE_KLESHMERSH?0:MILLI2CYCLES(8000)),"faction slow duration");
        }
        for(int y=35;y<43;++y)for(int x=22;x<33;++x)currentGameMap->getTile(x,y)->setType(Terrain_Sand);
        auto* worm=dynamic_cast<Sandworm*>(owner->placeUnit(Unit_TameSandworm,24,38,true));
        auto* friendUnit=owner->placeUnit(Unit_Tank,25,38,true);require(worm && friendUnit,"tame worm placement");
        require(worm->isRespondable() && !worm->canAttack(friendUnit),"tame worm attacks allies or refuses orders");
        const auto capacityID=capacity->getObjectID(),normalID=normal->getObjectID(),wormID=worm->getObjectID();const int timer=normal->getSlowTimer();
        const auto save=(std::filesystem::path(output)/(mod+"-expansion.dls")).string();require(game->saveGame(save),"expansion save");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(save));
        require(game->getObjectManager().getObject(capacityID)->getItemID()==Unit_CapacityHarvester && harvesterGetAmountOfSpice(game->getObjectManager().getObject(capacityID))==1000,"cargo/variant lost after load");
        auto* restored=static_cast<UnitBase*>(game->getObjectManager().getObject(normalID));require(restored && restored->getSlowTimer()==timer,"slow timer lost after load");
        require(game->getObjectManager().getObject(wormID)->getItemID()==Unit_TameSandworm,"tame worm identity lost");
        restored->doSetAttackMode(STOP);for(int tick=0;tick<timer;++tick)restored->update();require(restored->getSlowTimer()==0 && restored->getMovementSpeedFactor()==1.0_fix,"slow never expires");
        if(auto* neutral=game->getHouse(runtime(HOUSE_NEUTRAL))) {
            auto* post=dynamic_cast<GunTurret*>(neutral->placeStructure(NONE_ID,Structure_Slowpost,34,31,true,true));
            auto* victim=pLocalHouse->placeUnit(Unit_Tank,35,31,true);require(post && victim && !post->canBeCaptured(),"Slowpost factory/capture policy");
            victim->setVisible(-1,true);victim->doSetAttackMode(STOP);post->doAttackObject(victim);
            const auto count=bulletList.size();for(int tick=0;tick<500 && bulletList.size()==count;++tick)post->update();
            require(bulletList.size()>count && bulletList.back()->getBulletID()==Bullet_Slow,"Slowpost did not launch a slowing missile");
        }
        if(auto* klesh=game->getHouse(runtime(HOUSE_KLESHMERSH))) {
            auto* palace=dynamic_cast<Palace*>(klesh->placeStructure(NONE_ID,Structure_Palace,42,34,true,true));
            require(palace && palace->usesKleshmershNapalm() && palace->usesTargetedSpecialWeapon(),"Kleshmersh Palace is not targeted napalm");
            require(palace->getMaxSpecialWeaponTimer()==MILLI2CYCLES(600000),"napalm does not use Harkonnen recharge");
            for(int tick=0;tick<palace->getMaxSpecialWeaponTimer() && !palace->isSpecialWeaponReady();++tick){palace->setHealth(palace->getMaxHealth());palace->update();}
            require(palace->isSpecialWeaponReady(),"napalm Palace never becomes ready");
            std::set<Explosion*> previous;for(auto* effect:game->getExplosionList())previous.insert(effect);
            palace->doLaunchDeathhand(32,24);require(!bulletList.empty() && bulletList.back()->getBulletID()==Bullet_Napalm,"Palace launched an ordinary missile");
            auto* napalm=bulletList.back();require(FlameAccuracyFixture::directDamage(*napalm)==50,"napalm direct damage not halved");
            const auto inFlight=[&]{for(auto* missile:bulletList)if(missile==napalm)return true;return false;};
            for(int tick=0;tick<500;++tick) {
                if(!inFlight())break;
                napalm->update();
                if(inFlight())
                    require(game->getExplosionList().size()==previous.size(),"napalm ignited the route before impact");
            }
            std::vector<Explosion*> fires;for(auto* effect:game->getExplosionList())if(!previous.count(effect))fires.push_back(effect);
            require(fires.size()==9,"napalm must ignite only its 3x3 impact area");
            UnitBase* target=nullptr;Explosion* fire=nullptr;
            for(auto* effect:fires){
                const auto point=effect->getPosition();const Coord tile(point.x/TILESIZE,point.y/TILESIZE);
                if(!currentGameMap->tileExists(tile) || currentGameMap->getTile(tile)->hasAGroundObject())continue;
                // An impact can land near any tile corner. Try the infantry's
                // five sub-tile positions, not only the tile centre.
                std::vector<UnitBase*> probes;
                for(int slot=0;slot<5 && !fire;++slot) {
                    auto* probe=pLocalHouse->placeUnit(Unit_Soldier,tile.x,tile.y,true);
                    if(!probe)break;
                    probes.push_back(probe);
                    if(distanceFrom(probe->getCenterPoint(),point)<=TILESIZE/2){target=probe;fire=effect;}
                }
                for(auto* probe:probes)if(probe!=target)probe->destroy();
                if(fire)break;
            }
            require(target && fire,"no fire tile available for persistence check");const auto hp=target->getHealth();
            const auto targetID=target->getObjectID();const auto firePoint=fire->getPosition();
            const auto fireSave=(std::filesystem::path(output)/(mod+"-napalm-fire.dls")).string();require(game->saveGame(fireSave),"napalm hazard save");
            const auto burn=[&](Explosion* hazard){for(int tick=0;tick<150;++tick){bool alive=false;for(auto* effect:game->getExplosionList())if(effect==hazard)alive=true;if(!alive)break;hazard->update();}};
            burn(fire);const auto damage=hp-target->getHealth();require(damage>0,"napalm fire does not damage a unit arriving after impact");
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
            game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(fireSave));
            target=static_cast<UnitBase*>(game->getObjectManager().getObject(targetID));fire=nullptr;
            for(auto* effect:game->getExplosionList())if(effect->getPosition()==firePoint){fire=effect;break;}
            require(target && fire && target->getHealth()==hp,"saved hazard or victim lost");burn(fire);
            require(hp-target->getHealth()==damage,"napalm damage changes after save/load");
        }
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        options.chaosMode=true;game=std::make_unique<Game>();currentGame=game.get();game->initGame(chaosSmokeInit(runtime(HOUSE_ATREIDES),options));
        bool yes=false,no=false;Uint32 offerID=NONE_ID;
        for(int attempt=0;attempt<128 && !(yes&&no);++attempt){
            auto* shop=dynamic_cast<Worfinery*>(pLocalHouse->placeStructure(NONE_ID,Structure_Worfinery,3,4,true,true));require(shop,"Chaos Worfinery");
            const bool offer=shop->hasTameWormOffer();require(available(shop,Unit_TameSandworm)==offer,"Chaos worm offer ignored");
            if(offer){yes=true;offerID=shop->getObjectID();shop->doProduceItem(Unit_TameSandworm);require(shop->getCurrentProducedItem()==Unit_TameSandworm,"worm order refused");shop->doCancelItem(Unit_TameSandworm);}
            else no=true;
            if(offer && no)break;shop->destroy();offerID=NONE_ID;
        }
        require(yes&&no,"deterministic Chaos draw did not produce both outcomes");
        // Keep a surviving offer for save/reload, without rerolling on update.
        auto* shop=dynamic_cast<Worfinery*>(game->getObjectManager().getObject(offerID));
        if(shop){const auto file=(std::filesystem::path(output)/(mod+"-worm-offer.dls")).string();require(game->saveGame(file),"worm offer save");game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(file));shop=dynamic_cast<Worfinery*>(game->getObjectManager().getObject(offerID));require(shop && shop->hasTameWormOffer() && available(shop,Unit_TameSandworm),"worm offer rerolled on load");}
    }
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    auto easy=effectiveGameOptions;easy.easyMode=true;easy.sellingBuildings=true;
    const auto replayPath=(std::filesystem::path(output)/(mod+"-easy-sale.rpl")).string();
    GameInitSettings easySetup(static_cast<HOUSETYPE>(roster.front()),easy);
    for(const auto& house:init.getHouseInfoList())easySetup.addHouseInfo(house);
    { OFileStream replay;require(replay.open(replayPath),"Easy Mode replay setup");replay.writeString(settings.general.playerName);easySetup.save(replay); }
    game=std::make_unique<Game>();currentGame=game.get();game->initReplay(replayPath);
    auto* owner=pLocalHouse;require(owner,"Easy Mode local house missing");owner->createUnit(Unit_MCV,true);
    auto* silo=owner->placeStructure(NONE_ID,Structure_Silo,4,4,true,true);require(silo,"Easy Mode sale building");
    const int value=game->getGameInitSettings().campaignPurchasePrice(game->objectData.data[Structure_Silo][silo->getOriginalHouseID()].price,owner->getHouseID());
    const int refund=value*75/100;require(silo->getSaleRefund()==refund,"Easy Mode sale is not 75% of discounted purchase value");
    const int credits=owner->getCredits();silo->doSell();require(owner->getCredits()==credits+refund,"Easy Mode refund was lost");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    SDL_Log("RELEASE 543 EXPANSION PASS: %s, production gates, movement-only slowing, variants/cargo/offers save-load and 75%% Easy Mode sales",mod.c_str());
}
