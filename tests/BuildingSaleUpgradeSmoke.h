#pragma once
// SPDX-License-Identifier: GPL-2.0-or-later
#include <Campaign/CoopCampaignRuntime.h>
#include <structures/BuilderBase.h>
#include <structures/Scoutpost.h>
#include <structures/WindTrap.h>
#include <cstdlib>

inline void verifyUpgradedBuildingSales(const std::string& output, const std::string& mod) {
    const auto require = [](bool ok, const std::string& why) {
        if(!ok) { SDL_Log("UPGRADED SALE FAIL: %s", why.c_str()); std::exit(EXIT_FAILURE); }
    };
    const auto roster = getFactionSelectionOrder();
    const auto present = [&](int identity) {
        return std::any_of(roster.begin(), roster.end(), [&](int house) {
            return isHouseFaction(static_cast<HOUSETYPE>(house), static_cast<HOUSETYPE>(identity));
        });
    };
    struct Case { int faction, item, converted; };
    const Case cases[] = {
        {HOUSE_ATREIDES, Structure_LightFactory, ItemID_Invalid},
        {HOUSE_ATREIDES, Structure_HeavyFactory, ItemID_Invalid},
        {HOUSE_WILDSPADE, Structure_HighTechFactory, ItemID_Invalid},
        {HOUSE_REBELS, Structure_WindTrap, ItemID_Invalid},
        {HOUSE_KLESHMERSH, Structure_Scoutpost, Structure_Flamepost},
        {HOUSE_THARPIQUE, Structure_Scoutpost, Structure_Chemipost},
        {HOUSE_NEUTRAL, Structure_Scoutpost, Structure_Slowpost}
    };
    int checked = 0;
    for(const bool easy : {false, true}) for(const auto& test : cases) {
        if(!present(test.faction) || (mod == "vanilla" && (test.item == Structure_WindTrap || test.converted != ItemID_Invalid))) continue;
        auto options = effectiveGameOptions;
        options.chaosMode = false; options.instantBuild = true;
        options.sellingBuildings = true; options.easyMode = easy;
        const auto faction = getRuntimeHouseForIdentity(static_cast<HOUSETYPE>(test.faction));
        const auto second = roster.front() == faction ? roster[1] : roster.front();
        auto session = coop::CoopCampaignSession::create("upgraded-sale-check", mod, roster,
            {faction, second}, {settings.general.playerName, "Sale Ally"}, 31415, false, "legacy");
        session.setEasyMode(easy);
        std::string map = "[BASIC]\nVersion=2\nTechLevel=1\n[COOP_TEMPLATE]\nStage=1\n[MAP]\nSizeX=64\nSizeY=48\n";
        for(int y = 0; y < 48; ++y) map += fmt::sprintf("%03d=", y) + std::string(64, '%') + "\n";
        for(std::size_t i = 0; i < roster.size(); ++i)
            map += "[Player" + std::to_string(i + 1) + "]\nCredits=1000000\n";
        map = session.prepareMap(map);
        const auto context = coop::readContext(map);
        require(context.has_value(), "missing co-op context");
        GameInitSettings init("upgraded-sale-check", map, "Upgraded sale check", false, options);
        for(const auto& slot : context->slots) if(slot.active()) {
            GameInitSettings::HouseInfo info(static_cast<HOUSETYPE>(slot.house), slot.team());
            info.addPlayerInfo(GameInitSettings::PlayerInfo(slot.playerName, slot.playerClass));
            init.addHouseInfo(info);
        }
        auto game = std::make_unique<Game>(); currentGame = game.get(); game->initGame(init); game->techLevel = 9;
        auto* owner = pLocalHouse;
        require(owner && game->getGameInitSettings().isEasyModeHouse(owner->getHouseID()) == easy, "difficulty fixture");
        owner->createUnit(Unit_MCV, true); owner->addUnstoredCredits(100000);
        owner->placeStructure(NONE_ID, Structure_IX, 25, 2, true, true);
        auto* structure = owner->placeStructure(NONE_ID, test.item, 4, 4, true, true);
        require(structure != nullptr, "building placement");
        const auto id = structure->getObjectID();
        const int base = game->getGameInitSettings().campaignPurchasePrice(
            game->objectData.data[test.item][structure->getOriginalHouseID()].price, owner->getHouseID());
        const int fundsBefore = owner->getCredits();
        int partial = 0;
        if(auto* builder = dynamic_cast<BuilderBase*>(structure)) {
            const int levels = std::min(3, builder->getMaxUpgradeLevel());
            require(levels > 0, "no factory upgrades available");
            for(int level = 0; level < levels; ++level) {
                require(builder->doUpgrade(), "factory upgrade refused");
                builder->update();
                require(!builder->isUpgrading() && builder->getCurrentUpgradeLevel() == level + 1, "factory upgrade incomplete");
                const int spent = fundsBefore - owner->getCredits();
                require(builder->getSaleRefund() == (base + spent) * (easy ? 75 : 50) / 100,
                    mod + ": sale omits completed factory upgrades");
            }
        } else if(auto* trap = dynamic_cast<WindTrap*>(structure)) {
            require(trap->doPowerUpgrade(), "Rebels power upgrade refused");
            require(!trap->doPowerUpgrade(), "Rebels upgrade applied twice");
        } else if(auto* post = dynamic_cast<Scoutpost*>(structure)) {
            if(test.converted == Structure_Flamepost) post->doUpgradeToFlamepost();
            else if(test.converted == Structure_Chemipost) post->doUpgradeToChemipost();
            else post->doUpgradeToSlowpost();
            require(post->getItemID() == static_cast<Uint32>(test.converted), "Scoutpost conversion refused");
        }
        const int invested = base + fundsBefore - owner->getCredits();
        const int expected = invested * (easy ? 75 : 50) / 100;
        require(structure->getSaleRefund() == expected, mod + ": sale omits a completed upgrade or discounts it twice");
        structure->setHealth(structure->getMaxHealth() / 2);
        require(structure->getSaleRefund() == expected, "damage changes total invested sale value");
        const auto file = (std::filesystem::path(output) / (mod + "-upgraded-sale-" + std::to_string(test.item)
            + "-" + std::to_string(test.faction) + (easy ? "-easy.dls" : "-normal.dls"))).string();
        require(game->saveGame(file), "upgraded building save");
        game.reset(); currentGame = nullptr; pLocalHouse = nullptr; pLocalPlayer = nullptr;
        game = std::make_unique<Game>(); currentGame = game.get();
        GameInitSettings load(getBasename(file, true), readCompleteFile(file), "Upgraded sale load");
        for(const auto& house : init.getHouseInfoList()) load.addHouseInfo(house);
        game->initGame(load);
        structure = dynamic_cast<StructureBase*>(game->getObjectManager().getObject(id)); owner = structure ? structure->getOwner() : nullptr;
        require(structure && structure->getSaleRefund() == expected, "upgrade value lost on save/reload");
        if(auto* builder = dynamic_cast<BuilderBase*>(structure)) {
            // Starve an instant upgrade after starting it: its paid progress is
            // cancelled at face value, never counted again as a completed level.
            if(builder->getCurrentUpgradeLevel() > 1) {
                // A separate fresh factory has room for a pending upgrade.
                auto* pending = dynamic_cast<BuilderBase*>(owner->placeStructure(NONE_ID, test.item, 12, 4, true, true));
                require(pending && pending->doUpgrade(), "pending factory upgrade refused");
                owner->takeCredits(owner->getCredits() - 11); pending->update();
                require(pending->isUpgrading() && pending->getCurrentUpgradeLevel() == 0, "partial upgrade completed unexpectedly");
                partial = lround(pending->getUpgradeProgress());
                require(partial == 11 && pending->getSaleRefund() == base * (easy ? 75 : 50) / 100, "pending upgrade counted as completed");
                const int before = owner->getCredits(); pending->doSell();
                require(owner->getCredits() == before + partial + base * (easy ? 75 : 50) / 100, "pending upgrade refund lost or counted twice");
            }
        }
        const int credits = owner->getCredits(), saleCredits = owner->getSaleCredits();
        structure->doSell();
        require(owner->getCredits() == credits + expected, "sale credited the wrong total");
        require(owner->getSaleCredits() == saleCredits + expected, "sale statistics omit upgrade value");
        game.reset(); currentGame = nullptr; pLocalHouse = nullptr; pLocalPlayer = nullptr;
        ++checked;
    }
    SDL_Log("UPGRADED SALE PASS: %s, %d cases, completed costs, partial refunds, damage, save/reload and Easy co-op", mod.c_str(), checked);
}
