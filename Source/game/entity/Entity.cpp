//
// Created by Keir Davis on 25/06/2026.
//

#include "Entity.h"

#include <cmath>

#include "game/client/MathHelper.h"
#include "game/world/World.h"

Entity::Entity(World &world) : worldObj(world) {
    setPosition(0.0, 0.0, 0.0);
}

void Entity::onUpdate() {
    onEntityUpdate();
}

void Entity::onEntityUpdate() {
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    prevRotationYaw = rotationYaw;
    prevRotationPitch = rotationPitch;
}

float Entity::getEyeHeight() const {
    return 0.0f;
}

void Entity::preparePlayerToSpawn() {
    while (posY > 0.0) {
        setPosition(posX, posY, posZ);
        if (worldObj.getCollidingBoundingBoxes(*this, boundingBox).empty()) {
            break;
        }
        ++posY;
    }

    motionX = 0.0;
    motionY = 0.0;
    motionZ = 0.0;
    rotationPitch = 0.0f;
}

void Entity::setSize(const float newWidth, const float newHeight) {
    width = newWidth;
    height = newHeight;
}

void Entity::setPosition(const double x, const double y, const double z) {
    posX = x;
    posY = y;
    posZ = z;
    const float halfWidth = width / 2.0f;
    boundingBox.setBounds(x - static_cast<double>(halfWidth), y - static_cast<double>(yOffset) + ySize,
                          z - static_cast<double>(halfWidth), x + static_cast<double>(halfWidth),
                          y - static_cast<double>(yOffset) + ySize + height, z + static_cast<double>(halfWidth));
}

void Entity::setRotation(const float yaw, const float pitch) {
    rotationYaw = yaw;
    rotationPitch = pitch;
}

void Entity::setLocationAndAngles(const double x, const double y, const double z, const float yaw, const float pitch) {
    prevPosX = posX = x;
    prevPosY = posY = y;
    prevPosZ = posZ = z;
    rotationYaw = yaw;
    rotationPitch = pitch;
    setPosition(posX, posY, posZ);
}

void Entity::setAngles(const float yawDelta, const float pitchDelta) {
    const float oldPitch = rotationPitch;
    const float oldYaw = rotationYaw;
    rotationYaw += yawDelta * 0.15f;
    rotationPitch -= pitchDelta * 0.15f;
    if (rotationPitch < -90.0f) {
        rotationPitch = -90.0f;
    }
    if (rotationPitch > 90.0f) {
        rotationPitch = 90.0f;
    }
    prevRotationPitch += rotationPitch - oldPitch;
    prevRotationYaw += rotationYaw - oldYaw;
}

void Entity::moveEntity(double x, double y, double z) {
    if (noclip) {
        boundingBox.offset(x, y, z);
        posX = (boundingBox.minX + boundingBox.maxX) / 2.0;
        posY = boundingBox.minY + yOffset - (double) ySize;
        posZ = (boundingBox.minZ + boundingBox.maxZ) / 2.0;
    } else {
        const double originalX = x;
        const double originalY = y;
        const double originalZ = z;
        auto collidingBoxes = worldObj.getCollidingBoundingBoxes(*this, boundingBox.addCoord(x, y, z));

        for (const AxisAlignedBB &box: collidingBoxes) {
            y = box.calculateYOffset(boundingBox, y);
        }
        boundingBox.offset(0.0, y, 0.0);

        for (const AxisAlignedBB &box: collidingBoxes) {
            x = box.calculateXOffset(boundingBox, x);
        }
        boundingBox.offset(x, 0.0, 0.0);

        for (const AxisAlignedBB &box: collidingBoxes) {
            z = box.calculateZOffset(boundingBox, z);
        }
        boundingBox.offset(0.0, 0.0, z);

        posX = (boundingBox.minX + boundingBox.maxX) / 2.0;
        posY = boundingBox.minY + static_cast<double>(yOffset) - ySize;
        posZ = (boundingBox.minZ + boundingBox.maxZ) / 2.0;
        isCollidedHorizontally = originalX != x || originalZ != z;
        isCollidedVertically = originalY != y;
        onGround = originalY != y && originalY < 0.0;
        isCollided = isCollidedHorizontally || isCollidedVertically;

        if (originalX != x) {
            motionX = 0.0;
        }
        if (originalY != y) {
            motionY = 0.0;
        }
        if (originalZ != z) {
            motionZ = 0.0;
        }
    }
}

void Entity::moveFlying(float strafe, float forward, const float speed) {
    float length = std::sqrt(strafe * strafe + forward * forward);
    if (length < 0.01f) {
        return;
    }
    if (length < 1.0f) {
        length = 1.0f;
    }
    length = speed / length;
    strafe *= length;
    forward *= length;

    constexpr float degreesToRadians = 3.14159265358979323846f / 180.0f;
    const float sinYaw = std::sin(rotationYaw * degreesToRadians);
    const float cosYaw = std::cos(rotationYaw * degreesToRadians);
    motionX += static_cast<double>(strafe * cosYaw - forward * sinYaw);
    motionZ += static_cast<double>(forward * cosYaw + strafe * sinYaw);
}

float Entity::getBrightness(float partialTicks) {
    int_t x = MathHelper::floor_double(posX);
    const double box = (boundingBox.maxY - boundingBox.minY) * 0.66;
    int_t y = MathHelper::floor_double(posY - yOffset + box);
    int_t z = MathHelper::floor_double(posZ);
    return worldObj.getBrightness(x, y, z);
}
