//
// Created by Keir Davis on 25/06/2026.
//

#include "Chunk.h"

#include <iostream>

#include "game/block/Block.h"
#include "game/client/MathHelper.h"
#include "game/world/World.h"

bool Chunk::isLit = false;

Chunk::Chunk(World &world, const int_t chunkX, const int_t chunkZ) : worldObj(world), xPosition(chunkX),
                                                                     zPosition(chunkZ) {
}

Chunk::Chunk(World &world, const std::array<int_t, width * height * depth> &blocks, const int_t chunkX,
             const int_t chunkZ) : worldObj(world), blocks(blocks), xPosition(chunkX), zPosition(chunkZ) {
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
    const int_t oldBlockId = blocks[blockIndex(x, y, z)];
    if (oldBlockId == blockId) {
        return false;
    }

    const int_t oldHeight = heightMap[heightIndex(x, z)];
    const int_t worldX = xPosition * width + x;
    const int_t worldZ = zPosition * depth + z;
    blocks[blockIndex(x, y, z)] = blockId;
    metadata[blockIndex(x, y, z)] = 0;

    if (Block::lightOpacity[blockId] != 0) {
        if (y >= oldHeight) {
            relightBlock(x, y + 1, z);
        }
    } else if (y == oldHeight - 1) {
        relightBlock(x, y, z);
    }

    worldObj.scheduleLightingUpdate(EnumSkyBlock::Sky, worldX, y, worldZ, worldX, y, worldZ);
    worldObj.scheduleLightingUpdate(EnumSkyBlock::Block, worldX, y, worldZ, worldX, y, worldZ);
    updateSkylight_do(x, z);
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
    if (metadata[blockIndex(x, y, z)] == value) {
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
            while (columnHeight > 0 && Block::lightOpacity[getBlockID(x, columnHeight - 1, z)] == 0) {
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
    return skyBlock == EnumSkyBlock::Sky ? skylightMap[blockIndex(x, y, z)] : blocklightMap[blockIndex(x, y, z)];
}

void Chunk::setLightValue(const EnumSkyBlock skyBlock, const int_t x, const int_t y, const int_t z,
                          const int_t value) {
    isModified = true;
    if (skyBlock == EnumSkyBlock::Sky) {
        skylightMap[blockIndex(x, y, z)] = value & 15;
    } else {
        blocklightMap[blockIndex(x, y, z)] = value & 15;
    }
}

int_t Chunk::getBlockLightValue(const int_t x, const int_t y, const int_t z,
                                const int_t skylightSubtracted) const {
    int_t skyLight = skylightMap[blockIndex(x, y, z)];
    if (skyLight > 0) {
        isLit = true;
    }

    skyLight -= skylightSubtracted;
    const int_t blockLight = blocklightMap[blockIndex(x, y, z)];
    if (blockLight > skyLight) {
        skyLight = blockLight;
    }

    return skyLight;
}

void Chunk::generateSkylightMap() {
    int_t minHeight = 127;

    for (int_t x = 0; x < width; ++x) {
        for (int_t z = 0; z < depth; ++z) {
            heightMap[heightIndex(x, z)] = 128;
            relightBlock(x, 127, z);
            if (heightMap[heightIndex(x, z)] < minHeight) {
                minHeight = heightMap[heightIndex(x, z)];
            }
        }
    }

    heightValue = minHeight;

    for (int_t x = 0; x < width; ++x) {
        for (int_t z = 0; z < depth; ++z) {
            updateSkylight_do(x, z);
        }
    }

    isModified = true;
}

void Chunk::updateSkylight_do(const int_t x, const int_t z) {
    const int_t columnHeight = getHeightValue(x, z);
    const int_t worldX = xPosition * width + x;
    const int_t worldZ = zPosition * depth + z;
    checkSkylightNeighborHeight(worldX - 1, worldZ, columnHeight);
    checkSkylightNeighborHeight(worldX + 1, worldZ, columnHeight);
    checkSkylightNeighborHeight(worldX, worldZ - 1, columnHeight);
    checkSkylightNeighborHeight(worldX, worldZ + 1, columnHeight);
}

void Chunk::checkSkylightNeighborHeight(const int_t x, const int_t z, const int_t height) {
    const int_t neighborHeight = worldObj.getHeightValue(x, z);
    if (neighborHeight > height) {
        worldObj.scheduleLightingUpdate(EnumSkyBlock::Sky, x, height, z, x, neighborHeight, z);
    } else if (neighborHeight < height) {
        worldObj.scheduleLightingUpdate(EnumSkyBlock::Sky, x, neighborHeight, z, x, height, z);
    }
    isModified = true;
}

void Chunk::relightBlock(const int_t x, int_t y, const int_t z) {
    const int_t oldHeight = heightMap[heightIndex(x, z)];
    int_t newHeight = oldHeight;
    if (y > oldHeight) {
        newHeight = y;
    }

    while (newHeight > 0 && Block::lightOpacity[getBlockID(x, newHeight - 1, z)] == 0) {
        --newHeight;
    }

    if (newHeight != oldHeight) {
        worldObj.markBlocksDirtyVertical(x, z, newHeight, oldHeight);
        heightMap[heightIndex(x, z)] = newHeight;

        if (newHeight < heightValue) {
            heightValue = newHeight;
        } else {
            heightValue = 127;
            for (int_t localX = 0; localX < width; ++localX) {
                for (int_t localZ = 0; localZ < depth; ++localZ) {
                    if (heightMap[heightIndex(localX, localZ)] < heightValue) {
                        heightValue = heightMap[heightIndex(localX, localZ)];
                    }
                }
            }
        }

        const int_t worldX = xPosition * width + x;
        const int_t worldZ = zPosition * depth + z;
        if (newHeight < oldHeight) {
            for (int_t localY = newHeight; localY < oldHeight && localY < height; ++localY) {
                skylightMap[blockIndex(x, localY, z)] = 15;
            }
        } else {
            worldObj.scheduleLightingUpdate(EnumSkyBlock::Sky, worldX, oldHeight, worldZ, worldX, newHeight, worldZ);
            for (int_t localY = oldHeight; localY < newHeight && localY < height; ++localY) {
                skylightMap[blockIndex(x, localY, z)] = 0;
            }
        }

        int_t light = 15;
        int_t originalNewHeight = newHeight;

        while (newHeight > 0 && light > 0) {
            --newHeight;

            int_t opacity = Block::lightOpacity[getBlockID(x, newHeight, z)];
            if (opacity == 0) {
                opacity = 1;
            }

            light -= opacity;
            if (light < 0) {
                light = 0;
            }

            skylightMap[blockIndex(x, newHeight, z)] = light;
        }

        while (newHeight > 0 && Block::lightOpacity[getBlockID(x, newHeight - 1, z)] == 0) {
            --newHeight;
        }

        if (newHeight != originalNewHeight) {
            worldObj.scheduleLightingUpdate(EnumSkyBlock::Sky, worldX - 1, newHeight, worldZ - 1, worldX + 1,
                                            originalNewHeight, worldZ + 1);
        }

        isModified = true;
    }
}

void Chunk::addEntity(Entity &entity) {
    if (isChunkRendered) {
        return;
    }

    hasEntities = true;

    const int_t chunkX = MathHelper::floor_double(entity.posX / 16.0);
    const int_t chunkZ = MathHelper::floor_double(entity.posZ / 16.0);

    if (chunkX != xPosition || chunkZ != zPosition) {
        std::cout << "Wrong location! " << &entity << '\n';
    }

    int_t verticalIndex = MathHelper::floor_double(entity.posY / 16.0);
    if (verticalIndex < 0) {
        verticalIndex = 0;
    }

    if (verticalIndex >= static_cast<int_t>(entities.size())) {
        verticalIndex = static_cast<int_t>(entities.size()) - 1;
    }

    entity.addedToChunk = true;
    entity.chunkCoordX = xPosition;
    entity.chunkCoordY = verticalIndex;
    entity.chunkCoordZ = zPosition;

    entities[verticalIndex].push_back(&entity);
}

void Chunk::removeEntity(Entity &entity) {
    removeEntityAtIndex(entity, entity.chunkCoordY);
}

void Chunk::removeEntityAtIndex(Entity &entity, int_t index) {
    if (index < 0) {
        index = 0;
    }

    if (index >= static_cast<int_t>(entities.size())) {
        index = static_cast<int_t>(entities.size()) - 1;
    }

    auto &list = entities[index];
    list.erase(std::remove(list.begin(), list.end(), &entity), list.end());

    entity.addedToChunk = false;
}
