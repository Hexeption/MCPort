//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityLiving.h"

#include <cmath>

#include "game/block/Block.h"
#include "game/client/MathHelper.h"
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

std::unique_ptr<Vec3D> EntityLiving::getPosition(const float partialTicks) const {
    if (partialTicks == 1.0f) {
        return Vec3D::createVector(posX, posY, posZ);
    }

    const double x = prevPosX + (posX - prevPosX) * static_cast<double>(partialTicks);
    const double y = prevPosY + (posY - prevPosY) * static_cast<double>(partialTicks);
    const double z = prevPosZ + (posZ - prevPosZ) * static_cast<double>(partialTicks);
    return Vec3D::createVector(x, y, z);
}

std::unique_ptr<Vec3D> EntityLiving::getLook(const float partialTicks) const {
    constexpr float degreesToRadians = static_cast<float>(3.14159265358979323846 / 180.0);
    if (partialTicks == 1.0f) {
        const float yawCos = MathHelper::cos(-rotationYaw * degreesToRadians - static_cast<float>(std::acos(-1.0)));
        const float yawSin = MathHelper::sin(-rotationYaw * degreesToRadians - static_cast<float>(std::acos(-1.0)));
        const float pitchCos = -MathHelper::cos(-rotationPitch * degreesToRadians);
        const float pitchSin = MathHelper::sin(-rotationPitch * degreesToRadians);
        return Vec3D::createVector(static_cast<double>(yawSin * pitchCos), static_cast<double>(pitchSin),
                                   static_cast<double>(yawCos * pitchCos));
    }

    const float pitch = prevRotationPitch + (rotationPitch - prevRotationPitch) * partialTicks;
    const float yaw = prevRotationYaw + (rotationYaw - prevRotationYaw) * partialTicks;
    const float yawCos = MathHelper::cos(-yaw * degreesToRadians - static_cast<float>(std::acos(-1.0)));
    const float yawSin = MathHelper::sin(-yaw * degreesToRadians - static_cast<float>(std::acos(-1.0)));
    const float pitchCos = -MathHelper::cos(-pitch * degreesToRadians);
    const float pitchSin = MathHelper::sin(-pitch * degreesToRadians);
    return Vec3D::createVector(static_cast<double>(yawSin * pitchCos), static_cast<double>(pitchSin),
                               static_cast<double>(yawCos * pitchCos));
}

MovingObjectPosition EntityLiving::rayTrace(const double reachDistance, const float partialTicks) const {
    std::unique_ptr<Vec3D> start = getPosition(partialTicks);
    std::unique_ptr<Vec3D> look = getLook(partialTicks);
    std::unique_ptr<Vec3D> end = start->addVector(look->xCoord * reachDistance, look->yCoord * reachDistance,
                                                  look->zCoord * reachDistance);
    return worldObj.rayTraceBlocks(*start, *end);
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
