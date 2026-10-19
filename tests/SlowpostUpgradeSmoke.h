#pragma once
#include <structures/Scoutpost.h>
#include "CaptureHuntSmoke.h"
#include <set>

struct SlowpostSmokeFixture {
    static int frame(const Scoutpost& post) { return post.curAnimFrame; }
    static void ready(Scoutpost& post) { post.justPlacedTimer=0; post.animationCounter=0; }
};

inline void verifySlowpostUpgrade(const std::string& output,const std::string& mod) {
    const auto require=[](bool ok,const char* why) {
        if(!ok)throw std::runtime_error(std::string("Slowpost upgrade: ")+why);
    };
    auto options=effectiveGameOptions;options.chaosMode=false;options.instantBuild=true;options.sellingBuildings=true;
    auto game=std::make_unique<Game>();currentGame=game.get();
    game->initGame(chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_ATREIDES),options));
    auto* owner=pLocalHouse;owner->createUnit(Unit_MCV,true);owner->addUnstoredCredits(10000);
    auto* yard=dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID,Structure_ConstructionYard,2,2,true,true));
    auto* post=dynamic_cast<Scoutpost*>(owner->placeStructure(NONE_ID,Structure_Scoutpost,8,2,true,true));
    require(yard && post,"fixture buildings");
    game->techLevel=9;
    for(int house=0;house<NUM_HOUSES;++house) {
        yard->setOriginalHouseID(house);yard->updateBuildList();
        require(!yard->isAvailableToBuild(Structure_Slowpost),"direct Construction Yard production remains");
    }
    require(!post->isSlowpostUpgradeEligible(),"Atreides gained Neutral's upgrade");
    // Captured/borrowed Neutral plans remain usable even in a Lite edition.
    post->setOriginalHouseID(HOUSE_NEUTRAL);
    game->techLevel=6;require(!post->canUpgradeToSlowpost(),"upgrade below level seven");
    game->techLevel=7;require(!post->canUpgradeToSlowpost(),"upgrade without IX");
    owner->placeStructure(NONE_ID,Structure_IX,12,2,true,true);
    if(mod=="vanilla") {
        require(!post->isSlowpostUpgradeEligible(),"Vanilla gained a custom post upgrade");
    } else {
        const int price=post->getSlowpostUpgradeCost();
        require(price==450 && post->canUpgradeToSlowpost(),"Neutral level-seven IX upgrade missing");
        const int credits=owner->getCredits(),power=owner->getProducedPower();
        const auto health=post->getHealth();
        const auto id=post->getObjectID();
        Command unauthorized(255,CMD_SCOUTPOST_SLOWPOST_UPGRADE,id);unauthorized.executeCommand();
        require(post->getItemID()==Structure_Scoutpost && owner->getCredits()==credits,"unauthorized upgrade");
        Command upgrade(pLocalPlayer->getPlayerID(),CMD_SCOUTPOST_SLOWPOST_UPGRADE,id);upgrade.executeCommand();
        require(post->getItemID()==Structure_Slowpost && owner->getCredits()==credits-price,"upgrade payment or identity");
        require(post->getObjectID()==id && post->getHealth()==health && owner->getProducedPower()==power,"upgrade lost object state/power");
        require(!post->canUpgradeToSlowpost() && !post->canBeCaptured(),"repeat upgrade/capture allowed");
        upgrade.executeCommand();require(owner->getCredits()==credits-price,"duplicate command charged twice");
        const int refund=(game->objectData.data[Structure_Scoutpost][HOUSE_NEUTRAL].price+price)/2;
        require(post->getSaleRefund()==refund,"sale omits Scoutpost investment");
        SlowpostSmokeFixture::ready(*post);
        std::set<int> frames;
        for(int tick=0;tick<40;++tick) {
            CaptureHuntGameFixture::tick(*game);
            frames.insert(SlowpostSmokeFixture::frame(*post));
        }
        require(frames==std::set<int>{2,3},"Slowpost does not animate both supplied states");
        const auto file=(std::filesystem::path(output)/(mod+"-slowpost-upgrade.dls")).string();
        require(game->saveGame(file),"upgrade save");
        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(file));
        post=dynamic_cast<Scoutpost*>(game->getObjectManager().getObject(id));
        require(post && post->getSaleRefund()==refund && post->getProducedPower()>0,"conversion/power/refund lost on reload");
        // Map/legacy directly built posts keep their original purchase value.
        auto* placed=dynamic_cast<Scoutpost*>(post->getOwner()->placeStructure(NONE_ID,Structure_Slowpost,18,2,true,true));
        require(placed && placed->getSaleRefund()==game->objectData.data[Structure_Slowpost][placed->getOriginalHouseID()].price/2,
            "directly placed post credited an unpaid Scoutpost");
    }
    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
    SDL_Log("SLOWPOST UPGRADE PASS: %s, direct build gate, faction/level/IX, authorization, payment, animation and saved investment",mod.c_str());
}
