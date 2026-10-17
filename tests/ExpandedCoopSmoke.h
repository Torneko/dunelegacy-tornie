#pragma once

#include <Campaign/CoopCampaignRuntime.h>
#include <Menu/CustomGamePlayers.h>
#include <FileClasses/LoadSavePNG.h>
#include <FileClasses/INIFile.h>
#include <misc/draw_util.h>
#include <misc/FileSystem.h>
#include <units/MCV.h>
#include <filesystem>

inline void verifyExpandedCoop(const std::string& output,const std::string& mod) {
    auto require=[](bool ok,const std::string& message) {
        if(!ok) throw std::runtime_error("Expanded co-op: "+message);
    };
    const auto roster=getFactionSelectionOrder();
    const auto host=settings.general.playerName;
    const std::vector<std::string> names{host,"Coop Two","Coop Three","Coop Four","Coop Five","Coop Six","Coop Seven","Coop Eight"};
    auto create=[&](bool archon) {
        auto session=coop::CoopCampaignSession::create("expanded-check",mod,roster,
            {roster[0],roster[1]},{host,names[1]},98765,false);
        session.enableExpandedAllies(archon);
        return session;
    };
    auto options=effectiveGameOptions;options.chaosMode=false;
    for(int mode=0;mode<3;++mode) {
        auto session=create(mode==2);
        session.setEasyMode(true);
        session.setAlliedBuilding(mode!=0);
        if(mode==1) {
            session.configureAlly(2,roster[2],HUMANPLAYERCLASS,names[2]);
            session.configureAlly(3,roster[3],HUMANPLAYERCLASS,names[3]);
        } else if(mode==2) {
            session.configureAlly(1,roster[1],"Closed",names[1]);
            session.configureAlly(0,roster[0],HUMANPLAYERCLASS,host,names);
        }
        for(int stage=1;stage<=coop::StageCount;++stage) {
            auto init=coop::makeGameSettings(session,options,"Expanded runtime check");
            session=coop::CoopCampaignSession::fromMapData(init.getFiledata());
            const auto context=session.context();
            require(context.alliedBuilding==(mode!=0),"shared construction lost on mission transition");
            auto data=sdl2::RWops_ptr{SDL_RWFromConstMem(init.getFiledata().data(),init.getFiledata().size())};
            INIFile map(data.get());
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            for(const int index:context.alliedSlots()) {
                const auto& slot=context.slots[index];const auto* house=game->getHouse(slot.house);
                if(!slot.active()) { require(!house,"closed ally has a live house");continue; }
                require(house && house->getFactionID()==slot.faction,"faction/runtime binding");
                const int sourceCredits=map.getIntValue("Player1","Credits",-1);
                require(sourceCredits>=0 && map.getIntValue("Player"+std::to_string(index+1),"Credits",-1)==sourceCredits,
                    "allied starting funds differ from the host at stage "+std::to_string(stage)+" slot "+std::to_string(index+1));
                require(house->getCredits()==sourceCredits+500,
                    "Easy Mode starting credits at stage "+std::to_string(stage));
                require(house->getNumUnits()>0,"active ally has no starting army");
                for(const auto& name:slot.humanNames()) {
                    const auto* player=game->getPlayerByName(name);
                    require(player && player->getHouse()==house,"Archon controller is bound to the wrong house");
                }
                if(index>=context.enemyPresent.size()+2) {
                    MCV* start=nullptr;
                    for(auto* unit:unitList) if(unit->getOwner()==house && unit->getItemID()==Unit_MCV) {
                        start=dynamic_cast<MCV*>(unit);break;
                    }
                    require(start && start->canDeploy(),"optional ally MCV cannot deploy at stage "+std::to_string(stage));
                    require(start->doDeploy(),"optional ally MCV deployment failed");
                    require(house->getNumItems(Structure_ConstructionYard)>0,"optional ally has no Construction Yard after deployment");
                }
            }
            if(stage==3 || stage==9) {
                const auto save=(std::filesystem::path(output)/(mod+"-expanded-"+std::to_string(mode)+"-"+std::to_string(stage)+".dls")).string();
                std::map<int,int> savedCredits;
                for(const int index:context.alliedSlots()) if(context.slots[index].active()) {
                    const int house=context.slots[index].house;
                    savedCredits[house]=game->getHouse(house)->getCredits();
                }
                require(game->saveGame(save),"expanded save");
                game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                game=std::make_unique<Game>();currentGame=game.get();
                GameInitSettings load(getBasename(save,true),readCompleteFile(save),"Expanded save load");
                for(const auto& house:init.getHouseInfoList()) load.addHouseInfo(house);
                game->initGame(load);
                const auto restored=coop::readContext(game->getGameInitSettings().getFiledata());
                require(restored && restored->humanNames()==context.humanNames(),"saved controller roster");
                require(restored->alliedBuilding==context.alliedBuilding,"saved shared construction option");
                for(const auto& balance:savedCredits) require(game->getHouse(balance.first)
                    && game->getHouse(balance.first)->getCredits()==balance.second,"saved allied starting funds");
                for(const auto& name:context.humanNames()) require(game->getPlayerByName(name)!=nullptr,"saved controller object");
            }
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
            session.completeMission(true);
        }
    }
    // Compare identical scenario/RNG setups: the two difficulty switches add 5,
    // 10, or 15 units to every present enemy and never to an absent opponent.
    std::array<int,NUM_HOUSES> baseline{};
    for(int extra:{0,5,10,15}) {
        auto session=create(false);
        session.setExtraEnemyForces(extra==5 || extra==15);
        session.setExtraHardEnemyForces(extra>=10);
        auto init=coop::makeGameSettings(session,options,"Enemy force check");
        const auto context=coop::readContext(init.getFiledata());
        auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
        for(std::size_t i=0;i<context->enemyPresent.size();++i) {
            const int house=context->slots[i+2].house;
            const int count=game->getHouse(house) ? game->getHouse(house)->getNumUnits() : 0;
            if(extra==0) baseline[house]=count;
            else require(count==baseline[house]+(context->enemyPresent[i] ? extra : 0),"extra enemy force count");
        }
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    }
    for(const bool archon:{false,true}) {
        auto session=create(archon);
        auto init=coop::makeGameSettings(session,options,"Expanded lobby check");
        auto owned=std::make_unique<CustomGamePlayers>(init,false);
        auto& lobby=*owned;
        if(archon) {
            ChangeEventList changes;
            for(int i=0;i<8;++i) {
                changes.changeEventList.emplace_back(ChangeEventList::ChangeEvent::EventType::ChangeArchonPlayer,i,names[i]);
                changes.changeEventList.emplace_back(ChangeEventList::ChangeEvent::EventType::ChangeArchonAssignment,i,0);
            }
            lobby.onReceiveChangeEventList(changes);
        } else {
            const auto indices=session.context().alliedSlots();
            for(int i=0;i<4;++i) lobby.setPlayer2Slot(names[i],indices[i]*2);
            // Real checkbox interaction as a guest, including its popup choices.
            const auto originalName=settings.general.playerName;
            settings.general.playerName=names[2];lobby.enforceCoopLobby();
            auto& row=lobby.houseInfo[indices[2]];
            require(row.bonusColorCheckbox.isEnabled(),"guest bonus colours disabled");
            row.bonusColorCheckbox.setChecked(true);lobby.onBonusColorCheckbox(indices[2]);
            require(row.bonusColorCheckbox.isChecked() && row.colorDropDown.getNumEntries()>0,"guest bonus colour was reset");
            settings.general.playerName=originalName;
        }
        lobby.addAllPlayersToGameInitSettings();
        const auto context=coop::readContext(lobby.gameInitSettings.getFiledata());
        require(context && context->humanNames().size()==(archon ? 8u : 4u),"lobby dropped human controllers");
        SDL_RenderSetClipRect(renderer,nullptr);SDL_RenderClear(renderer);lobby.draw();
        auto pixels=renderReadSurface(renderer);
        require(pixels && SavePNG(pixels.get(),(std::filesystem::path(output)/(mod+(archon ? "-archon-lobby.png" : "-four-coop-lobby.png"))).string().c_str())==0,"lobby capture");
    }
    // Exercise the real lobby twice (prepare + countdown), then the settings
    // received by each guest. Compare with the host, not the guest map field.
    for(const bool easy:{false,true}) {
        auto session=create(false);
        auto initial=coop::makeGameSettings(session,options,"Starting funds lobby");
        initial.setMapData(initial.getFiledata()+"SourcePending=1\n");
        GameInitSettings committed;
        {
            auto lobby=std::make_unique<CustomGamePlayers>(initial,true);
            const auto indices=session.context().alliedSlots();
            for(int ordinal=0;ordinal<4;++ordinal) lobby->setPlayer2Slot(names[ordinal],indices[ordinal]*2);
            lobby->coopEasyModeCheckbox.setChecked(easy);
            lobby->alliedBuildingCheckbox.setChecked(true);
            lobby->addAllPlayersToGameInitSettings();
            lobby->addAllPlayersToGameInitSettings();
            committed=lobby->gameInitSettings;
        }
        OMemoryStream wire;committed.save(wire);
        IMemoryStream incoming(wire.getData(),wire.getDataLength());
        GameInitSettings received(incoming);
        const auto context=coop::readContext(received.getFiledata());
        require(context && context->humanNames().size()==4 && context->alliedBuilding,"starting funds/building lobby roster");
        auto data=sdl2::RWops_ptr{SDL_RWFromConstMem(received.getFiledata().data(),received.getFiledata().size())};
        INIFile map(data.get());
        const int expected=map.getIntValue("Player1","Credits",-1)+(easy ? 500 : 0);
        // Optional player sections may contain an empty budget in an older or
        // partially initialized map. They must receive the host's starting funds.
        const auto indices=context->alliedSlots();
        map.setIntValue("Player"+std::to_string(indices[2]+1),"Credits",0);
        map.removeKey("Player"+std::to_string(indices[3]+1),"Credits");
        std::vector<char> buffer(received.getFiledata().size()*2+1024);
        auto written=sdl2::RWops_ptr{SDL_RWFromMem(buffer.data(),buffer.size())};
        require(map.saveChangesTo(written.get()),"starting funds fixture serialization");
        received.setMapData(std::string(buffer.data(),static_cast<std::size_t>(SDL_RWtell(written.get()))));
        for(int ordinal=0;ordinal<4;++ordinal) {
            settings.general.playerName=names[ordinal];
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(received);
            const auto* player=game->getPlayerByName(names[ordinal]);
            require(player && pLocalHouse==player->getHouse(),"starting funds guest local ownership");
            require(pLocalHouse->getCredits()==expected,"starting funds guest "+std::to_string(ordinal+1)
                +" expected "+std::to_string(expected)+" got "+std::to_string(pLocalHouse->getCredits()));
            // Test the same placement predicate used by building previews and
            // mouse placement, from each human's runtime house (including 3/4).
            Coord target(INVALID,INVALID);
            for(int y=4;y<currentGameMap->getSizeY()-6 && !target.isValid();++y)
                for(int x=4;x<currentGameMap->getSizeX()-6 && !target.isValid();++x) {
                    bool clear=true;
                    for(int dy=0;dy<4;++dy) for(int dx=0;dx<4;++dx)
                        clear &= !currentGameMap->getTile(x+dx,y+dy)->isBlocked();
                    if(clear) target=Coord(x,y);
                }
            require(target.isValid(),"no placement fixture area");
            for(int y=target.y-BUILDRANGE;y<=target.y+3+BUILDRANGE;++y)
                for(int x=target.x-BUILDRANGE;x<=target.x+3+BUILDRANGE;++x)
                    currentGameMap->getTile(x,y)->setOwner(HOUSE_INVALID);
            for(int dy=0;dy<4;++dy) for(int dx=0;dx<4;++dx)
                currentGameMap->getTile(target.x+dx,target.y+dy)->setType(Terrain_Rock);
            auto* anchor=currentGameMap->getTile(target.x-1,target.y);
            anchor->setType(Terrain_Slab);
            auto disabled=received.getFiledata();
            const auto flag=disabled.find("AlliedBuilding=1");
            require(flag!=std::string::npos,"shared construction not serialized");
            disabled.replace(flag,16,"AlliedBuilding=0");
            for(const int index:indices) {
                const int owner=context->slots[index].house;
                anchor->setOwner(owner);
                game->updateCoopParticipants(received.getFiledata(),received.getHouseInfoList());
                require(currentGameMap->okayToPlaceStructure(target.x,target.y,2,2,false,pLocalHouse),"enabled shared placement rejected ally");
                require(!game->canControlHouse(game->getHouse(owner)) || owner==pLocalHouse->getHouseID(),"shared terrain enabled allied commands");
                game->updateCoopParticipants(disabled,received.getHouseInfoList());
                require(currentGameMap->okayToPlaceStructure(target.x,target.y,2,2,false,pLocalHouse)
                    ==(owner==pLocalHouse->getHouseID()),"disabled shared placement accepted another house");
            }
            game->updateCoopParticipants(received.getFiledata(),received.getHouseInfoList());
            anchor->setOwner(context->slots[2].house);
            require(!currentGameMap->isWithinBuildRange(target.x,target.y,pLocalHouse),"enemy terrain grants build range");
            anchor->setOwner(context->slots[indices[(ordinal+1)%4]].house);
            currentGameMap->getTile(target.x,target.y)->setType(Terrain_Sand);
            require(!currentGameMap->okayToPlaceStructure(target.x,target.y,2,2,false,pLocalHouse),"shared range permits sand");
            currentGameMap->getTile(target.x,target.y)->setType(Terrain_Rock);
            require(!currentGameMap->okayToPlaceStructure(target.x,target.y,2,2,true,pLocalHouse),"concrete requirement bypassed");
            const int neighbourCredits=game->getHouse(anchor->getOwner())->getCredits();
            auto* building=pLocalHouse->placeStructure(NONE_ID,Structure_WindTrap,target.x,target.y,true);
            require(building && building->getOwner()==pLocalHouse,"building on allied land changed owner");
            require(game->getHouse(anchor->getOwner())->getCredits()==neighbourCredits,"building changed allied funds");
            require(!currentGameMap->okayToPlaceStructure(target.x,target.y,2,2,false,pLocalHouse),"occupied allied land accepted");

            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        }
        settings.general.playerName=host;
    }
    SDL_Log("EXPANDED COOP PASS: %s, stages 1-9, closed/four/eight controllers, Easy credits, shared construction, save/load and lobby assignments",mod.c_str());
}
