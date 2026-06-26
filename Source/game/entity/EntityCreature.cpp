//
// Created by Keir Davis on 26/06/2026.
//

#include "EntityCreature.h"

#include <cmath>

#include "game/block/Material.h"
#include "game/client/MathHelper.h"
#include "game/util/Vec3D.h"
#include "game/world/PathEntity.h"
#include "game/world/World.h"

EntityCreature::EntityCreature(World &world) : EntityLiving(world) {
}

void EntityCreature::updateEntityActionState() {
    hasAttacked = false;
    constexpr float attackRange = 16.0f;

    if (entityToAttack == nullptr) {
        entityToAttack = findPlayerToAttack();
        if (entityToAttack != nullptr) {
            pathToEntity = worldObj.getPathToEntity(this, entityToAttack, attackRange);
        }
    } else if (!entityToAttack->isEntityAlive()) {
        entityToAttack = nullptr;
    } else {
        const float dist = entityToAttack->getDistanceToEntity(*this);
        if (canEntityBeSeen(*entityToAttack)) {
            attackEntity(*entityToAttack, dist);
        }
    }

    if (hasAttacked || entityToAttack == nullptr ||
        (pathToEntity != nullptr && rand.nextInt(20) != 0)) {
        if ((pathToEntity == nullptr && rand.nextInt(80) == 0) || rand.nextInt(80) == 0) {
            bool found = false;
            int_t bestX = -1, bestY = -1, bestZ = -1;
            float bestWeight = -99999.0f;

            for (int_t i = 0; i < 10; ++i) {
                const int_t tx = MathHelper::floor_double(posX + rand.nextInt(13) - 6.0);
                const int_t ty = MathHelper::floor_double(posY + rand.nextInt(7) - 3.0);
                const int_t tz = MathHelper::floor_double(posZ + rand.nextInt(13) - 6.0);
                const float w = getBlockPathWeight(tx, ty, tz);
                if (w > bestWeight) {
                    bestWeight = w;
                    bestX = tx;
                    bestY = ty;
                    bestZ = tz;
                    found = true;
                }
            }

            if (found) {
                pathToEntity = worldObj.getEntityPathToXYZ(this, bestX, bestY, bestZ, 10.0f);
            }
        }
    } else {
        pathToEntity = worldObj.getPathToEntity(this, entityToAttack, attackRange);
    }

    const int_t floorY = MathHelper::floor_double(boundingBox.minY);
    const bool inWaterNow = handleWaterMovement();
    const bool inLavaNow = handleLavaMovement();
    rotationPitch = 0.0f;

    if (pathToEntity != nullptr && rand.nextInt(100) != 0) {
        auto var25 = pathToEntity->getPosition(this);
        const double var26 = static_cast<double>(width * 2.0f);

        while (var25 != nullptr &&
               var25->squareDistanceTo(posX, var25->yCoord, posZ) < var26 * var26) {
            pathToEntity->incrementPathIndex();
            if (pathToEntity->isFinished()) {
                var25.reset();
                pathToEntity.reset();
            } else {
                var25 = pathToEntity->getPosition(this);
            }
        }

        isJumping = false;
        if (var25 != nullptr) {
            const double var27 = var25->xCoord - posX;
            const double var28 = var25->zCoord - posZ;
            const double var12 = var25->yCoord - static_cast<double>(floorY);
            float var14 = static_cast<float>(std::atan2(var28, var27) * 180.0 / 3.14159265358979) - 90.0f;
            float var15 = var14 - rotationYaw;

            moveForward = moveSpeed;
            while (var15 < -180.0f) var15 += 360.0f;
            while (var15 >= 180.0f) var15 -= 360.0f;

            if (var15 > 30.0f) var15 = 30.0f;
            if (var15 < -30.0f) var15 = -30.0f;

            rotationYaw += var15;

            if (hasAttacked && entityToAttack != nullptr) {
                const double ex = entityToAttack->posX - posX;
                const double ez = entityToAttack->posZ - posZ;
                const float savedYaw = rotationYaw;
                rotationYaw = static_cast<float>(std::atan2(ez, ex) * 180.0 / 3.14159265358979) - 90.0f;
                const float strideAngle = (savedYaw - rotationYaw + 90.0f) * 3.14159265358979f / 180.0f;
                moveStrafing = -MathHelper::sin(strideAngle) * moveForward * 1.0f;
                moveForward = MathHelper::cos(strideAngle) * moveForward * 1.0f;
            }

            if (var12 > 0.0) {
                isJumping = true;
            }
        }

        if (entityToAttack != nullptr) {
            faceEntity(*entityToAttack, 30.0f);
        }

        if (isCollidedHorizontally) {
            isJumping = true;
        }

        if (rand.nextFloat() < 0.8f && (inWaterNow || inLavaNow)) {
            isJumping = true;
        }
    } else {
        EntityLiving::updateEntityActionState();
        pathToEntity.reset();
    }
}

void EntityCreature::attackEntity(Entity &, float) {
}

float EntityCreature::getBlockPathWeight(int_t, int_t, int_t) const {
    return 0.0f;
}

Entity *EntityCreature::findPlayerToAttack() {
    return nullptr;
}

bool EntityCreature::getCanSpawnHere() const {
    const int_t x = MathHelper::floor_double(posX);
    const int_t y = MathHelper::floor_double(boundingBox.minY);
    const int_t z = MathHelper::floor_double(posZ);
    return EntityLiving::getCanSpawnHere() && getBlockPathWeight(x, y, z) >= 0.0f;
}
