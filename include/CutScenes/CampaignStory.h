// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <DataTypes.h>
#include <FileClasses/Palette.h>
#include <array>
#include <string>
class Wsafile;

// Story roles follow the campaign template, independently of runtime house IDs
// and the player's optional colour. No global palette or source resource changes.
struct CampaignStory {
    explicit CampaignStory(int house);
    int player;
    int model;
    std::array<int,NUM_HOUSES> roles;
    Palette scenePalette() const;
    std::string text(const std::string& original) const;
    void apply(Wsafile* animation) const;
};
