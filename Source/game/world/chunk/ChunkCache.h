//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_CHUNKCACHE_H
#define MCPORT_CHUNKCACHE_H

#include "game/world/IBlockAccess.h"
#include "java/Type.h"

class World;

class ChunkCache : public IBlockAccess {
public:
    World &worldObj;
    int_t chunkX = 0;
    int_t chunkZ = 0;
    int_t chunkArrayWidth = 0;
    int_t chunkArrayDepth = 0;

    ChunkCache(World &world, int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    int_t getBlockId(int_t x, int_t y, int_t z) const override;

    float getBrightness(int_t x, int_t y, int_t z) const override;

    int_t getBlockMetadata(int_t x, int_t y, int_t z) const override;

    Material *getBlockMaterial(int_t x, int_t y, int_t z) const override;

    bool isBlockNormalCube(int_t x, int_t y, int_t z) const override;
};

#endif //MCPORT_CHUNKCACHE_H
