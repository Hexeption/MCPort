//
// Created by Keir Davis on 26/06/2026.
//

#include "EntityAnimal.h"

#include "game/block/Block.h"
#include "game/client/MathHelper.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/world/World.h"

EntityAnimal::EntityAnimal(World &world) : EntityCreature(world) {
}

float EntityAnimal::getBlockPathWeight(const int_t x, const int_t y, const int_t z) const {
    if (worldObj.getBlockId(x, y - 1, z) == Block::grass->blockID) {
        return 10.0f;
    }
    return worldObj.getBrightness(x, y, z) - 0.5f;
}

bool EntityAnimal::getCanSpawnHere() const {
    const int_t x = MathHelper::floor_double(posX);
    const int_t y = MathHelper::floor_double(boundingBox.minY);
    const int_t z = MathHelper::floor_double(posZ);
    return worldObj.getBlockId(x, y - 1, z) == Block::grass->blockID &&
           worldObj.getBlockLightValue(x, y, z) > 8 &&
           EntityCreature::getCanSpawnHere();
}

int_t EntityAnimal::getTalkInterval() const {
    return 120;
}

void EntityAnimal::writeEntityToNBT(NBTTagCompound &nbt) {
    EntityCreature::writeEntityToNBT(nbt);
}

void EntityAnimal::readEntityFromNBT(NBTTagCompound &nbt) {
    EntityCreature::readEntityFromNBT(nbt);
}
