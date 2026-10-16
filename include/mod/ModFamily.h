#pragma once
#include <string>

inline bool isArcaKevMod(const std::string& name) {
    return name == "ArcaKev" || name == "ArcaKevLite";
}
inline bool isTornieFamilyMod(const std::string& name) {
    return name == "Tornie" || name == "TornieLite" || name == "Jericho"
        || name == "JerichoLite" || isArcaKevMod(name);
}
inline bool isLiteMod(const std::string& name) {
    return name == "TornieLite" || name == "JerichoLite" || name == "ArcaKevLite";
}
