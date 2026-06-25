//
// Created by Keir Davis on 25/06/2026.
//

#include "MetadataChunkBlock.h"

#include "World.h"
#include "game/block/Block.h"

MetadataChunkBlock::MetadataChunkBlock(const EnumSkyBlock skyBlock, const int_t minX, const int_t minY,
                                       const int_t minZ, const int_t maxX, const int_t maxY, const int_t maxZ)
    : skyBlock(skyBlock), minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {
}

void MetadataChunkBlock::updateLight(World &world) {
    const int_t width = maxX - minX;
    const int_t height = maxY - minY;
    const int_t depth = maxZ - minZ;
    const int_t volume = width * height * depth;
    if (volume > 32768) {
        return;
    }

    for (int_t blockX = minX; blockX <= maxX; ++blockX) {
        for (int_t blockZ = minZ; blockZ <= maxZ; ++blockZ) {
            if (!world.blockExists(blockX, 0, blockZ)) {
                continue;
            }

            for (int_t blockY = minY; blockY <= maxY; ++blockY) {
                if (blockY < 0 || blockY >= 128) {
                    continue;
                }

                const int_t savedLightValue = world.getSavedLightValue(skyBlock, blockX, blockY, blockZ);
                const int_t blockId = world.getBlockId(blockX, blockY, blockZ);
                int_t lightOpacity = blockId >= 0 && blockId < static_cast<int_t>(Block::lightOpacity.size())
                                         ? Block::lightOpacity[blockId]
                                         : 0;
                if (lightOpacity == 0) {
                    lightOpacity = 1;
                }

                int_t emittedLight = 0;
                if (skyBlock == EnumSkyBlock::Sky) {
                    if (world.canExistingBlockSeeTheSky(blockX, blockY, blockZ)) {
                        emittedLight = 15;
                    }
                } else if (skyBlock == EnumSkyBlock::Block && blockId >= 0 &&
                           blockId < static_cast<int_t>(Block::lightValue.size())) {
                    emittedLight = Block::lightValue[blockId];
                }

                int_t newLightValue;
                if (lightOpacity >= 15 && emittedLight == 0) {
                    newLightValue = 0;
                } else {
                    int_t neighborLight = world.getSavedLightValue(skyBlock, blockX - 1, blockY, blockZ);
                    const int_t eastLight = world.getSavedLightValue(skyBlock, blockX + 1, blockY, blockZ);
                    const int_t downLight = world.getSavedLightValue(skyBlock, blockX, blockY - 1, blockZ);
                    const int_t upLight = world.getSavedLightValue(skyBlock, blockX, blockY + 1, blockZ);
                    const int_t northLight = world.getSavedLightValue(skyBlock, blockX, blockY, blockZ - 1);
                    const int_t southLight = world.getSavedLightValue(skyBlock, blockX, blockY, blockZ + 1);

                    if (eastLight > neighborLight) {
                        neighborLight = eastLight;
                    }
                    if (downLight > neighborLight) {
                        neighborLight = downLight;
                    }
                    if (upLight > neighborLight) {
                        neighborLight = upLight;
                    }
                    if (northLight > neighborLight) {
                        neighborLight = northLight;
                    }
                    if (southLight > neighborLight) {
                        neighborLight = southLight;
                    }

                    newLightValue = neighborLight - lightOpacity;
                    if (newLightValue < 0) {
                        newLightValue = 0;
                    }
                    if (emittedLight > newLightValue) {
                        newLightValue = emittedLight;
                    }
                }

                if (savedLightValue != newLightValue) {
                    world.setLightValue(skyBlock, blockX, blockY, blockZ, newLightValue);
                    int_t propagatedLight = newLightValue - 1;
                    if (propagatedLight < 0) {
                        propagatedLight = 0;
                    }

                    world.neighborLightPropagationChanged(skyBlock, blockX - 1, blockY, blockZ, propagatedLight);
                    world.neighborLightPropagationChanged(skyBlock, blockX, blockY - 1, blockZ, propagatedLight);
                    world.neighborLightPropagationChanged(skyBlock, blockX, blockY, blockZ - 1, propagatedLight);
                    if (blockX + 1 >= maxX) {
                        world.neighborLightPropagationChanged(skyBlock, blockX + 1, blockY, blockZ, propagatedLight);
                    }
                    if (blockY + 1 >= maxY) {
                        world.neighborLightPropagationChanged(skyBlock, blockX, blockY + 1, blockZ, propagatedLight);
                    }
                    if (blockZ + 1 >= maxZ) {
                        world.neighborLightPropagationChanged(skyBlock, blockX, blockY, blockZ + 1, propagatedLight);
                    }
                }
            }
        }
    }
}

bool MetadataChunkBlock::getLightUpdated(int_t newMinX, int_t newMinY, int_t newMinZ, int_t newMaxX,
                                         int_t newMaxY, int_t newMaxZ) {
    if (newMinX >= minX && newMinY >= minY && newMinZ >= minZ && newMaxX <= maxX && newMaxY <= maxY &&
        newMaxZ <= maxZ) {
        return true;
    }

    constexpr int_t expand = 1;
    if (newMinX < minX - expand || newMinY < minY - expand || newMinZ < minZ - expand ||
        newMaxX > maxX + expand || newMaxY > maxY + expand || newMaxZ > maxZ + expand) {
        return false;
    }

    const int_t oldWidth = maxX - minX;
    const int_t oldHeight = maxY - minY;
    const int_t oldDepth = maxZ - minZ;
    if (newMinX > minX) {
        newMinX = minX;
    }
    if (newMinY > minY) {
        newMinY = minY;
    }
    if (newMinZ > minZ) {
        newMinZ = minZ;
    }
    if (newMaxX < maxX) {
        newMaxX = maxX;
    }
    if (newMaxY < maxY) {
        newMaxY = maxY;
    }
    if (newMaxZ < maxZ) {
        newMaxZ = maxZ;
    }

    const int_t newWidth = newMaxX - newMinX;
    const int_t newHeight = newMaxY - newMinY;
    const int_t newDepth = newMaxZ - newMinZ;
    const int_t oldVolume = oldWidth * oldHeight * oldDepth;
    const int_t newVolume = newWidth * newHeight * newDepth;
    if (newVolume - oldVolume > 2) {
        return false;
    }

    minX = newMinX;
    minY = newMinY;
    minZ = newMinZ;
    maxX = newMaxX;
    maxY = newMaxY;
    maxZ = newMaxZ;
    return true;
}
