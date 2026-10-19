#pragma once
// SPDX-License-Identifier: GPL-2.0-or-later
#include <units/HarvesterHelpers.h>
#include <fstream>
#include <cstdlib>

inline void verifyHarvesterCrushing(const std::string& output,const std::string& mod) {
    const auto require=[](bool ok,const std::string& why) {
        if(!ok){SDL_Log("HARVESTER CRUSH FAIL: %s",why.c_str());std::exit(EXIT_FAILURE);}
    };
    for(int item:{Unit_Harvester,Unit_FastHarvester,Unit_CapacityHarvester,Unit_RebelHarvester}) {
        if(mod=="vanilla" && item!=Unit_Harvester)continue;
        auto options=effectiveGameOptions;options.chaosMode=false;options.sandwormsRespawn=false;
        std::string map="[BASIC]\nVersion=2\nTechLevel=9\n[MAP]\nSizeX=20\nSizeY=12\n";
        for(int y=0;y<12;++y)map+=fmt::sprintf("%03d=",y)+std::string(20,'%')+"\n";
        map+="[Player1]\nCredits=1000\n[Player2]\nCredits=1000\n";
        GameInitSettings init("harvester-crush",map,false,options);
        const auto localID=getRuntimeHouseForIdentity(HOUSE_HARKONNEN),enemyID=getRuntimeHouseForIdentity(HOUSE_ATREIDES);
        for(const auto id:{localID,enemyID}) {
            GameInitSettings::HouseInfo house(static_cast<HOUSETYPE>(id),id==localID?1:2);
            house.addPlayerInfo(GameInitSettings::PlayerInfo(id==localID?settings.general.playerName:"Blocker",HUMANPLAYERCLASS));
            init.addHouseInfo(house);
        }
        auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
        auto* owner=game->getHouse(localID);auto* enemy=game->getHouse(enemyID);
        for(int y=0;y<12;++y)for(int x=0;x<20;++x)currentGameMap->getTile(x,y)->setType(y==5?Terrain_Sand:Terrain_Mountain);
        const Coord blockerPosition(4,5),spicePosition(8,5);
        for(int x=2;x<=8;++x)currentGameMap->getTile(x,5)->setType(Terrain_ThickSpice);
        owner->placeUnit(Unit_MCV,0,5,true);enemy->placeUnit(Unit_MCV,19,5,true);
        auto* unit=owner->placeUnit(item,2,5,true);auto* blocker=enemy->placeUnit(Unit_Soldier,4,5,true);
        require(unit && blocker,"harvester/blocker creation");
        blocker->doSetAttackMode(STOP);blocker->setVisible(VIS_ALL,true);
        const auto blockerID=blocker->getObjectID();
        unit->doSetAttackMode(AREAGUARD);unit->setGuardPoint(spicePosition);unit->doMove2Pos(spicePosition,false);
        CaptureHuntGameFixture::start(*game);
        std::ofstream trace(std::filesystem::path(output)/(mod+"-harvester-crush-"+std::to_string(item)+".csv"));
        trace<<"tick,x,y,destX,destY,moving,target,cargo\n";
        Coord killPosition=Coord::Invalid();FixPoint cargoAtKill=0;
        for(int tick=0;tick<1600;++tick) {
            const bool alive=game->getObjectManager().getObject(blockerID)!=nullptr;
            CaptureHuntGameFixture::tick(*game);
            if(alive && !game->getObjectManager().getObject(blockerID)){killPosition=unit->getLocation();cargoAtKill=harvesterGetAmountOfSpice(unit);}
            if(tick%50==0)trace<<tick<<','<<unit->getLocation().x<<','<<unit->getLocation().y<<','<<unit->getDestination().x<<','<<unit->getDestination().y<<','<<unit->isMoving()<<','<<(unit->getTarget()?unit->getTarget()->getItemID():0)<<','<<harvesterGetAmountOfSpice(unit).toDouble()<<'\n';
            if(killPosition.isValid() && harvesterGetAmountOfSpice(unit)>cargoAtKill)break;
        }
        trace.close();
        SDL_Log("HARVESTER CRUSH RESULT: %s item=%d x=%d destination=%d blocker=%d killX=%d cargo=%.2f",mod.c_str(),item,unit->getLocation().x,unit->getDestination().x,game->getObjectManager().getObject(blockerID)!=nullptr,killPosition.x,harvesterGetAmountOfSpice(unit).toDouble());
        if(item==Unit_RebelHarvester)require(killPosition.isValid(),"armed harvester failed to clear its blocker");
        else require(killPosition==blockerPosition,"Area Guard harvester did not crush its infantry blocker: "+mod+" / "+std::to_string(item));
        require(harvesterGetAmountOfSpice(unit)>cargoAtKill,"harvester did not resume harvesting after crushing");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    }
    SDL_Log("HARVESTER CRUSH PASS: %s, Area Guard harvest routes crush infantry, armed Harvestank retains firing, and collection resumes",mod.c_str());
}
