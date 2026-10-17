/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 */

#pragma once
#include <units/InfantryBase.h>
#include <ObjectPointer.h>

// IX infantry: Deviator gas against enemies, healing missiles for allied units.
class GasInfantry final : public InfantryBase {
public:
    explicit GasInfantry(House* owner);
    explicit GasInfantry(InputStream& stream);
    void save(OutputStream& stream) const override;
    bool update() override;
    void doAttackObject(Uint32 targetObjectID, bool forced) override;
    bool canAttack(const ObjectBase* object) const override;
    bool canHeal() const override { return true; }
    void playAttackSound() override;
private:
    void init();
    bool isValidHealTarget(const ObjectBase* object) const;
    UnitBase* findDamagedAlly() const;
    void fireHealingMissile(UnitBase* unit);
    Sint32 healingReloadTimer = 0;
    ObjectPointer healingTarget;
    bool manualHealing = false;
};
