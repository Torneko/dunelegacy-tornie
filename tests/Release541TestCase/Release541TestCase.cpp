#include <catch2/catch_all.hpp>
#include <misc/FlameDamage.h>
#include <misc/SpiceGeneration.h>
#include <mod/ModContentOptions.h>

TEST_CASE("Flame Troopers gain exactly twenty percent damage without combat RNG", "[combat][flame]") {
    for(const int base : {1, 5, 6, 7, 11, 29, 100}) {
        std::uint8_t remainder = 0;
        int total = 0;
        for(int shot = 0; shot < 5; ++shot) {
            total += flameDamageWithBonus(base, remainder);
            REQUIRE(remainder < 5);
        }
        REQUIRE(total == base * 6);
        REQUIRE(remainder == 0);
    }
}

TEST_CASE("A resumed flame damage fraction preserves the next shots", "[combat][save]") {
    std::uint8_t uninterrupted = 0;
    flameDamageWithBonus(6, uninterrupted);
    flameDamageWithBonus(6, uninterrupted);
    auto restored = uninterrupted;
    for(int shot = 0; shot < 8; ++shot)
        REQUIRE(flameDamageWithBonus(6, uninterrupted) == flameDamageWithBonus(6, restored));
}

TEST_CASE("ArcaKev defaults keep normal green and blue spice with custom content", "[mods][spice]") {
    for(const std::string name : {"ArcaKev", "ArcaKevLite"}) {
        const auto content = ModContentOptions::legacy(name);
        REQUIRE(content.customUnitsAndBuildings);
        REQUIRE(content.spiceMask() == 9);
        REQUIRE(allowedSpiceTerrain(Terrain_Spice, content.spiceMask()) == Terrain_Spice);
        REQUIRE(allowedSpiceTerrain(Terrain_GreenSpice, content.spiceMask()) == Terrain_GreenSpice);
        REQUIRE(allowedSpiceTerrain(Terrain_WhiteSpice, content.spiceMask()) == Terrain_WhiteSpice);
        REQUIRE(allowedSpiceTerrain(Terrain_RedSpice, content.spiceMask()) == Terrain_Spice);
        REQUIRE(allowedSpiceTerrain(Terrain_PaleLilacSpice, content.spiceMask()) == Terrain_Spice);
    }
    REQUIRE(isLiteMod("ArcaKevLite"));
    REQUIRE_FALSE(isLiteMod("ArcaKev"));
    REQUIRE(ModContentOptions::legacy("Jericho").spiceMask() == 10);
}

#include <misc/CampaignTechLevel.h>
TEST_CASE("Campaign and skirmish finale unlock technology nine", "[campaign][technology]") {
    REQUIRE(campaignTechLevel(1)==1);
    REQUIRE(campaignTechLevel(2)==2);
    REQUIRE(campaignTechLevel(19)==7);
    REQUIRE(campaignTechLevel(20)==8);
    REQUIRE(campaignTechLevel(21)==8);
    REQUIRE(campaignTechLevel(22)==9);
}

TEST_CASE("Mod spice palettes contain exactly their two declared colours", "[mods][spice]") {
    for(const auto name:{"Tornie","TornieLite"}) REQUIRE(ModContentOptions::legacy(name).spiceMask()==5);
    for(const auto name:{"Jericho","JerichoLite"}) REQUIRE(ModContentOptions::legacy(name).spiceMask()==10);
    REQUIRE(ModContentOptions::legacy("vanilla").spiceMask()==0);
}

TEST_CASE("Old all-colour preferences cannot enable unsupported mod spices", "[mods][spice]") {
    for(const std::string name:{"Tornie","TornieLite","Jericho","JerichoLite","ArcaKev","ArcaKevLite"}) {
        ModContentOptions old;old.greenSpice=old.redSpice=old.purpleSpice=old.blueSpice=true;
        old.customUnitsAndBuildings=false;old.restrictSpices(name);
        REQUIRE(old.spiceMask()==ModContentOptions::legacy(name).spiceMask());
        REQUIRE_FALSE(old.customUnitsAndBuildings);
        old.greenSpice=old.redSpice=old.purpleSpice=old.blueSpice=false;old.restrictSpices(name);REQUIRE(old.spiceMask()==0);
    }
}
