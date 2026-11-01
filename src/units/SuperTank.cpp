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

#include <units/SuperTank.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <House.h>
#include <Game.h>
#include <Map.h>
#include <Explosion.h>
#include <ScreenBorder.h>
#include <SoundPlayer.h>

SuperTank::SuperTank(House* newOwner) : TankBase(newOwner) {
    SuperTank::init();

    setHealth(getMaxHealth());
}

SuperTank::SuperTank(InputStream& stream) : TankBase(stream) {
    SuperTank::init();
}

void SuperTank::init() {
    itemID = Unit_SuperTank;
    owner->incrementUnits(itemID);

    numWeapons = 2;
    bulletType = Bullet_Flame;

    graphicID = ObjPic_Siegetank_Base;
    graphic = pGFXManager->getObjPic(graphicID,getOwner()->getHouseID());
    gunGraphicID = ObjPic_SuperTankGun;
    turretGraphic = pGFXManager->getObjPic(gunGraphicID,getOwner()->getHouseID());

    numImagesX = NUM_ANGLES;
    numImagesY = 1;
}

SuperTank::~SuperTank() = default;

void SuperTank::blitToScreen() {
    int x1 = screenborder->world2screenX(realX);
    int y1 = screenborder->world2screenY(realY);

    SDL_Texture* pUnitGraphic = graphic[currentZoomlevel];
    SDL_Rect source1 = calcSpriteSourceRect(pUnitGraphic, drawnAngle, numImagesX);
    SDL_Rect dest1 = calcSpriteDrawingRect( pUnitGraphic, x1, y1, numImagesX, 1, HAlign::Center, VAlign::Center);

    SDL_RenderCopy(renderer, pUnitGraphic, &source1, &dest1);

    const Coord siegeTankTurretOffset[] =   {   Coord(8, -12),
                                                Coord(0, -20),
                                                Coord(0, -20),
                                                Coord(-4, -20),
                                                Coord(-8, -12),
                                                Coord(-8, -4),
                                                Coord(-4, -12),
                                                Coord(8, -4)
                                            };

    SDL_Texture* pTurretGraphic = turretGraphic[currentZoomlevel];
    SDL_Rect source2 = calcSpriteSourceRect(pTurretGraphic, drawnTurretAngle, NUM_ANGLES);
    SDL_Rect dest2 = calcSpriteDrawingRect( pTurretGraphic,
                                            screenborder->world2screenX(realX + siegeTankTurretOffset[drawnTurretAngle].x),
                                            screenborder->world2screenY(realY + siegeTankTurretOffset[drawnTurretAngle].y),
                                            NUM_ANGLES, 1, HAlign::Center, VAlign::Center);

    SDL_RenderCopy(renderer, pTurretGraphic, &source2, &dest2);

    if(isBadlyDamaged()) {
        drawSmoke(x1, y1);
    }
}

void SuperTank::destroy()
{
    if(currentGameMap->tileExists(location) && isVisible()) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                Coord realPos(lround(realX) + (i - 1)*TILESIZE, lround(realY) + (j - 1)*TILESIZE);

                currentGameMap->damage(objectID, owner, realPos, itemID, owner->radarAttackDamage(itemID,150), 16, false);

                Uint32 explosionID = currentGame->randomGen.getRandOf({Explosion_Large1, Explosion_Large2});
                currentGame->getExplosionList().push_back(new Explosion(explosionID, realPos, owner->getHouseID()));
            }
        }

        if(isVisible(getOwner()->getTeamID())) {
            screenborder->shakeScreen(18);
            soundPlayer->playSoundAt(Sound_ExplosionLarge,location);
        }
    }

    TankBase::destroy();
}

void SuperTank::playAttackSound() {
    soundPlayer->playSoundAt(Sound_Rocket, location);
}
