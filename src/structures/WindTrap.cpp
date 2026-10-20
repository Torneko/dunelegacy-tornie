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

#include <structures/WindTrap.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <House.h>
#include <Game.h>
#include <mod/ModManager.h>
#include <Command.h>
#include <players/HumanPlayer.h>

#include <GUI/ObjectInterfaces/WindTrapInterface.h>

WindTrap::WindTrap(House* newOwner) : StructureBase(newOwner) {
    WindTrap::init();

    setHealth(getMaxHealth());
}

WindTrap::WindTrap(InputStream& stream) : StructureBase(stream) {
    if(currentGame->getLoadedSavegameVersion()>=9834)powerUpgraded=stream.readBool();
    WindTrap::init();
}

void WindTrap::init() {
    itemID = Structure_WindTrap;
    owner->incrementStructures(itemID);

    structureSize.x = 2;
    structureSize.y = 2;

    graphicID = ObjPic_Windtrap;
    graphic = pGFXManager->getObjPic(graphicID,getOwner()->getHouseID());
    numImagesX = NUM_WINDTRAP_ANIMATIONS_PER_ROW;
    numImagesY = (2+NUM_WINDTRAP_ANIMATIONS+NUM_WINDTRAP_ANIMATIONS_PER_ROW-1)/NUM_WINDTRAP_ANIMATIONS_PER_ROW;
    firstAnimFrame = 2;
    lastAnimFrame = 2+NUM_WINDTRAP_ANIMATIONS-1;
}

WindTrap::~WindTrap() = default;

ObjectInterface* WindTrap::getInterfaceContainer() {
    if(currentGame->canControlHouse(owner) || (debug == true)) {
        return WindTrapInterface::create(objectID);
    } else {
        return DefaultObjectInterface::create(objectID);
    }
}

bool WindTrap::update() {
    bool bResult = StructureBase::update();

    if(bResult) {
        // we are still alive
        if(justPlacedTimer <= 0 || curAnimFrame != 0) {
            curAnimFrame = 2 + ((currentGame->getGameCycleCount()/8) % NUM_WINDTRAP_ANIMATIONS);
        }
    }

    return bResult;
}

void WindTrap::setHealth(FixPoint newHealth) {
    int producedPowerBefore = getProducedPower();
    StructureBase::setHealth(newHealth);
    int producedPowerAfterwards = getProducedPower();

    owner->setProducedPower(owner->getProducedPower() - producedPowerBefore + producedPowerAfterwards);
}

int WindTrap::getProducedPower() const {
    int windTrapProducedPower = abs(currentGame->objectData.data[Structure_WindTrap][originalHouseID].power);

    FixPoint ratio = getHealth() / getMaxHealth();
    return lround(ratio * windTrapProducedPower * (powerUpgraded ? 2 : 1));
}

void WindTrap::save(OutputStream& stream) const { StructureBase::save(stream);stream.writeBool(powerUpgraded); }

bool WindTrap::supportsPowerUpgrade(HOUSETYPE house) {
    return ModManager::instance().isTornieContentActive() && isHouseFaction(house,HOUSE_REBELS);
}
bool WindTrap::isPowerUpgradeEligible() const {
    return !powerUpgraded && currentGame && owner
        && supportsPowerUpgrade(static_cast<HOUSETYPE>(getTechnologyHouseID()))
        && currentGame->techLevel>=7 && owner->getNumItems(Structure_IX)>0;
}
void WindTrap::handlePowerUpgradeClick() {
    if(currentGame && pLocalPlayer)currentGame->getCommandManager().addCommand(
        Command(pLocalPlayer->getPlayerID(),CMD_WINDTRAP_POWER_UPGRADE,objectID));
}
bool WindTrap::doPowerUpgrade() {
    if(!isPowerUpgradeEligible() || owner->getCredits()<150)return false;
    owner->takeCredits(150);inheritPowerUpgrade(true);
    return true;
}

void WindTrap::inheritPowerUpgrade(bool upgraded) {
    const int before=getProducedPower();powerUpgraded=upgraded;
    owner->setProducedPower(owner->getProducedPower()-before+getProducedPower());
}
