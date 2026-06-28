//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ENTITYFALLINGSAND_H
#define MCPORT_ENTITYFALLINGSAND_H

#include "Entity.h"

#include "game/nbt/NBTTagCompound.h"

class EntityFallingSand : public Entity {
public:
    int_t blockID = 0;
    int_t fallTime = 0;

    explicit EntityFallingSand(World &world);

    EntityFallingSand(World &world, float x, float y, float z, int_t blockID);

    bool canBeCollidedWith() const override;

    void onUpdate() override;

    World &getWorld() const;

    void writeEntityToNBT(NBTTagCompound &nbt) override;

    void readEntityFromNBT(NBTTagCompound &nbt) override;

private:
    void dropItem(int_t itemId, int_t count);
};

#endif //MCPORT_ENTITYFALLINGSAND_H
