//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityLiving.h"

#include <cmath>

#include "game/block/Block.h"
#include "game/block/Material.h"
#include "game/client/MathHelper.h"
#include "game/entity/EntityPlayer.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/phys/MovingObjectPosition.h"
#include "game/util/Vec3D.h"
#include "game/world/World.h"

EntityLiving::EntityLiving(World &world) : Entity(world) {
    stepHeight = 0.5f;
    preventEntitySpawning = true;
    unusedRotationPitch = static_cast<float>((rand.nextFloat() + 1.0) * 0.01);
    setPosition(posX, posY, posZ);
    unusedRotationPitch2 = rand.nextFloat() * 12398.0f;
    rotationYaw = static_cast<float>(rand.nextFloat() * 3.14159265358979f * 2.0f);
    unusedFloat = 1.0f;
    unusedFloat4 = static_cast<float>(rand.nextFloat() * 0.9 + 0.1);
}

void EntityLiving::onEntityUpdate() {
    prevSwingProgress = swingProgress;
    Entity::onEntityUpdate();

    if (rand.nextInt(1000) < livingSoundTime++) {
        livingSoundTime = -getTalkInterval();
        const jstring sound = getLivingSound();
        if (!sound.empty()) {
            worldObj.playSoundAtEntity(this, sound, getSoundVolume(),
                                       (rand.nextFloat() - rand.nextFloat()) * 0.2f + 1.0f);
        }
    }

    if (isEntityAlive() && isEntityInsideOpaqueBlock()) {
        attackEntityFrom(nullptr, 1);
    }

    if (isEntityAlive() && isInsideOfMaterial(Material::water)) {
        --air;
        if (air == -20) {
            air = 0;
            for (int_t i = 0; i < 8; ++i) {
                const float vx = rand.nextFloat() - rand.nextFloat();
                const float vy = rand.nextFloat() - rand.nextFloat();
                const float vz = rand.nextFloat() - rand.nextFloat();
                worldObj.spawnParticle(u"bubble", posX + vx, posY + vy, posZ + vz,
                                       motionX, motionY, motionZ);
            }
            attackEntityFrom(nullptr, 2);
        }
        fire = 0;
    } else {
        air = maxAir;
    }

    prevCameraPitch = cameraPitch;

    if (attackTime > 0) {
        --attackTime;
    }
    if (hurtTime > 0) {
        --hurtTime;
    }
    if (heartsLife > 0) {
        --heartsLife;
    }

    if (health <= 0) {
        ++deathTime;
        if (deathTime > 20) {
            onEntityDeath();
            setEntityDead();
            for (int_t i = 0; i < 20; ++i) {
                const double dx = (rand.nextFloat() - rand.nextFloat()) * 0.02;
                const double dy = (rand.nextFloat() - rand.nextFloat()) * 0.02;
                const double dz = (rand.nextFloat() - rand.nextFloat()) * 0.02;
                worldObj.spawnParticle(u"explode",
                                       posX + rand.nextFloat() * width * 2.0f - width,
                                       posY + rand.nextFloat() * height,
                                       posZ + rand.nextFloat() * width * 2.0f - width,
                                       dx, dy, dz);
            }
        }
    }

    prevRotationUnused = rotationUnused;
    prevRenderYawOffset = renderYawOffset;
    prevRotationYaw = rotationYaw;
    prevRotationPitch = rotationPitch;
}

void EntityLiving::onUpdate() {
    Entity::onUpdate();
    onLivingUpdate();

    const double dx = posX - prevPosX;
    const double dz = posZ - prevPosZ;
    float var5 = MathHelper::sqrt_double(dx * dx + dz * dz);
    float var6 = renderYawOffset;
    float var7 = 0.0f;

    ridingRotUnused = prevRidingRotUnused;
    float var8 = 0.0f;

    if (var5 > 0.05f) {
        var8 = 1.0f;
        var7 = var5 * 3.0f;
        var6 = static_cast<float>(std::atan2(dz, dx) * 180.0 / 3.14159265358979) - 90.0f;
    }

    if (swingProgress > 0.0f) {
        var6 = rotationYaw;
    }

    if (!onGround) {
        var8 = 0.0f;
    }

    prevRidingRotUnused += (var8 - prevRidingRotUnused) * 0.3f;

    float var9 = var6 - renderYawOffset;
    while (var9 < -180.0f) var9 += 360.0f;
    while (var9 >= 180.0f) var9 -= 360.0f;
    renderYawOffset += var9 * 0.3f;

    float var10 = rotationYaw - renderYawOffset;
    while (var10 < -180.0f) var10 += 360.0f;
    while (var10 >= 180.0f) var10 -= 360.0f;

    const bool var11 = var10 < -90.0f || var10 >= 90.0f;
    if (var10 < -75.0f) var10 = -75.0f;
    if (var10 >= 75.0f) var10 = 75.0f;

    renderYawOffset = rotationYaw - var10;
    if (var10 * var10 > 2500.0f) {
        renderYawOffset += var10 * 0.2f;
    }

    if (var11) {
        var7 *= -1.0f;
    }

    while (rotationYaw - prevRotationYaw < -180.0f) prevRotationYaw -= 360.0f;
    while (rotationYaw - prevRotationYaw >= 180.0f) prevRotationYaw += 360.0f;
    while (renderYawOffset - prevRenderYawOffset < -180.0f) prevRenderYawOffset -= 360.0f;
    while (renderYawOffset - prevRenderYawOffset >= 180.0f) prevRenderYawOffset += 360.0f;
    while (rotationPitch - prevRotationPitch < -180.0f) prevRotationPitch -= 360.0f;
    while (rotationPitch - prevRotationPitch >= 180.0f) prevRotationPitch += 360.0f;

    rotationUnused += var7;
}

float EntityLiving::getEyeHeight() const {
    return height * 0.85f;
}

std::unique_ptr<Vec3D> EntityLiving::getPosition(const float partialTicks) const {
    if (partialTicks == 1.0f) {
        return Vec3D::createVector(posX, posY, posZ);
    }
    const double x = lastTickPosX + (posX - lastTickPosX) * static_cast<double>(partialTicks);
    const double y = lastTickPosY + (posY - lastTickPosY) * static_cast<double>(partialTicks);
    const double z = lastTickPosZ + (posZ - lastTickPosZ) * static_cast<double>(partialTicks);
    return Vec3D::createVector(x, y, z);
}

std::unique_ptr<Vec3D> EntityLiving::getLook(const float partialTicks) const {
    constexpr float pi = static_cast<float>(3.14159265358979323846);
    constexpr float toRad = pi / 180.0f;
    if (partialTicks == 1.0f) {
        const float yawCos = MathHelper::cos(-rotationYaw * toRad - pi);
        const float yawSin = MathHelper::sin(-rotationYaw * toRad - pi);
        const float pitchCos = -MathHelper::cos(-rotationPitch * toRad);
        const float pitchSin = MathHelper::sin(-rotationPitch * toRad);
        return Vec3D::createVector(yawSin * pitchCos, pitchSin, yawCos * pitchCos);
    }
    const float pitch = prevRotationPitch + (rotationPitch - prevRotationPitch) * partialTicks;
    const float yaw = prevRotationYaw + (rotationYaw - prevRotationYaw) * partialTicks;
    const float yawCos = MathHelper::cos(-yaw * toRad - pi);
    const float yawSin = MathHelper::sin(-yaw * toRad - pi);
    const float pitchCos = -MathHelper::cos(-pitch * toRad);
    const float pitchSin = MathHelper::sin(-pitch * toRad);
    return Vec3D::createVector(yawSin * pitchCos, pitchSin, yawCos * pitchCos);
}

MovingObjectPosition EntityLiving::rayTrace(const double reachDistance, const float partialTicks) const {
    std::unique_ptr<Vec3D> start = getPosition(partialTicks);
    std::unique_ptr<Vec3D> look = getLook(partialTicks);
    std::unique_ptr<Vec3D> end = start->addVector(look->xCoord * reachDistance,
                                                   look->yCoord * reachDistance,
                                                   look->zCoord * reachDistance);
    return worldObj.rayTraceBlocks(*start, *end);
}

bool EntityLiving::canBeCollidedWith() const {
    return !isDead;
}

bool EntityLiving::canBePushed() const {
    return !isDead;
}

bool EntityLiving::isEntityAlive() const {
    return !isDead && health > 0;
}

bool EntityLiving::isEntityInsideOpaqueBlock() const {
    return Entity::isEntityInsideOpaqueBlock();
}

void EntityLiving::onLivingUpdate() {
    if (newPosRotationIncrements > 0) {
        const double x = posX + (newPosX - posX) / static_cast<double>(newPosRotationIncrements);
        const double y = posY + (newPosY - posY) / static_cast<double>(newPosRotationIncrements);
        const double z = posZ + (newPosZ - posZ) / static_cast<double>(newPosRotationIncrements);
        double dyaw = newRotationYaw - static_cast<double>(rotationYaw);
        while (dyaw < -180.0) dyaw += 360.0;
        while (dyaw >= 180.0) dyaw -= 360.0;
        rotationYaw = static_cast<float>(static_cast<double>(rotationYaw) + dyaw / static_cast<double>(newPosRotationIncrements));
        rotationPitch = static_cast<float>(static_cast<double>(rotationPitch) + (newRotationPitch - static_cast<double>(rotationPitch)) / static_cast<double>(newPosRotationIncrements));
        --newPosRotationIncrements;
        setPosition(x, y, z);
        setRotation(rotationYaw, rotationPitch);
    }

    if (health <= 0) {
        isJumping = false;
        moveStrafing = 0.0f;
        moveForward = 0.0f;
        randomYawVelocity = 0.0f;
    } else if (!isAIEnabled) {
        updateEntityActionState();
    }

    const bool inWaterNow = handleWaterMovement();
    const bool inLava = handleLavaMovement();
    if (isJumping) {
        if (inWaterNow) {
            motionY += 0.04;
        } else if (inLava) {
            motionY += 0.04;
        } else if (onGround) {
            jump();
        }
    }

    moveStrafing *= 0.98f;
    moveForward *= 0.98f;
    randomYawVelocity *= 0.9f;
    moveEntityWithHeading(moveStrafing, moveForward);

    const auto nearby = worldObj.getEntitiesWithinAABBExcludingEntity(*this, boundingBox.expand(0.2, 0.0, 0.2));
    if (!nearby.empty()) {
        for (Entity *other : nearby) {
            if (other != nullptr && other->canBePushed()) {
                other->applyEntityCollision(*this);
            }
        }
    }
}

void EntityLiving::moveEntityWithHeading(const float strafe, const float forward) {
    if (handleWaterMovement()) {
        const double posYBefore = posY;
        moveFlying(strafe, forward, 0.02f);
        moveEntity(motionX, motionY, motionZ);
        motionX *= 0.8;
        motionY *= 0.8;
        motionZ *= 0.8;
        motionY -= 0.02;
        if (isCollidedHorizontally && isOffsetPositionInLiquid(motionX, motionY + 0.6 - posY + posYBefore, motionZ)) {
            motionY = 0.3;
        }
        return;
    }

    if (handleLavaMovement()) {
        const double posYBefore = posY;
        moveFlying(strafe, forward, 0.02f);
        moveEntity(motionX, motionY, motionZ);
        motionX *= 0.5;
        motionY *= 0.5;
        motionZ *= 0.5;
        motionY -= 0.02;
        if (isCollidedHorizontally && isOffsetPositionInLiquid(motionX, motionY + 0.6 - posY + posYBefore, motionZ)) {
            motionY = 0.3;
        }
        return;
    }

    float slip = 0.91f;
    if (onGround) {
        slip = 546.0f * 0.1f * 0.1f * 0.1f;
        const int_t blockId = worldObj.getBlockId(MathHelper::floor_double(posX),
                                                  MathHelper::floor_double(boundingBox.minY) - 1,
                                                  MathHelper::floor_double(posZ));
        if (blockId > 0 && Block::blocksList[blockId] != nullptr) {
            slip = Block::blocksList[blockId]->slipperiness * 0.91f;
        }
    }

    const float groundSpeed = 0.16277136f / (slip * slip * slip);
    moveFlying(strafe, forward, onGround ? 0.1f * groundSpeed : 0.02f);

    slip = 0.91f;
    if (onGround) {
        slip = 546.0f * 0.1f * 0.1f * 0.1f;
        const int_t blockId = worldObj.getBlockId(MathHelper::floor_double(posX),
                                                  MathHelper::floor_double(boundingBox.minY) - 1,
                                                  MathHelper::floor_double(posZ));
        if (blockId > 0 && Block::blocksList[blockId] != nullptr) {
            slip = Block::blocksList[blockId]->slipperiness * 0.91f;
        }
    }

    if (isOnLadder()) {
        fallDistance = 0.0f;
        if (motionY < -0.15) {
            motionY = -0.15;
        }
    }

    moveEntity(motionX, motionY, motionZ);

    if (isCollidedHorizontally && isOnLadder()) {
        motionY = 0.2;
    }

    motionY -= 0.08;
    motionY *= 0.98;
    motionX *= slip;
    motionZ *= slip;

    prevLimbYaw = limbYaw;
    const double lx = posX - prevPosX;
    const double lz = posZ - prevPosZ;
    float limbSpeed = MathHelper::sqrt_double(lx * lx + lz * lz) * 4.0f;
    if (limbSpeed > 1.0f) limbSpeed = 1.0f;
    limbYaw += (limbSpeed - limbYaw) * 0.4f;
    limbSwing += limbYaw;
}

void EntityLiving::jump() {
    motionY = 0.42;
}

jstring EntityLiving::getTexture() const {
    return texture;
}

void EntityLiving::updateEntityActionState() {
    ++entityAge;
    EntityPlayer *player = worldObj.getClosestPlayerToEntity(this, -1.0);
    if (player != nullptr) {
        const double dx = player->posX - posX;
        const double dy = player->posY - posY;
        const double dz = player->posZ - posZ;
        const double distSq = dx * dx + dy * dy + dz * dz;
        if (distSq > 16384.0) {
            setEntityDead();
        }
        if (entityAge > 600 && rand.nextInt(800) == 0) {
            if (distSq < 1024.0) {
                entityAge = 0;
            } else {
                setEntityDead();
            }
        }
    }

    moveStrafing = 0.0f;
    moveForward = 0.0f;
    constexpr float lookRadius = 8.0f;
    if (rand.nextFloat() < 0.02f) {
        EntityPlayer *nearest = worldObj.getClosestPlayerToEntity(this, static_cast<double>(lookRadius));
        if (nearest != nullptr) {
            currentTarget = nearest;
            numTicksToChaseTarget = 10 + rand.nextInt(20);
        } else {
            randomYawVelocity = (rand.nextFloat() - 0.5f) * 20.0f;
        }
    }

    if (currentTarget != nullptr) {
        faceEntity(*currentTarget, 10.0f);
        if (--numTicksToChaseTarget <= 0 || currentTarget->isDead ||
            currentTarget->getDistanceSqToEntity(*this) > static_cast<double>(lookRadius * lookRadius)) {
            currentTarget = nullptr;
        }
    } else {
        if (rand.nextFloat() < 0.05f) {
            randomYawVelocity = (rand.nextFloat() - 0.5f) * 20.0f;
        }
        rotationYaw += randomYawVelocity;
        rotationPitch = defaultPitch;
    }

    const bool inWaterNow = handleWaterMovement();
    const bool inLavaNow = handleLavaMovement();
    if (inWaterNow || inLavaNow) {
        isJumping = rand.nextFloat() < 0.8f;
    }
}

bool EntityLiving::attackEntityFrom(Entity *attacker, const int_t amount) {
    entityAge = 0;
    if (health <= 0) {
        return false;
    }

    limbYaw = 1.5f;

    if (static_cast<float>(heartsLife) > static_cast<float>(heartsHalvesLife) / 2.0f) {
        if (prevHealth - amount >= health) {
            return false;
        }
        health = prevHealth - amount;
    } else {
        prevHealth = health;
        heartsLife = heartsHalvesLife;
        health -= amount;
        hurtTime = maxHurtTime = 10;
    }

    attackedAtYaw = 0.0f;
    if (attacker != nullptr) {
        double dx = attacker->posX - posX;
        double dz = attacker->posZ - posZ;
        while (dx * dx + dz * dz < 1.0e-4) {
            dx = (rand.nextFloat() - rand.nextFloat()) * 0.01;
            dz = (rand.nextFloat() - rand.nextFloat()) * 0.01;
        }
        attackedAtYaw = static_cast<float>(std::atan2(dz, dx) * 180.0 / 3.14159265358979) - rotationYaw;
        knockBack(attacker, amount, dx, dz);
    } else {
        attackedAtYaw = static_cast<float>(static_cast<int_t>(rand.nextFloat() * 2.0f) * 180);
    }

    if (health <= 0) {
        worldObj.playSoundAtEntity(this, getDeathSound(), getSoundVolume(),
                                   (rand.nextFloat() - rand.nextFloat()) * 0.2f + 1.0f);
        onDeath(attacker);
    } else {
        worldObj.playSoundAtEntity(this, getHurtSound(), getSoundVolume(),
                                   (rand.nextFloat() - rand.nextFloat()) * 0.2f + 1.0f);
    }

    return true;
}

void EntityLiving::faceEntity(Entity &target, float maxYawDelta) {
    const double dx = target.posX - posX;
    const double dz = target.posZ - posZ;
    double dy;
    if (auto *living = dynamic_cast<EntityLiving *>(&target)) {
        dy = living->posY + static_cast<double>(living->getEyeHeight()) -
             (posY + static_cast<double>(getEyeHeight()));
    } else {
        dy = (target.boundingBox.minY + target.boundingBox.maxY) * 0.5 -
             (posY + static_cast<double>(getEyeHeight()));
    }
    const double horizDist = MathHelper::sqrt_double(dx * dx + dz * dz);
    const float yawTarget = static_cast<float>(std::atan2(dz, dx) * 180.0 / 3.14159265358979) - 90.0f;
    const float pitchTarget = static_cast<float>(-(std::atan2(dy, horizDist) * 180.0 / 3.14159265358979));
    rotationPitch = updateRotation(rotationPitch, pitchTarget, maxYawDelta);
    rotationYaw = updateRotation(rotationYaw, yawTarget, maxYawDelta);
}

float EntityLiving::updateRotation(const float current, const float target, const float maxDelta) const {
    float delta = target - current;
    while (delta < -180.0f) delta += 360.0f;
    while (delta >= 180.0f) delta -= 360.0f;
    if (delta > maxDelta) delta = maxDelta;
    if (delta < -maxDelta) delta = -maxDelta;
    return current + delta;
}

float EntityLiving::getSwingProgress(const float partialTicks) const {
    float delta = swingProgress - prevSwingProgress;
    if (delta < 0.0f) ++delta;
    return prevSwingProgress + delta * partialTicks;
}

bool EntityLiving::isOnLadder() const {
    if (Block::ladder == nullptr) return false;
    const int_t x = MathHelper::floor_double(posX);
    const int_t y = MathHelper::floor_double(boundingBox.minY);
    const int_t z = MathHelper::floor_double(posZ);
    return worldObj.getBlockId(x, y, z) == Block::ladder->blockID ||
           worldObj.getBlockId(x, y + 1, z) == Block::ladder->blockID;
}

bool EntityLiving::canEntityBeSeen(Entity &target) const {
    const Vec3D from(posX, posY + getEyeHeight(), posZ);
    const Vec3D to(target.posX, target.posY + target.getEyeHeight(), target.posZ);
    return worldObj.rayTraceBlocks(from, to).hitVec == nullptr;
}

void EntityLiving::heal(const int_t amount) {
    if (health > 0) {
        health += amount;
        if (health > 20) health = 20;
        heartsLife = heartsHalvesLife / 2;
    }
}

void EntityLiving::knockBack(Entity *, int_t, const double dx, const double dz) {
    const float dist = MathHelper::sqrt_double(dx * dx + dz * dz);
    constexpr float strength = 0.4f;
    motionX /= 2.0;
    motionY /= 2.0;
    motionZ /= 2.0;
    motionX -= dx / static_cast<double>(dist) * strength;
    motionY += strength;
    motionZ -= dz / static_cast<double>(dist) * strength;
    if (motionY > strength) {
        motionY = strength;
    }
}

void EntityLiving::onDeath(Entity *cause) {
    if (scoreValue > 0 && cause != nullptr) {
        cause->addToPlayerScore(this, scoreValue);
    }
    dead = true;
    const int_t dropId = getDropItemId();
    if (dropId > 0) {
        const int_t count = rand.nextInt(3);
        for (int_t i = 0; i < count; ++i) {
            dropItem(dropId, 1);
        }
    }
}

void EntityLiving::onEntityDeath() {
}

void EntityLiving::fall(const float distance) {
    const int_t damage = static_cast<int_t>(std::ceil(static_cast<double>(distance - 3.0f)));
    if (damage > 0) {
        attackEntityFrom(nullptr, damage);
    }
}

void EntityLiving::writeEntityToNBT(NBTTagCompound &nbt) {
    nbt.setShort(u"Health", static_cast<short>(health));
    nbt.setShort(u"HurtTime", static_cast<short>(hurtTime));
    nbt.setShort(u"DeathTime", static_cast<short>(deathTime));
    nbt.setShort(u"AttackTime", static_cast<short>(attackTime));
}

void EntityLiving::readEntityFromNBT(NBTTagCompound &nbt) {
    health = nbt.getShort(u"Health");
    if (!nbt.hasKey(u"Health")) {
        health = 10;
    }
    hurtTime = nbt.getShort(u"HurtTime");
    deathTime = nbt.getShort(u"DeathTime");
    attackTime = nbt.getShort(u"AttackTime");
}

void EntityLiving::setPositionAndRotation(const double x, const double y, const double z,
                                           const float yaw, const float pitch,
                                           const int_t increments) {
    yOffset = 0.0f;
    newPosX = x;
    newPosY = y;
    newPosZ = z;
    newRotationYaw = yaw;
    newRotationPitch = pitch;
    newPosRotationIncrements = increments;
}

int_t EntityLiving::getTalkInterval() const { return 80; }
jstring EntityLiving::getLivingSound() const { return u""; }
jstring EntityLiving::getHurtSound() const { return u"random.hurt"; }
jstring EntityLiving::getDeathSound() const { return u"random.hurt"; }
float EntityLiving::getSoundVolume() const { return 1.0f; }
int_t EntityLiving::getDropItemId() const { return 0; }

bool EntityLiving::getCanSpawnHere() const {
    return worldObj.checkIfAABBIsClear(boundingBox) &&
           worldObj.getCollidingBoundingBoxes(*this, boundingBox).empty() &&
           !worldObj.getIsAnyLiquid(boundingBox);
}
