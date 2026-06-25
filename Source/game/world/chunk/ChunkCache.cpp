//
// Created by Keir Davis on 25/06/2026.
//

#include "ChunkCache.h"

#include "game/world/World.h"

ChunkCache::ChunkCache(World &world, const int_t x0, int_t, const int_t z0, const int_t x1, int_t, const int_t z1)
    : worldObj(world) {
    chunkX = x0 >> 4;
    chunkZ = z0 >> 4;
    chunkArrayWidth = (x1 >> 4) - chunkX + 1;
    chunkArrayDepth = (z1 >> 4) - chunkZ + 1;
}

int_t ChunkCache::getBlockId(const int_t x, const int_t y, const int_t z) const {
    return worldObj.getBlockId(x, y, z);
}

float ChunkCache::getBrightness(const int_t x, const int_t y, const int_t z) const {
    return worldObj.getBrightness(x, y, z);
}

int_t ChunkCache::getBlockMetadata(const int_t x, const int_t y, const int_t z) const {
    return worldObj.getBlockMetadata(x, y, z);
}

Material *ChunkCache::getBlockMaterial(const int_t x, const int_t y, const int_t z) const {
    return worldObj.getBlockMaterial(x, y, z);
}

bool ChunkCache::isBlockNormalCube(const int_t x, const int_t y, const int_t z) const {
    return worldObj.isBlockNormalCube(x, y, z);
}
