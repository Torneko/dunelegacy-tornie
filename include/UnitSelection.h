#pragma once
#include <data.h>

inline bool isHarvesterSelectionItem(int item) {
    return item==Unit_Harvester || item==Unit_RebelHarvester
        || item==Unit_CapacityHarvester || item==Unit_FastHarvester;
}

inline bool isArmySelectionItem(int item) {
    return isUnit(item) && !isHarvesterSelectionItem(item)
        && !isCarryallUnit(item) && item!=Unit_Frigate && item!=Unit_MCV
        && item!=Unit_Sandworm && item!=Unit_Special;
}
