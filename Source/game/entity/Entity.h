//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITY_H
#define MCPORT_ENTITY_H

#include "game/phys/AxisAlignedBB.h"

class World;

class Entity {
public:
    World &worldObj;
    double prevPosX = 0.0;
    double prevPosY = 0.0;
    double prevPosZ = 0.0;
    double posX = 0.0;
    double posY = 0.0;
    double posZ = 0.0;
    double motionX = 0.0;
    double motionY = 0.0;
    double motionZ = 0.0;
    float rotationYaw = 0.0f;
    float rotationPitch = 0.0f;
    float prevRotationYaw = 0.0f;
    float prevRotationPitch = 0.0f;
    AxisAlignedBB boundingBox;
    bool onGround = false;
    bool isCollidedHorizontally = false;
    bool isCollidedVertically = false;
    bool isCollided = false;
    bool isDead = false;
    float yOffset = 0.0f;
    float width = 0.6f;
    float height = 1.8f;
    float ySize = 0.0f;
    float stepHeight = 0.0f;

    explicit Entity(World &world);

    virtual ~Entity() = default;

    virtual void onUpdate();

    virtual void onEntityUpdate();

    virtual float getEyeHeight() const;

    virtual void preparePlayerToSpawn();

    void setSize(float width, float height);

    void setPosition(double x, double y, double z);

    void setRotation(float yaw, float pitch);

    void setLocationAndAngles(double x, double y, double z, float yaw, float pitch);

    void setAngles(float yawDelta, float pitchDelta);

    void moveEntity(double x, double y, double z);

    void moveFlying(float strafe, float forward, float speed);
};

#endif //MCPORT_ENTITY_H
