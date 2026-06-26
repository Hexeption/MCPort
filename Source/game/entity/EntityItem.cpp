//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityItem.h"

#include <cmath>

#include "game/block/Block.h"
#include "game/block/Material.h"
#include "game/client/MathHelper.h"
#include "game/entity/EntityPlayer.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/world/World.h"
#include "java/Math.h"

EntityItem::EntityItem(World &world, double x, double y, double z, const ItemStack &stack)
    : Entity(world), item(stack) {
    setSize(0.25f, 0.25f);
    yOffset = height / 2.0f;
    setPosition(x, y, z);

    hoverStart = static_cast<float>(Math::random() * M_PI * 2.0);
    rotationYaw = static_cast<float>(Math::random() * 360.0);

    motionX = static_cast<double>(static_cast<float>(Math::random() * 0.2 - 0.1));
    motionY = 0.2;
    motionZ = static_cast<double>(static_cast<float>(Math::random() * 0.2 - 0.1));

    canTriggerWalking = false;
}

EntityItem::EntityItem(World &world)
    : Entity(world), item(0) {
    setSize(0.25f, 0.25f);
    yOffset = height / 2.0f;
    hoverStart = static_cast<float>(Math::random() * M_PI * 2.0);
}

void EntityItem::onUpdate() {
    Entity::onUpdate();

    if (delayBeforeCanPickup > 0) {
        --delayBeforeCanPickup;
    }

    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;

    motionY -= 0.04;

    if (worldObj.getBlockMaterial(
            MathHelper::floor_double(posX),
            MathHelper::floor_double(posY),
            MathHelper::floor_double(posZ)) == Material::lava) {
        motionY = 0.2;
        motionX = static_cast<double>((rand.nextFloat() - rand.nextFloat()) * 0.2f);
        motionZ = static_cast<double>((rand.nextFloat() - rand.nextFloat()) * 0.2f);
    }

    pushOutOfBlocks(posX, posY, posZ);
    handleWaterMovement();
    moveEntity(motionX, motionY, motionZ);

    float friction = 0.98f;
    if (onGround) {
        friction = 0.1f * 0.1f * 58.8f;

        const int_t blockId = worldObj.getBlockId(
            MathHelper::floor_double(posX),
            MathHelper::floor_double(boundingBox.minY) - 1,
            MathHelper::floor_double(posZ));

        if (blockId > 0 && Block::blocksList[blockId] != nullptr) {
            friction = Block::blocksList[blockId]->slipperiness * 0.98f;
        }
    }

    motionX *= static_cast<double>(friction);
    motionY *= 0.98;
    motionZ *= static_cast<double>(friction);

    if (onGround) {
        motionY *= -0.5;
    }

    ++age2;
    ++age;

    if (age >= 6000) {
        setEntityDead();
    }
}

bool EntityItem::handleWaterMovement() {
    return worldObj.handleMaterialAcceleration(boundingBox, Material::water, *this);
}

bool EntityItem::pushOutOfBlocks(double x, double y, double z) {
    const int_t bx = MathHelper::floor_double(x);
    const int_t by = MathHelper::floor_double(y);
    const int_t bz = MathHelper::floor_double(z);

    const double localX = x - static_cast<double>(bx);
    const double localY = y - static_cast<double>(by);
    const double localZ = z - static_cast<double>(bz);

    if (Block::opaqueCubeLookup[worldObj.getBlockId(bx, by, bz)]) {
        const bool freeNegX = !Block::opaqueCubeLookup[worldObj.getBlockId(bx - 1, by, bz)];
        const bool freePosX = !Block::opaqueCubeLookup[worldObj.getBlockId(bx + 1, by, bz)];
        const bool freeNegY = !Block::opaqueCubeLookup[worldObj.getBlockId(bx, by - 1, bz)];
        const bool freePosY = !Block::opaqueCubeLookup[worldObj.getBlockId(bx, by + 1, bz)];
        const bool freeNegZ = !Block::opaqueCubeLookup[worldObj.getBlockId(bx, by, bz - 1)];
        const bool freePosZ = !Block::opaqueCubeLookup[worldObj.getBlockId(bx, by, bz + 1)];

        byte_t direction = -1;
        double bestDistance = 9999.0;

        if (freeNegX && localX < bestDistance) {
            bestDistance = localX;
            direction = 0;
        }
        if (freePosX && 1.0 - localX < bestDistance) {
            bestDistance = 1.0 - localX;
            direction = 1;
        }
        if (freeNegY && localY < bestDistance) {
            bestDistance = localY;
            direction = 2;
        }
        if (freePosY && 1.0 - localY < bestDistance) {
            bestDistance = 1.0 - localY;
            direction = 3;
        }
        if (freeNegZ && localZ < bestDistance) {
            bestDistance = localZ;
            direction = 4;
        }
        if (freePosZ && 1.0 - localZ < bestDistance) {
            bestDistance = 1.0 - localZ;
            direction = 5;
        }

        const float speed = rand.nextFloat() * 0.2f + 0.1f;

        if (direction == 0) motionX = -speed;
        if (direction == 1) motionX = speed;
        if (direction == 2) motionY = -speed;
        if (direction == 3) motionY = speed;
        if (direction == 4) motionZ = -speed;
        if (direction == 5) motionZ = speed;
    }

    return false;
}

void EntityItem::dealFireDamage(int_t amount) {
    attackEntityFrom(nullptr, amount);
}

bool EntityItem::attackEntityFrom(Entity *, int_t amount) {
    health -= amount;
    if (health <= 0) {
        setEntityDead();
    }

    return false;
}

void EntityItem::writeEntityToNBT(NBTTagCompound &nbt) {
    nbt.setShort(u"Health", static_cast<short_t>(static_cast<byte_t>(health)));
    nbt.setShort(u"Age", static_cast<short_t>(age));

    auto *itemTag = new NBTTagCompound();
    item.writeToNBT(*itemTag);
    nbt.setCompoundTag(u"Item", itemTag);
}

void EntityItem::readEntityFromNBT(NBTTagCompound &nbt) {
    health = nbt.getShort(u"Health") & 255;
    age = nbt.getShort(u"Age");

    NBTTagCompound *itemTag = nbt.getCompoundTag(u"Item");
    item = ItemStack(*itemTag);
}

void EntityItem::onCollideWithPlayer(EntityPlayer &player) {
    if (worldObj.multiplayerWorld) {
        return;
    }

    if (delayBeforeCanPickup > 0) {
        return;
    }

    const int_t pickedUp = item.stackSize;
    if (player.inventory.addItemStackToInventory(item)) {
        player.onItemPickup(*this, pickedUp);
        setEntityDead();
    }
}
