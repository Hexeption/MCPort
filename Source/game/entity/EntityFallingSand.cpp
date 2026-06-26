//
// Created by Keir Davis on 26/06/2026.
//

#include "EntityFallingSand.h"

#include "EntityItem.h"
#include "game/client/MathHelper.h"
#include "game/item/ItemStack.h"
#include "game/world/World.h"

EntityFallingSand::EntityFallingSand(World &world) : Entity(world) {
}

EntityFallingSand::EntityFallingSand(World &world, const float x, const float y, const float z, const int_t blockId)
    : Entity(world) {
    blockID = blockId;
    preventEntitySpawning = true;
    setSize(0.98f, 0.98f);
    yOffset = height / 2.0f;
    setPosition(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z));
    motionX = 0.0;
    motionY = 0.0;
    motionZ = 0.0;
    canTriggerWalking = false;
    prevPosX = static_cast<double>(x);
    prevPosY = static_cast<double>(y);
    prevPosZ = static_cast<double>(z);
}

bool EntityFallingSand::canBeCollidedWith() const {
    return !isDead;
}

void EntityFallingSand::onUpdate() {
    if (blockID == 0) {
        setEntityDead();
        return;
    }

    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    ++fallTime;
    motionY -= 0.04;
    moveEntity(motionX, motionY, motionZ);
    motionX *= 0.98;
    motionY *= 0.98;
    motionZ *= 0.98;

    const int_t bx = MathHelper::floor_double(posX);
    const int_t by = MathHelper::floor_double(posY);
    const int_t bz = MathHelper::floor_double(posZ);

    if (worldObj.getBlockId(bx, by, bz) == blockID) {
        worldObj.setBlockWithNotify(bx, by, bz, 0);
    }

    if (onGround) {
        motionX *= 0.7;
        motionZ *= 0.7;
        motionY *= -0.5;
        setEntityDead();
        if (!worldObj.canBlockBePlacedAt(blockID, bx, by, bz, true) ||
            !worldObj.setBlockWithNotify(bx, by, bz, blockID)) {
            dropItem(blockID, 1);
        }
    } else if (fallTime > 100) {
        dropItem(blockID, 1);
        setEntityDead();
    }
}

World &EntityFallingSand::getWorld() const {
    return worldObj;
}

void EntityFallingSand::dropItem(const int_t itemId, const int_t count) {
    for (int_t i = 0; i < count; ++i) {
        auto item = std::make_unique<EntityItem>(worldObj, posX, posY, posZ, ItemStack(itemId));
        item->delayBeforeCanPickup = 10;
        worldObj.spawnEntityInWorld(std::move(item));
    }
}
