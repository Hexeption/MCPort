//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_BLOCK_H
#define MCPORT_BLOCK_H

#include <array>

#include "Material.h"
#include "java/Type.h"

class Random;
class World;
class IBlockAccess;

class Block {
public:
    static std::array<Block *, 256> blocksList;
    static std::array<int_t, 256> lightOpacity;
    static std::array<int_t, 256> lightValue;
    static Block *stone;
    static Block *grass;
    static Block *dirt;
    static Block *sand;

    int_t blockID;
    Material *material;
    double minX = 0.0;
    double minY = 0.0;
    double minZ = 0.0;
    double maxX = 1.0;
    double maxY = 1.0;
    double maxZ = 1.0;
    float slipperiness = 0.6f;

    Block(int_t blockID, int_t blockIndexInTexture, Material *material);

    Block(int_t blockID, int_t topTexture, int_t bottomTexture, int_t sideTexture, Material *material);

    int_t getRenderType() const;

    void setBlockBoundsForItemRender();

    int_t getBlockTextureFromSide(int_t side) const;

    bool isOpaqueCube() const;

    float getBlockBrightness(IBlockAccess &blockAccess, int_t x, int_t y, int_t z) const;

    bool shouldSideBeRendered(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const;

    virtual void updateTick(World &world, int_t x, int_t y, int_t z, Random &random);

    virtual void randomDisplayTick(World &world, int_t x, int_t y, int_t z, Random &random);

    virtual bool canPlaceBlockAt(World &world, int_t x, int_t y, int_t z) const;

    virtual int_t tickRate() const;

private:
    int_t blockIndexInTexture;
    std::array<int_t, 6> blockTextures{};
};

#endif //MCPORT_BLOCK_H
