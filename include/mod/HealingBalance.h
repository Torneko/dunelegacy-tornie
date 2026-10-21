// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <globals.h>
#include <mod/ModManager.h>

namespace healing {
// Follow the production blueprint, including captured/Chaos technology. Enemy
// deviation weapons have their own reload and are deliberately unaffected.
inline int reload(int base, int technologyHouse) {
    return ModManager::instance().isTornieContentActive()
        && isHouseFaction(static_cast<HOUSETYPE>(technologyHouse), HOUSE_THARPIQUE)
        ? base * 3 / 2 : base;
}
}
