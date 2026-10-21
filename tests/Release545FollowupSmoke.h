// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <GameInterface.h>
#include <units/GasInfantry.h>
#include <units/ChemicalSiegeTank.h>
#include <structures/Scoutpost.h>

struct HealingBalanceFixture {
    static void verify(const std::string& mod) {
        const auto require=[](bool ok,const char* why) {
            if(!ok)throw std::runtime_error(std::string("Tharpique healing: ")+why);
        };
        auto options=effectiveGameOptions;options.chaosMode=false;
        const bool full=mod=="Tornie" || mod=="Jericho" || mod=="ArcaKev";
        for(const auto identity: full ? std::vector<HOUSETYPE>{HOUSE_THARPIQUE,HOUSE_ORDOS,HOUSE_CUSTOM}
                                     : std::vector<HOUSETYPE>{HOUSE_ORDOS}) {
            auto game=std::make_unique<Game>();currentGame=game.get();
            const int house=getRuntimeHouseForIdentity(identity);
            game->initGame(chaosSmokeInit(static_cast<HOUSETYPE>(house),options));
            pLocalHouse->createUnit(Unit_MCV,true);
            auto* target=pLocalHouse->placeUnit(Unit_Tank,12,12,true);
            auto* gas=dynamic_cast<GasInfantry*>(pLocalHouse->placeUnit(Unit_GasInfantry,11,12,true));
            auto* tank=dynamic_cast<ChemicalSiegeTank*>(pLocalHouse->placeUnit(Unit_ChemicalSiegeTank,11,13,true));
            auto* post=pLocalHouse->placeStructure(NONE_ID,Structure_Chemipost,8,8,true,true);
            require(target&&gas&&tank&&post,"fixture objects");
            const bool slower=full&&identity==HOUSE_THARPIQUE;
            const int attack=gas->getWeaponReloadTime();
            target->setHealth(target->getMaxHealth()-60);
            const auto shot=[&](auto* healer) {
                const auto count=bulletList.size();healer->fireHealingMissile(target);
                require(bulletList.size()==count+1,"healing missile missing");
                require(healer->healingReloadTimer==(slower?180:120),"wrong production-faction healing recharge");
                healer->fireHealingMissile(target);
                require(bulletList.size()==count+1,"healing cooldown bypassed");
                auto* missile=bulletList.back();const auto health=target->getHealth();missile->destroy();
                require(target->getHealth()==health+15,"healing amount changed");
            };
            shot(gas);shot(tank);
            require(gas->getWeaponReloadTime()==attack,"enemy deviation reload changed");
            const int base=game->objectData.data[Structure_Chemipost][house].weaponreloadtime;
            require((mod=="vanilla" || base==150) && post->getWeaponReloadTime()==(slower?225:base),"Chemipost healing cadence");
            if(full) {
                // Changing ownership/provenance must follow the donor blueprint.
                gas->setProductionHouseID(getRuntimeHouseForIdentity(slower?HOUSE_ORDOS:HOUSE_THARPIQUE));
                gas->healingReloadTimer=0;gas->fireHealingMissile(target);
                require(gas->healingReloadTimer==(slower?120:180),"borrowed production technology healing cadence");
            }
            game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        }
        SDL_Log("RELEASE 545 HEALING PASS: %s, Tharpique +50%% reload, 15 HP, other factions and deviation unchanged",mod.c_str());
    }
};

struct AircraftShortcutFixture {
    static void verify(const std::string& output,const std::string& mod) {
        const auto require=[](bool ok,const char* why) {
            if(!ok)throw std::runtime_error(std::string("Aircraft shortcuts: ")+why);
        };
        for(bool shared:{false,true}) {
            auto options=effectiveGameOptions;options.chaosMode=false;options.alliedControl=shared;
            auto roster=getFactionSelectionOrder();
            GameInitSettings init("aircraft-shortcuts",chaosSmokeMap()+"[Player2]\nCredits=1000000\n[Player3]\nCredits=1000000\n",false,options);
            GameInitSettings::HouseInfo local(static_cast<HOUSETYPE>(roster[0]),1);
            local.addPlayerInfo(GameInitSettings::PlayerInfo(settings.general.playerName,HUMANPLAYERCLASS));init.addHouseInfo(local);
            for(int i=1;i<3;++i) {
                GameInitSettings::HouseInfo info(static_cast<HOUSETYPE>(roster[i]),i==1?1:2);
                info.addPlayerInfo(GameInitSettings::PlayerInfo("Aircraft "+std::to_string(i),"qBotEasy"));init.addHouseInfo(info);
            }
            auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
            for(int i=0;i<3;++i) {
                auto* owner=game->getHouse(roster[i]);require(owner,"aircraft house setup");owner->createUnit(Unit_MCV,true);
                require(owner->placeUnit(Unit_Ornithopter,4+i*5,10,true),"ordinary ornithopter");
                require(owner->placeUnit(Unit_EliteOrnithopter,4+i*5,12,true),"elite ornithopter");
                require(owner->placeUnit(Unit_ChemicalCarryall,4+i*5,14,true),"chemical carryall");
            }
            auto* unavailable=pLocalHouse->placeUnit(Unit_Ornithopter,4,16,true);unavailable->setRespondable(false);
            GameInterface ui;ui.updateObjectInterface();
            const int groups=shared?2:1;
            require(ui.ornithopterSelectButton.getText()==_("Ornithopter")+" ("+std::to_string(groups*2)+")"
                && ui.chemicalCarryallSelectButton.getText()==_("Chemical Carryall")+" ("+std::to_string(groups)+")",
                "parenthesized counts do not match selectable aircraft");
            for(const auto colour:{getHouseVisualHouse(pLocalHouse->getHouseID()),HOUSECOLOR_CUSTOM_APPLE_GREEN}) {
                const auto previous=getHouseVisualHouse(pLocalHouse->getHouseID());
                setHouseVisualHouse(static_cast<HOUSETYPE>(pLocalHouse->getHouseID()),colour);ui.refreshAircraftButtons();
                require(ui.ornithopterSelectButton.getTextColor()==getHouseColorRGB(colour,3)
                    && ui.chemicalCarryallSelectButton.getTextColor()==getHouseColorRGB(colour,3),"faction/bonus text colour");
                setHouseVisualHouse(static_cast<HOUSETYPE>(pLocalHouse->getHouseID()),previous);
            }
            ui.updateObjectInterface();ui.draw(Point(0,0));
            auto picture=renderReadSurface(renderer);
            SavePNG(picture.get(),(std::filesystem::path(output)/(mod+"-aircraft-counts-"+(shared?"shared":"owned")+".png")).string().c_str());
            require(ui.ornithopterSelectButton.isVisible() && ui.chemicalCarryallSelectButton.isVisible(),"shortcuts hidden with no selection");
            game->selectAllOrnithopters();ui.updateObjectInterface();
            require(game->getSelectedList().size()==static_cast<size_t>(groups*2) && !ui.ornithopterSelectButton.isVisible()
                && !ui.chemicalCarryallSelectButton.isVisible(),"selection differs from count or shortcuts remain visible");
            game->selectAllChemicalCarryalls();require(game->getSelectedList().size()==static_cast<size_t>(groups),"chemical selection differs from count");
            game->unselectAll(game->getSelectedList());game->getSelectedList().clear();
            auto* extra=pLocalHouse->placeUnit(Unit_ChemicalCarryall,4,18,true);require(extra,"new aircraft");
            ui.updateObjectInterface();
            require(ui.chemicalCarryallSelectButton.getText()==_("Chemical Carryall")+" ("+std::to_string(groups+1)+")",
                "count did not refresh after deselection");
            // Destroy the interface before the game to release its selected-object container.
            game->getSelectedList().clear();
        }
        currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
        SDL_Log("RELEASE 545 AIRCRAFT PASS: %s, no-selection shortcuts, live parenthesized counts, faction/bonus colours and allied control",mod.c_str());
    }
};
