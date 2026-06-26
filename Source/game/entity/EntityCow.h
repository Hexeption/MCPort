//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ENTITYCOW_H
#define MCPORT_ENTITYCOW_H

#include "EntityAnimal.h"

class NBTTagCompound;

class EntityCow : public EntityAnimal {
public:
    bool unusedBool2 = false;

    explicit EntityCow(World &world);

    bool interact(EntityPlayer &player) override;

    void writeEntityToNBT(NBTTagCompound &nbt) override;

    void readEntityFromNBT(NBTTagCompound &nbt) override;

protected:
    jstring getLivingSound() const override;
    jstring getHurtSound() const override;
    jstring getDeathSound() const override;
    float getSoundVolume() const override;
    int_t getDropItemId() const override;
};

#endif //MCPORT_ENTITYCOW_H
