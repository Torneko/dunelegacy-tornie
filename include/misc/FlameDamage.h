#pragma once
#include <cstdint>

// Preserve the fractional fifth between shots. Five shots deal exactly 120%
// of the unboosted damage, with integer combat and no additional RNG draws.
inline std::int32_t flameDamageWithBonus(std::int32_t base, std::uint8_t& remainder) {
    const std::int64_t scaled = std::int64_t(base) * 6 + remainder;
    remainder = static_cast<std::uint8_t>(scaled % 5);
    return static_cast<std::int32_t>(scaled / 5);
}
