#pragma once

constexpr int campaignTechLevel(int mission) {
    return mission>=22 ? 9 : (mission+1)/3+1;
}
