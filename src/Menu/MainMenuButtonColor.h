#pragma once

#include <globals.h>
#include <GUI/GUIStyle.h>
#include <mod/ModManager.h>

namespace MainMenuButtonColor {

inline Uint32 colorForMod(const std::string& mod) {
    // Keep menu colours stable across faction orders and bonus colour slots.
    if(mod == "Tornie" || mod == "TornieLite") return COLOR_RGB(20,68,28);
    if(mod == "Jericho" || mod == "JerichoLite") return COLOR_RGB(52,52,52);
    if(mod == "ArcaKev" || mod == "ArcaKevLite") return COLOR_RGB(176,64,24);
    return COLOR_DEFAULT;
}

template<typename ButtonWidget>
inline void apply(ButtonWidget& button) {
    ModManager& modManager = ModManager::instance();
    const std::string mod = modManager.isInitialized() ? modManager.getActiveModName() : "vanilla";
    button.setTextColor(colorForMod(mod),
        (mod == "ArcaKev" || mod == "ArcaKevLite") ? COLOR_BLACK : COLOR_DEFAULT);
}

} // namespace MainMenuButtonColor
