//
// Created by Keir Davis on 25/06/2026.
//

#include "Chunk.h"

#include <algorithm>

#include "game/block/Block.h"
#include "game/world/World.h"

Chunk::Chunk(World &world, const int_t chunkX, const int_t chunkZ) : worldObj(world), xPosition(chunkX),
                                                                     zPosition(chunkZ) {
    generateSkylightMap();
}

Chunk::Chunk(World &world, const std::array<int_t, width * height * depth> &blocks, const int_t chunkX,
             const int_t chunkZ) : worldObj(world), blocks(blocks), xPosition(chunkX), zPosition(chunkZ) {
    generateSkylightMap();
}

int_t Chunk::getBlockID(const int_t x, const int_t y, const int_t z) const {
    if (!isValidLocalBlock(x, y, z)) {
        return 0;
    }
    return blocks[blockIndex(x, y, z)];
}

bool Chunk::setBlockID(const int_t x, const int_t y, const int_t z, const int_t blockId) {
    if (!isValidLocalBlock(x, y, z)) {
        return false;
    }
    blocks[blockIndex(x, y, z)] = blockId;
    relightBlock(x, y, z);
    return true;
}

int_t Chunk::getBlockMetadata(const int_t x, const int_t y, const int_t z) const {
    if (!isValidLocalBlock(x, y, z)) {
        return 0;
    }
    return metadata[blockIndex(x, y, z)];
}

bool Chunk::setBlockMetadata(const int_t x, const int_t y, const int_t z, const int_t value) {
    if (!isValidLocalBlock(x, y, z)) {
        return false;
    }
    metadata[blockIndex(x, y, z)] = value;
    return true;
}

int_t Chunk::getHeightValue(const int_t x, const int_t z) const {
    if (x < 0 || x >= width || z < 0 || z >= depth) {
        return 0;
    }
    return heightMap[heightIndex(x, z)];
}

int_t Chunk::blockIndex(const int_t x, const int_t y, const int_t z) {
    return x << 11 | z << 7 | y;
}

int_t Chunk::heightIndex(const int_t x, const int_t z) {
    return z << 4 | x;
}

bool Chunk::isValidLocalBlock(const int_t x, const int_t y, const int_t z) {
    return x >= 0 && x < width && z >= 0 && z < depth && y >= 0 && y < height;
}

void Chunk::generateHeightMap() {
    int_t minimumHeight = 127;
    for (int_t x = 0; x < width; ++x) {
        for (int_t z = 0; z < depth; ++z) {
            int_t columnHeight = 127;
            while (columnHeight > 0 && getBlockID(x, columnHeight - 1, z) == 0) {
                --columnHeight;
            }
            heightMap[heightIndex(x, z)] = columnHeight;
            if (columnHeight < minimumHeight) {
                minimumHeight = columnHeight;
            }
        }
    }
    heightValue = minimumHeight;
    isModified = true;
}

int_t Chunk::getSavedLightValue(const EnumSkyBlock skyBlock, const int_t x, const int_t y, const int_t z) const {
    if (!isValidLocalBlock(x, y, z)) {
        return skyBlock == EnumSkyBlock::Sky ? 15 : 0;
    }
    return skyBlock == EnumSkyBlock::Sky ? skylightMap[blockIndex(x, y, z)] : blocklightMap[blockIndex(x, y, z)];
}

void Chunk::setLightValue(const EnumSkyBlock skyBlock, const int_t x, const int_t y, const int_t z,
                          const int_t value) {
    if (!isValidLocalBlock(x, y, z)) {
        return;
    }

    const int_t clampedValue = std::clamp(value, 0, 15);
    if (skyBlock == EnumSkyBlock::Sky) {
        skylightMap[blockIndex(x, y, z)] = clampedValue;
    } else {
        blocklightMap[blockIndex(x, y, z)] = clampedValue;
    }
    isModified = true;
}

int_t Chunk::getBlockLightValue(const int_t x, const int_t y, const int_t z,
                                const int_t skylightSubtracted) const {
    if (!isValidLocalBlock(x, y, z)) {
        return y >= height ? std::max(0, 15 - skylightSubtracted) : 0;
    }

    int_t skyLight = skylightMap[blockIndex(x, y, z)] - skylightSubtracted;
    if (skyLight < 0) {
        skyLight = 0;
    }
    const int_t blockLight = blocklightMap[blockIndex(x, y, z)];
    return blockLight > skyLight ? blockLight : skyLight;
}

void Chunk::generateSkylightMap() {
    generateHeightMap();
    skylightMap.fill(0);
    blocklightMap.fill(0);

    for (int_t x = 0; x < width; ++x) {
        for (int_t z = 0; z < depth; ++z) {
            int_t light = 15;
            const int_t columnHeight = heightMap[heightIndex(x, z)];

            for (int_t y = height - 1; y >= 0; --y) {
                if (y >= columnHeight) {
                    light = 15;
                } else {
                    int_t opacity = Block::lightOpacity[getBlockID(x, y, z)];
                    if (opacity == 0) {
                        opacity = 1;
                    }
                    light -= opacity;
                    if (light < 0) {
                        light = 0;
                    }
                }
                skylightMap[blockIndex(x, y, z)] = light;
            }
        }
    }
}

void Chunk::relightBlock(const int_t x, int_t y, const int_t z) {
    const int_t oldHeight = heightMap[heightIndex(x, z)];
    if (y > oldHeight) {
        y = oldHeight;
    }
    int_t newHeight = 127;
    while (newHeight > 0 && getBlockID(x, newHeight - 1, z) == 0) {
        --newHeight;
    }
    if (newHeight != oldHeight) {
        worldObj.markBlocksDirtyVertical(xPosition * 16 + x, zPosition * 16 + z, newHeight, oldHeight);
        heightMap[heightIndex(x, z)] = newHeight;
        int_t light = 15;
        for (int_t localY = height - 1; localY >= 0; --localY) {
            if (localY >= newHeight) {
                light = 15;
            } else {
                int_t opacity = Block::lightOpacity[getBlockID(x, localY, z)];
                if (opacity == 0) {
                    opacity = 1;
                }
                light -= opacity;
                if (light < 0) {
                    light = 0;
                }
            }
            skylightMap[blockIndex(x, localY, z)] = light;
        }
        heightValue = 127;
        for (int_t localX = 0; localX < width; ++localX) {
            for (int_t localZ = 0; localZ < depth; ++localZ) {
                heightValue = std::min(heightValue, heightMap[heightIndex(localX, localZ)]);
            }
        }
        isModified = true;
    }
}
