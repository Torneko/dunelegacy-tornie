#pragma once

#include <string>
#include <mod/ModFamily.h>

// Custom objects and spice families are independent rules. IDs are never removed.
struct ModContentOptions {
    bool customUnitsAndBuildings = true;
    bool greenSpice = false;
    bool redSpice = false;
    bool purpleSpice = false;
    bool blueSpice = false;

    unsigned spiceMask() const {
        return (greenSpice ? 1u : 0u) | (redSpice ? 2u : 0u)
            | (purpleSpice ? 4u : 0u) | (blueSpice ? 8u : 0u);
    }
    bool operator==(const ModContentOptions& other) const {
        return customUnitsAndBuildings == other.customUnitsAndBuildings && spiceMask() == other.spiceMask();
    }
    std::string signature() const {
        return std::to_string(customUnitsAndBuildings) + ":" + std::to_string(spiceMask());
    }
    void restrictSpices(const std::string& name) {
        if(!isTornieFamilyMod(name))return;
        const auto allowed=legacy(name);
        greenSpice &= allowed.greenSpice;redSpice &= allowed.redSpice;
        purpleSpice &= allowed.purpleSpice;blueSpice &= allowed.blueSpice;
    }
    static ModContentOptions legacy(const std::string& name) {
        ModContentOptions options;
        const bool variants = isTornieFamilyMod(name);
        const bool jericho = name == "Jericho" || name == "JerichoLite";
        options.greenSpice = variants && !jericho;
        options.redSpice = variants && jericho;
        options.blueSpice = variants && (jericho || isArcaKevMod(name));
        options.purpleSpice = variants && !jericho && !isArcaKevMod(name);
        return options;
    }
};
