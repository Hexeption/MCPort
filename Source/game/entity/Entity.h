//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITY_H
#define MCPORT_ENTITY_H

#include "game/phys/AxisAlignedBB.h"
#include "java/Type.h"

class World;
class Material;

class Entity {
public:
    World &worldObj;
    Entity *riddenByEntity = nullptr;
    Entity *ridingEntity = nullptr;
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
    bool inWater = false;
    bool firstUpdate = true;
    float yOffset = 0.0f;
    float width = 0.6f;
    float height = 1.8f;
    float fallDistance = 0.0f;
    float ySize = 0.0f;
    float stepHeight = 0.0f;
    bool noclip = false;
    int_t ticksExisted = 0;
    int_t fire = 0;
    int_t maxAir = 300;
    int_t air = 300;
    double lastTickPosX;
    double lastTickPosY;
    double lastTickPosZ;

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

    float getBrightness(float partialTicks);

    bool isInsideOfMaterial(Material *material) const;

    bool handleWaterMovement();

    bool handleLavaMovement() const;

    bool isOffsetPositionInLiquid(double x, double y, double z) const;

    bool isInWater() const;
};

#endif //MCPORT_ENTITY_H
