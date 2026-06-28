//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITY_H
#define MCPORT_ENTITY_H

#include <memory>

#include "game/block/Material.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/phys/AxisAlignedBB.h"
#include "game/util/Vec3D.h"
#include "java/Random.h"
#include "java/String.h"
#include "java/Type.h"

class World;
class EntityItem;
class EntityPlayer;

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
    double renderDistanceWeight = 1.0;
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
    Random rand = Random();
    bool addedToChunk = false;
    int_t chunkCoordX = 0;
    int_t chunkCoordY = 0;
    int_t chunkCoordZ = 0;
    bool canTriggerWalking = true;
    bool preventEntitySpawning = false;
    jstring skinUrl;

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

    virtual bool canBeCollidedWith() const;

    virtual bool canBePushed() const;

    double getDistanceSqToEntity(const Entity &other) const;

    float getDistanceToEntity(const Entity &other) const;

    virtual bool interact(EntityPlayer &player);

    virtual void applyEntityCollision(Entity &other);

    virtual bool attackEntityFrom(Entity *attacker, int_t amount);

    bool isInRangeToRenderVec3D(const Vec3D &vec) const;

    bool isInRangeToRenderDist(double distanceSq) const;

    bool isInsideOfMaterial(Material *material) const;

    virtual bool handleWaterMovement();

    virtual bool handleLavaMovement() const;

    bool isOffsetPositionInLiquid(double x, double y, double z) const;

    bool isInWater() const;

    virtual void onCollideWithPlayer(EntityPlayer &player);

    virtual void setEntityDead();

    virtual void addToPlayerScore(Entity *entity, int_t score);

    virtual bool isEntityAlive() const;

    virtual bool isEntityInsideOpaqueBlock() const;

    EntityItem *dropItem(int_t itemId, int_t count);

    EntityItem *entityDropItem(int_t itemId, int_t count, float offsetY);

    std::unique_ptr<Vec3D> getLookVec() const;

    virtual void fall(float distance);

    bool addEntityID(NBTTagCompound &nbt);

    void writeToNBT(NBTTagCompound &nbt);

    void readFromNBT(NBTTagCompound &nbt);

    virtual void writeEntityToNBT(NBTTagCompound &nbt);

    virtual void readEntityFromNBT(NBTTagCompound &nbt);
};

#endif //MCPORT_ENTITY_H
