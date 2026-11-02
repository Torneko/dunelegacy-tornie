#ifndef SUPERTANKRENDER_H
#define SUPERTANKRENDER_H

#include <DataTypes.h>
#include <array>

// Drawing anchors for Tornie's original 16-pixel turret frames. The same
// anchors are used by the game, map editor and composed palette thumbnails.
namespace SuperTankRender {
inline const std::array<Coord, NUM_ANGLES>& nativeTurretOffsets() {
    static const std::array<Coord, NUM_ANGLES> offsets{{
        {2,1}, {1,-1}, {0,-2}, {0,-1}, {-2,1}, {-1,1}, {0,1}, {2,1}
    }};
    return offsets;
}

inline const std::array<Coord, NUM_ANGLES>& worldTurretOffsets() {
    static_assert(TILESIZE % 16 == 0, "Native sprite/world coordinate scale");
    static const auto offsets=[] {
        std::array<Coord, NUM_ANGLES> result;
        for(int angle=0;angle<NUM_ANGLES;++angle) {
            const auto p=nativeTurretOffsets()[angle];
            result[angle]=Coord(p.x*(TILESIZE/16),p.y*(TILESIZE/16));
        }
        return result;
    }();
    return offsets;
}

inline Coord editorOffset() {return nativeTurretOffsets()[0];}
}

#endif
