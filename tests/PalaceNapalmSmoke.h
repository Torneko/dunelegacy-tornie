#pragma once
// SPDX-License-Identifier: GPL-2.0-or-later
#include <structures/Palace.h>
#include <cstdlib>

struct PalaceNapalmFixture {
    static void draw(Palace& palace) {
        palace.specialWeaponTimer = 0;
        palace.updateStructureSpecificStuff();
    }
};

inline void verifyRandomPalaceNapalm(const std::string& output, const std::string& mod) {
    const auto require = [](bool ok, const char* why) {
        if(!ok) { SDL_Log("PALACE NAPALM FAIL: %s", why); std::exit(EXIT_FAILURE); }
    };
    auto options = effectiveGameOptions; options.chaosMode = false;
    auto game = std::make_unique<Game>(); currentGame = game.get();
    game->initGame(chaosSmokeInit(getRuntimeHouseForIdentity(HOUSE_ATREIDES), options));
    pLocalHouse->createUnit(Unit_MCV, true);
    auto* palace = dynamic_cast<Palace*>(pLocalHouse->placeStructure(NONE_ID, Structure_Palace, 4, 4, true, true));
    require(palace != nullptr, "palace placement");
    // A captured/random-technology Palace must offer the same power to another owner.
    palace->setOriginalHouseID(getRuntimeHouseForIdentity(HOUSE_REBELS));
    const bool expanded = mod == "Tornie" || mod == "Jericho" || mod == "ArcaKev";
    if(palace->usesTornieMainRebelsRandomSpecial()) {
        std::set<Palace::TornieRebelsSpecialWeapon> pool;
        for(int draw = 0; draw < 256; ++draw) {
            PalaceNapalmFixture::draw(*palace);
            pool.insert(palace->getTornieMainRebelsSpecialWeapon());
        }
        require(pool.size() == (expanded ? 6u : 3u), "random Palace pool has missing or unexpected powers");
        require(pool.count(Palace::TornieRebelsSpecialWeapon::Napalm) == (expanded ? 1u : 0u), "napalm is missing or enabled in Vanilla");
        if(expanded) {
            for(int draw = 0; draw < 256 && !palace->usesKleshmershNapalm(); ++draw) PalaceNapalmFixture::draw(*palace);
            require(palace->usesKleshmershNapalm() && palace->usesTargetedSpecialWeapon(), "random napalm is not targetable");
            const auto id = palace->getObjectID();
            const auto file = (std::filesystem::path(output) / (mod + "-random-napalm.dls")).string();
            require(game->saveGame(file), "random napalm save");
            game.reset(); currentGame = nullptr; pLocalHouse = nullptr; pLocalPlayer = nullptr;
            game = std::make_unique<Game>(); currentGame = game.get(); game->initGame(GameInitSettings(file));
            palace = dynamic_cast<Palace*>(game->getObjectManager().getObject(id));
            require(palace && palace->usesKleshmershNapalm() && palace->usesTargetedSpecialWeapon(), "selected napalm lost after reload");
            const auto cooldown = palace->getMaxSpecialWeaponTimer();
            const auto effectsBefore = game->getExplosionList().size();
            Command launch(pLocalPlayer->getPlayerID(), CMD_PALACE_DEATHHAND, id, 16, 16);
            OMemoryStream wire; launch.save(wire);
            IMemoryStream bytes(wire.getData(), wire.getDataLength()); Command received(bytes); received.executeCommand();
            require(!bulletList.empty() && bulletList.back()->getBulletID() == Bullet_Napalm, "random Palace launched a regular missile");
            require(palace->getSpecialWeaponTimer() == cooldown, "random Palace recharge changed after napalm");
            auto* missile = bulletList.back();
            const auto inFlight = [&] {
                for(auto* bullet : bulletList) if(bullet == missile) return true;
                return false;
            };
            for(int tick = 0; tick < 500; ++tick) {
                if(!inFlight()) break;
                missile->update();
                if(inFlight())
                    require(game->getExplosionList().size() == effectsBefore, "napalm ignites the flight route");
            }
            require(!inFlight(), "napalm never impacts");
            require(game->getExplosionList().size() == effectsBefore + 9, "napalm impact is not confined to its 3x3 area");
        }
    } else require(!expanded, "expanded random Palace unavailable");
    game.reset(); currentGame = nullptr; pLocalHouse = nullptr; pLocalPlayer = nullptr;
    SDL_Log("PALACE NAPALM PASS: %s, random pool, Vanilla exclusion, target command, saved power and impact-only fire", mod.c_str());
}
