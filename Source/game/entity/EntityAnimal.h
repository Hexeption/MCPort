//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ENTITYANIMAL_H
#define MCPORT_ENTITYANIMAL_H

#include "EntityCreature.h"

#include "game/nbt/NBTTagCompound.h"

class EntityAnimal : public EntityCreature {
public:
    explicit EntityAnimal(World &world);

    float getBlockPathWeight(int_t x, int_t y, int_t z) const override;

    bool getCanSpawnHere() const override;

    int_t getTalkInterval() const override;

    void writeEntityToNBT(NBTTagCompound &nbt) override;

    void readEntityFromNBT(NBTTagCompound &nbt) override;
};

#endif //MCPORT_ENTITYANIMAL_H
