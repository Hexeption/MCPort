//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityLiving.h"

#include <cmath>

#include "game/block/Block.h"
#include "game/world/World.h"

EntityLiving::EntityLiving(World &world) : Entity(world) {
    stepHeight = 0.5f;
}

void EntityLiving::onUpdate() {
    Entity::onUpdate();
    onLivingUpdate();
}

float EntityLiving::getEyeHeight() const {
    return height * 0.85f;
}

void EntityLiving::onLivingUpdate() {
    if (isJumping && onGround) {
        jump();
    }

    moveStrafing *= 0.98f;
    moveForward *= 0.98f;
    moveEntityWithHeading(moveStrafing, moveForward);
}

void EntityLiving::moveEntityWithHeading(const float strafe, const float forward) {
    float slipperiness = 0.91f;
    if (onGround) {
        slipperiness = 546.0f * 0.1f * 0.1f * 0.1f;
        const int_t blockId = worldObj.getBlockId(static_cast<int_t>(std::floor(posX)),
                                                  static_cast<int_t>(std::floor(boundingBox.minY)) - 1,
                                                  static_cast<int_t>(std::floor(posZ)));
        if (blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) &&
            Block::blocksList[blockId] != nullptr) {
            slipperiness = Block::blocksList[blockId]->slipperiness * 0.91f;
        }
    }

    const float groundSpeed = 0.16277136f / (slipperiness * slipperiness * slipperiness);
    const float movementSpeed = onGround ? 0.1f * groundSpeed : 0.02f;
    moveFlying(strafe, forward, movementSpeed);

    slipperiness = 0.91f;
    if (onGround) {
        slipperiness = 546.0f * 0.1f * 0.1f * 0.1f;
        const int_t blockId = worldObj.getBlockId(static_cast<int_t>(std::floor(posX)),
                                                  static_cast<int_t>(std::floor(boundingBox.minY)) - 1,
                                                  static_cast<int_t>(std::floor(posZ)));
        if (blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) &&
            Block::blocksList[blockId] != nullptr) {
            slipperiness = Block::blocksList[blockId]->slipperiness * 0.91f;
        }
    }

    moveEntity(motionX, motionY, motionZ);
    motionY -= 0.08;
    motionY *= 0.98;
    motionX *= slipperiness;
    motionZ *= slipperiness;
}

void EntityLiving::jump() {
    motionY = 0.42;
}
