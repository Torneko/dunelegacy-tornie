/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FLAMEQUAD_H
#define FLAMEQUAD_H

#include <units/GroundUnit.h>

/// Rocket Trike — House Neutral light vehicle armed with trooper-style rockets.
/// Uses the supplied FlameQuadTiles.png atlas and fires
/// Bullet_Flame projectiles.
class FlameQuad final : public GroundUnit
{
public:
    explicit FlameQuad(House* newOwner);
    explicit FlameQuad(InputStream& stream);
    void init();
    virtual ~FlameQuad();

    void destroy() override;

    bool hasBumpyMovementOnRock() const override { return true; }

    void playAttackSound() override;
};

#endif //FLAMEQUAD_H
