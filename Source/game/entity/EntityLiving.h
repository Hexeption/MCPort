//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYLIVING_H
#define MCPORT_ENTITYLIVING_H

#include "Entity.h"
#include "game/phys/MovingObjectPosition.h"
#include "java/String.h"
#include "java/Type.h"

#include "game/nbt/NBTTagCompound.h"

class EntityLiving : public Entity {
public:
    int_t heartsHalvesLife = 20;
    float unusedRotationPitch2 = 0.0f;
    float unusedFloat = 0.0f;
    float unusedRotationPitch = 0.0f;
    float renderYawOffset = 0.0f;
    float prevRenderYawOffset = 0.0f;
    bool isAIEnabled = false;
    float prevSwingProgress = 0.0f;
    float swingProgress = 0.0f;
    int_t health = 10;
    int_t prevHealth = 10;
    int_t hurtTime = 0;
    int_t maxHurtTime = 0;
    float attackedAtYaw = 0.0f;
    int_t deathTime = 0;
    int_t attackTime = 0;
    float prevCameraPitch = 0.0f;
    float cameraPitch = 0.0f;
    int_t unusedInt = -1;
    float unusedFloat4 = 0.0f;
    float prevLimbYaw = 0.0f;
    float limbYaw = 0.0f;
    float limbSwing = 0.0f;
    int_t heartsLife = 0;
    float prevDistanceWalkedModified = 0.0f;
    float distanceWalkedModified = 0.0f;

    explicit EntityLiving(World &world);

    void onEntityUpdate() override;

    void onUpdate() override;

    float getEyeHeight() const override;

    bool canBeCollidedWith() const override;

    bool canBePushed() const override;

    bool isEntityAlive() const override;

    bool isEntityInsideOpaqueBlock() const override;

    std::unique_ptr<Vec3D> getPosition(float partialTicks) const;

    std::unique_ptr<Vec3D> getLook(float partialTicks) const;

    MovingObjectPosition rayTrace(double reachDistance, float partialTicks) const;

    virtual void onLivingUpdate();

    void moveEntityWithHeading(float strafe, float forward);

    virtual jstring getTexture() const;

    virtual void updateEntityActionState();

    bool attackEntityFrom(Entity *attacker, int_t amount) override;

    void faceEntity(Entity &target, float maxYawDelta);

    float getSwingProgress(float partialTicks) const;

    virtual int_t getTalkInterval() const;

    virtual jstring getLivingSound() const;

    virtual jstring getHurtSound() const;

    virtual jstring getDeathSound() const;

    virtual float getSoundVolume() const;

    virtual int_t getDropItemId() const;

    virtual bool getCanSpawnHere() const;

    virtual bool isOnLadder() const;

    bool canEntityBeSeen(Entity &target) const;

    void heal(int_t amount);

    void knockBack(Entity *attacker, int_t damage, double dx, double dz);

    void onDeath(Entity *cause);

    virtual void onEntityDeath();

    void fall(float distance) override;

    void writeEntityToNBT(NBTTagCompound &nbt) override;

    void readEntityFromNBT(NBTTagCompound &nbt) override;

    void setPositionAndRotation(double x, double y, double z, float yaw, float pitch, int_t increments);

protected:
    jstring texture = u"/char.png";
    bool dead = false;
    float ridingRotUnused = 0.0f;
    float prevRidingRotUnused = 0.0f;
    float rotationUnused = 0.0f;
    float prevRotationUnused = 0.0f;
    bool unusedBool1 = true;
    bool unusedBool2 = true;
    float unusedRotation = 0.0f;
    jstring entityType;
    float unusedFloat1 = 1.0f;
    int_t scoreValue = 0;
    float unusedFloat2 = 0.0f;
    int_t entityAge = 0;
    float moveStrafing = 0.0f;
    float moveForward = 0.0f;
    float randomYawVelocity = 0.0f;
    bool isJumping = false;
    float defaultPitch = 0.0f;
    float moveSpeed = 0.7f;

    virtual void jump();

private:
    int_t livingSoundTime = 0;
    int_t newPosRotationIncrements = 0;
    double newPosX = 0.0;
    double newPosY = 0.0;
    double newPosZ = 0.0;
    double newRotationYaw = 0.0;
    double newRotationPitch = 0.0;
    float unusedFloat3 = 0.0f;
    Entity *currentTarget = nullptr;
    int_t numTicksToChaseTarget = 0;

    float updateRotation(float current, float target, float maxDelta) const;
};

#endif //MCPORT_ENTITYLIVING_H
