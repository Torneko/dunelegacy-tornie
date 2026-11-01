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

#ifndef SUPERTANK_H
#define SUPERTANK_H

#include <units/TankBase.h>

// Elite Siege Tank â€” Tornie mod only. Uses same Siege Tank tile graphics but with upgraded stats.
class SuperTank final : public TankBase
{
public:
    friend struct Release547Fixture;
    explicit SuperTank(House* newOwner);
    explicit SuperTank(InputStream& stream);
    void init();
    virtual ~SuperTank();

    void blitToScreen() override;

    void destroy() override;

    void playAttackSound() override;
};

#endif // SUPERTANK_H
