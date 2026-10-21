/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 */

#include <units/GasInfantry.h>
#include <mod/HealingBalance.h>
#include <FileClasses/GFXManager.h>
#include <globals.h>
#include <Game.h>
#include <House.h>
#include <Bullet.h>
#include <SoundPlayer.h>
#include <misc/InputStream.h>
#include <misc/OutputStream.h>

GasInfantry::GasInfantry(House* owner) : InfantryBase(owner) { init();setHealth(getMaxHealth()); }
GasInfantry::GasInfantry(InputStream& stream) : InfantryBase(stream) {
    init();healingReloadTimer=stream.readSint32();healingTarget.load(stream);manualHealing=stream.readBool();
}
void GasInfantry::save(OutputStream& stream) const {
    InfantryBase::save(stream);stream.writeSint32(healingReloadTimer);healingTarget.save(stream);stream.writeBool(manualHealing);
}
void GasInfantry::init() {
    itemID=Unit_GasInfantry;owner->incrementUnits(itemID);numWeapons=1;bulletType=Bullet_DRocket;
    graphicID=ObjPic_GasInfantry;graphic=pGFXManager->getObjPic(graphicID,getOwner()->getHouseID());numImagesX=4;numImagesY=3;
}
bool GasInfantry::canAttack(const ObjectBase* object) const {
    return object && !object->isAStructure() && object->getHealth()>0 && object->getOwner()->getTeamID()!=owner->getTeamID() && object->isVisible(owner->getTeamID()) && !object->isAFlyingUnit();
}
void GasInfantry::playAttackSound() {soundPlayer->playSoundAt(Sound_RocketSmall,location);}

namespace {
constexpr int GAS_INFANTRY_HEAL_AMOUNT = 15;
constexpr int GAS_INFANTRY_HEAL_RELOAD = 120;
}

bool GasInfantry::isValidHealTarget(const ObjectBase* pObject) const
{
    const auto* pUnit = dynamic_cast<const UnitBase*>(pObject);
    return pUnit != nullptr && pUnit != this && pUnit->getHealth() > 0
        && pUnit->getHealth() < pUnit->getMaxHealth()
        && pUnit->getOwner()->getTeamID() == owner->getTeamID();
}

UnitBase* GasInfantry::findDamagedAlly() const
{
    UnitBase* pBest = nullptr;
    int bestDistance = getAreaGuardRange() + 1;
    for(UnitBase* pUnit : unitList) {
        if(!isValidHealTarget(pUnit)) {
            continue;
        }
        const Coord targetLocation = pUnit->getLocation();
        const int guardDistance = std::max(std::abs(targetLocation.x - guardPoint.x), std::abs(targetLocation.y - guardPoint.y));
        if(guardDistance > getAreaGuardRange()) {
            continue;
        }
        const int distance = std::max(std::abs(targetLocation.x - location.x), std::abs(targetLocation.y - location.y));
        if(distance < bestDistance) {
            bestDistance = distance;
            pBest = pUnit;
        }
    }
    return pBest;
}

void GasInfantry::fireHealingMissile(UnitBase* pTarget)
{
    if(pTarget == nullptr || healingReloadTimer > 0) {
        return;
    }
    Coord source = getCenterPoint();
    Coord destination = pTarget->getCenterPoint();
    bulletList.push_back(new Bullet(objectID, &source, &destination, Bullet_Heal,
                                    GAS_INFANTRY_HEAL_AMOUNT, pTarget->isAFlyingUnit(), pTarget));
    healingReloadTimer = healing::reload(GAS_INFANTRY_HEAL_RELOAD, getProductionHouseID());
}

void GasInfantry::doAttackObject(Uint32 targetObjectID, bool bForced)
{
    ObjectBase* pObject = currentGame->getObjectManager().getObject(targetObjectID);
    if(isValidHealTarget(pObject)) {
        healingTarget.pointTo(pObject);
        target.pointTo(nullptr);
        forced = false;
        manualHealing = true;
        return;
    }
    healingTarget.pointTo(nullptr);
    manualHealing = false;
    InfantryBase::doAttackObject(targetObjectID, bForced);
}

bool GasInfantry::update()
{
    if(!InfantryBase::update()) {
        return false;
    }
    if(healingReloadTimer > 0) {
        --healingReloadTimer;
    }
    if(wasForced() || hasATarget()) {
        healingTarget.pointTo(nullptr);
        manualHealing = false;
        return true;
    }

    UnitBase* pHealTarget = dynamic_cast<UnitBase*>(healingTarget.getObjPointer());
    if(!isValidHealTarget(pHealTarget)) {
        healingTarget.pointTo(nullptr);
        manualHealing = false;
        if(getAttackMode() == AREAGUARD) {
            pHealTarget = findDamagedAlly();
            healingTarget.pointTo(pHealTarget);
        }
    }
    if(pHealTarget == nullptr) {
        return true;
    }

    const Coord targetLocation = pHealTarget->getLocation();
    const int distance = std::max(std::abs(targetLocation.x - location.x), std::abs(targetLocation.y - location.y));
    if(distance <= getWeaponRange()) {
        setDestination(location);
        fireHealingMissile(pHealTarget);
    } else {
        setDestination(targetLocation);
    }
    return true;
}
