//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_BLOCK_H
#define MCPORT_BLOCK_H

#include <array>

#include "Material.h"
#include "game/phys/AxisAlignedBB.h"
#include "game/phys/MovingObjectPosition.h"
#include "java/Type.h"
#include "java/Random.h"

class Random;
class Entity;
class EntityPlayer;
class World;
class IBlockAccess;
class Vec3D;

class Block {
public:
    static std::array<Block *, 256> blocksList;
    static std::array<int_t, 256> lightOpacity;
    static std::array<int_t, 256> lightValue;
    static std::array<bool, 256> tickOnLoad;
    static std::array<bool, 256> opaqueCubeLookup;
    static Block *stone;
    static Block *grass;
    static Block *dirt;
    static Block *cobblestone;
    static Block *planks;
    static Block *sand;
    static Block *bedrock;
    static Block *gravel;
    static Block *obsidian;
    static Block *waterMoving;
    static Block *waterStill;
    static Block *lavaMoving;
    static Block *lavaStill;
    static Block *leaves;
    static Block *wood;
    static Block *sapling;
    static Block *ladder;
    static Block *chest;
    static Block *workbench;
    static Block *stoneOvenIdle;
    static Block *stairDouble;
    static Block *stairSingle;
    static Block *cobblestoneMossy;
    static Block *oreIron;
    static Block *blockSteel;
    static Block *oreCoal;
    static Block *blockGold;
    static Block *oreGold;
    static Block *oreDiamond;
    static Block *blockDiamond;
    static Block *ice;
    static Block *bookshelf;
    static Block *snow;
    static Block *blockSnow;
    static Block *blockClay;
    static Block *oreRedstone;
    static Block *oreRedstoneGlowing;

    int_t blockIndexInTexture;
    int_t blockID;
    Material *material;
    double minX = 0.0;
    double minY = 0.0;
    double minZ = 0.0;
    double maxX = 1.0;
    double maxY = 1.0;
    double maxZ = 1.0;
    float slipperiness = 0.6f;
    float hardness = 0.0f;

    float blockParticleGravity = 1.0f;

    Block(int_t blockID, Material *material);

    Block(int_t blockID, int_t blockIndexInTexture, Material *material);

    Block(int_t blockID, int_t topTexture, int_t bottomTexture, int_t sideTexture, Material *material);

    Block *setLightOpacity(int_t opacity);

    Block *setLightValue(int_t value);

    Block *setHardness(float hardness);

    virtual int_t getRenderType() const;

    void setBlockBoundsForItemRender();

    virtual int_t getBlockTexture(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const;

    virtual int_t getBlockTextureFromSide(int_t side) const;

    virtual int_t getBlockTextureFromSideAndMetadata(int_t side, int_t metadata) const;

    virtual bool isOpaqueCube() const;

    virtual float getBlockBrightness(IBlockAccess &blockAccess, int_t x, int_t y, int_t z) const;

    virtual bool shouldSideBeRendered(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const;

    void setBlockBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

    virtual AxisAlignedBB getSelectedBoundingBoxFromPool(const World &world, int_t x, int_t y, int_t z) const;

    virtual AxisAlignedBB getCollisionBoundingBoxFromPool(const World &world, int_t x, int_t y, int_t z) const;

    virtual void setBlockBoundsBasedOnState(IBlockAccess &blockAccess, int_t x, int_t y, int_t z);

    virtual void updateTick(World &world, int_t x, int_t y, int_t z, Random &random);

    virtual void randomDisplayTick(World &world, int_t x, int_t y, int_t z, Random &random);

    virtual bool canPlaceBlockAt(World &world, int_t x, int_t y, int_t z) const;

    virtual int_t tickRate() const;

    void setTickOnLoad(bool value);

    virtual int_t getRenderBlockPass() const;

    virtual void onBlockDestroyedByPlayer(World &world, int_t x, int_t y, int_t z, int_t metadata);

    virtual int_t quantityDropped(Random random);

    virtual int_t idDropped(int_t var1, Random random);

    virtual void onBlockAdded(World &world, int_t x, int_t y, int_t z);

    virtual void onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId);

    virtual void onBlockClicked(World &world, int_t x, int_t y, int_t z, EntityPlayer &player);

    virtual bool blockActivated(World &world, int_t x, int_t y, int_t z, EntityPlayer &player);

    virtual float blockStrength(EntityPlayer &player) const;

    virtual void dropBlockAsItem(World &world, int_t x, int_t y, int_t z, int_t metadata);

    virtual void dropBlockAsItemWithChance(World &world, int_t x, int_t y, int_t z, int_t metadata, float chance);

    virtual bool canCollideCheck(int_t var1, bool var2) const;

    virtual void velocityToAddToEntity(World &world, int_t x, int_t y, int_t z, Entity &entity,
                                       Vec3D &velocity) const;

    virtual MovingObjectPosition collisionRayTrace(World &world, int_t x, int_t y, int_t z, const Vec3D &start,
                                                   const Vec3D &end);

    virtual void harvestBlock(World &world, int_t x, int_t y, int_t z, int_t metadata);

    virtual bool renderAsNormalBlock();

    virtual bool canBlockStay(World &world, int x, int y, int z);

private:
    std::array<int_t, 6> blockTextures{};

    bool isVecInsideYZBounds(const std::unique_ptr<Vec3D> &vec) const;

    bool isVecInsideXZBounds(const std::unique_ptr<Vec3D> &vec) const;

    bool isVecInsideXYBounds(const std::unique_ptr<Vec3D> &vec) const;
};

#endif //MCPORT_BLOCK_H
