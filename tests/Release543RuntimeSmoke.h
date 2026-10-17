#pragma once
#include <GUI/ObjectInterfaces/DefaultStructureInterface.h>
#include <structures/Refinery.h>
#include <structures/RepairYard.h>
#include <structures/Silo.h>
#include <units/Harvester.h>
#include <misc/OMemoryStream.h>
#include <misc/IMemoryStream.h>

#include "Release543UnitsSmoke.h"

inline void verifyRelease543Alliances(const std::string& output,const std::string& mod) {
    auto require=[](bool ok,const char* message){if(!ok)throw std::runtime_error(std::string("Release 543 alliances: ")+message);};
    const HOUSETYPE houses[]={getRuntimeHouseForIdentity(HOUSE_ATREIDES),getRuntimeHouseForIdentity(HOUSE_HARKONNEN),getRuntimeHouseForIdentity(HOUSE_ORDOS)};
    for(int flags=0;flags<4;++flags) {
        auto options=effectiveGameOptions;options.chaosMode=false;options.sellingBuildings=true;
        options.alliedControl=(flags&1)!=0;options.alliedBuilding=(flags&2)!=0;
        GameInitSettings init("alliance-test",chaosSmokeMap()+"[Player2]\nCredits=1000000\n[Player3]\nCredits=1000000\n","Alliance test",false,options);
        for(int i=0;i<3;++i) {
            GameInitSettings::HouseInfo info(houses[i],i<2?1:2);
            info.addPlayerInfo(GameInitSettings::PlayerInfo(i==0?settings.general.playerName:"Alliance AI "+std::to_string(i),i==0?HUMANPLAYERCLASS:"qBotEasy"));init.addHouseInfo(info);
        }
        auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
        auto* owner=game->getHouse(houses[0]);auto* ally=game->getHouse(houses[1]);auto* enemy=game->getHouse(houses[2]);
        for(auto* house:{owner,ally,enemy})house->createUnit(Unit_MCV,true);
        require(game->canPlayerControlHouse(owner,owner),"own control denied");
        require(game->canPlayerControlHouse(owner,ally)==options.alliedControl,"allied control flag ignored");
        require(!game->canPlayerControlHouse(owner,enemy),"enemy control allowed");
        require(game->canBuildOnAlliedLand(owner,houses[1])==options.alliedBuilding,"allied construction flag ignored");
        require(!game->canBuildOnAlliedLand(owner,houses[2]),"enemy construction allowed");
        auto* silo=ally->placeStructure(NONE_ID,Structure_Silo,12,12,true,true);require(silo,"allied silo placement");
        const auto id=silo->getObjectID();const int ownFunds=owner->getCredits(),allyFunds=ally->getCredits(),refund=silo->getSaleRefund();
        Command sale(pLocalPlayer->getPlayerID(),CMD_STRUCTURE_SELL,id);sale.executeCommand();
        require((game->getObjectManager().getObject(id)==nullptr)==options.alliedControl,"allied sale authorization");
        require(owner->getCredits()==ownFunds && ally->getCredits()==allyFunds+(options.alliedControl?refund:0),"allied refund credited wrong owner");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        if(flags==0) {
            auto host=std::make_unique<CustomGamePlayers>(init,true);auto guest=std::make_unique<CustomGamePlayers>(init,false);
            require(host->alliedControlCheckbox.isEnabled() && host->alliedBuildingCheckbox.isEnabled(),"host alliance options disabled");
            require(!guest->alliedControlCheckbox.isEnabled() && !guest->alliedBuildingCheckbox.isEnabled(),"guest may change alliance options");
            ChangeEventList forged;forged.changeEventList.emplace_back(ChangeEventList::ChangeEvent::EventType::ChangeAlliedBuilding,0,1);
            host->onReceiveChangeEventList(forged);require(!host->alliedBuildingCheckbox.isChecked(),"guest forged host construction option");
            for(auto* box:{&host->alliedControlCheckbox,&host->alliedBuildingCheckbox}) {box->handleMouseLeft(5,5,true);box->handleMouseLeft(5,5,false);}
            OMemoryStream wire;host->getChangeEventList().save(wire);IMemoryStream bytes(wire.getData(),wire.getDataLength());ChangeEventList snapshot(bytes);
            guest->onReceiveChangeEventList(snapshot);
            require(guest->gameInitSettings.getGameOptions().alliedControl && guest->gameInitSettings.getGameOptions().alliedBuilding,"host options did not synchronize to guest");
        }
    }
    SDL_Log("RELEASE 543 ALLIANCES PASS: %s, independent permissions, allied sale ownership, host-only lobby and serialized synchronization",mod.c_str());
}

inline void verifyRelease543Rules(const std::string& output,const std::string& mod) {
    auto require=[](bool ok,const std::string& message){if(!ok)throw std::runtime_error("Release 543: "+message);};
    const auto defaults=effectiveGameOptions;
    require(defaults.sellingBuildings==(mod=="Tornie"||mod=="TornieLite"||mod=="ArcaKev"||mod=="ArcaKevLite"),"selling default for "+mod);
    require(defaults.showHarvesterCount==(mod!="Tornie"&&mod!="TornieLite"),"harvester overlay default for "+mod);
    auto options=defaults;options.sellingBuildings=true;options.alliedControl=true;options.alliedBuilding=true;
    auto init=chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_ATREIDES),options);
    OMemoryStream wire;init.save(wire);wire.writeUint32(0x1234abcd);
    IMemoryStream input(wire.getData(),wire.getDataLength());GameInitSettings received(input);
    require(received.getGameOptions()==options && input.readUint32()==0x1234abcd,"MOD6 settings roundtrip changed rules or stream boundary");
    auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
    auto* owner=pLocalHouse;
    // Own an MCV so selling the last structure does not finish the test match.
    owner->createUnit(Unit_MCV,true);
    owner->takeCredits(owner->getCredits());owner->addUnstoredCredits(1000);
    auto* silo=owner->placeStructure(NONE_ID,Structure_Silo,4,4,true,true);
    require(silo,"silo placement");const auto id=silo->getObjectID();const auto refund=silo->getSaleRefund();
    require(refund==game->getGameInitSettings().campaignPurchasePrice(game->objectData.data[Structure_Silo][silo->getOriginalHouseID()].price,owner->getHouseID())/2,"sale is not half construction price");
    silo->setHealth(silo->getMaxHealth()/2);require(silo->getSaleRefund()==refund,"damaged building has a different sale rate");
    struct Toolbar : DefaultStructureInterface {
        explicit Toolbar(int id):DefaultStructureInterface(id){}
        bool refresh(){return update();}
        bool paired(){return sellButton.getSize()==repairButton.getSize() && topBox.getWidgetPosition(&sellButton).x==topBox.getWidgetPosition(&repairButton).x+repairButton.getSize().x+2;}
    };
    {
        Toolbar toolbar(id);require(toolbar.refresh() && toolbar.paired(),"sale/repair buttons differ in size or overlap");
        SDL_RenderSetClipRect(renderer,nullptr);SDL_SetRenderDrawColor(renderer,30,30,30,255);SDL_RenderClear(renderer);toolbar.draw(Point(0,0));
        SDL_Rect area{0,0,SIDEBARWIDTH-25,22};
        auto surface=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,area.w,area.h,32,SDL_PIXELFORMAT_RGBA32)};
        require(SDL_RenderReadPixels(renderer,&area,surface->format->format,surface->pixels,surface->pitch)==0,"sale toolbar render");
        require(SavePNG(surface.get(),(std::filesystem::path(output)/(mod+"-sale-toolbar.png")).string().c_str())==0,"sale toolbar preview");
    }
    Command unauthorized(255,CMD_STRUCTURE_SELL,id);unauthorized.executeCommand();
    require(game->getObjectManager().getObject(id)!=nullptr,"invalid player sold a building");
    const int funds=owner->getCredits();const int losses=owner->getNumDestroyedStructures();
    Command sale(pLocalPlayer->getPlayerID(),CMD_STRUCTURE_SELL,id);
    OMemoryStream commands;sale.save(commands);IMemoryStream bytes(commands.getData(),commands.getDataLength());Command networkSale(bytes);
    networkSale.executeCommand();networkSale.executeCommand();
    require(!game->getObjectManager().getObject(id),"sale left a structure registered");
    require(owner->getCapacity()==0 && owner->getStoredCredits()==0,"sale proceeds occupy storage");
    owner->update();
    require(owner->getCredits()==funds+refund,"refund lost without storage or duplicate command refunded twice");
    require(owner->getStructuresSold()==1 && owner->getSaleCredits()==refund,"per-house sale statistics");
    require(owner->getNumDestroyedStructures()==losses,"sale counted as destruction");
    require(!currentGameMap->getTile(4,4)->hasAGroundObject(),"sold building still blocks its footprint");
    // A refinery's hidden harvester must be deployed, not destroyed with its container.
    auto* refinery=dynamic_cast<Refinery*>(owner->placeStructure(NONE_ID,Structure_Refinery,8,8,true,true));require(refinery,"refinery placement");
    auto* harvester=dynamic_cast<Harvester*>(owner->createUnit(Unit_Harvester,true));require(harvester,"harvester creation");
    const auto harvesterID=harvester->getObjectID();refinery->book();require(refinery->receiveHarvester(harvester),"harvester entry");
    refinery->doSell();
    require(game->getObjectManager().getObject(harvesterID)==harvester && harvester->isActive(),"sale killed or stranded the contained harvester");
    auto* wind=owner->placeStructure(NONE_ID,Structure_WindTrap,12,12,true,true);require(wind,"windtrap placement");
    const int power=owner->getProducedPower();wind->doSell();require(owner->getProducedPower()<power,"selling a windtrap left ghost power");
    const auto saleCount=owner->getStructuresSold();const auto saleCredits=owner->getSaleCredits();const auto balance=owner->getCredits();
    const auto file=(std::filesystem::path(output)/(mod+"-sale-save.dls")).string();require(game->saveGame(file),"sale save");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    GameInitSettings load(file);game=std::make_unique<Game>();currentGame=game.get();game->initGame(load);
    require(pLocalHouse->getStructuresSold()==saleCount && pLocalHouse->getSaleCredits()==saleCredits && pLocalHouse->getCredits()==balance,"sale statistics or protected refund changed after reload");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    options.sellingBuildings=false;init=chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_ATREIDES),options);
    game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
    auto* disabled=pLocalHouse->placeStructure(NONE_ID,Structure_Silo,4,4,true,true);require(disabled && !disabled->canSell(),"disabled rule still permits a sale");
    const auto disabledID=disabled->getObjectID();disabled->doSell();require(game->getObjectManager().getObject(disabledID)==disabled,"disabled direct sale removed a building");
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    verifyRelease543Alliances(output,mod);
    verifyRelease543Units(output,mod);
    SDL_Log("RELEASE 543 PASS: %s, rule defaults, 50%% selling, no-storage funds, evacuation, power, command replay and save/reload",mod.c_str());
}
