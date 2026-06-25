//
// Created by Keir Davis on 25/06/2026.
//

#include "BlockFluid.h"

#include <algorithm>
#include <cmath>

#include "game/world/IBlockAccess.h"
#include "game/world/World.h"
#include "game/entity/Entity.h"
#include "game/util/Vec3D.h"

namespace {
    int_t getFlowDecay(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                       const Material *material) {
        if (blockAccess.getBlockMaterial(x, y, z) != material) {
            return -1;
        }

        return blockAccess.getBlockMetadata(x, y, z);
    }

    int_t getEffectiveFlowDecay(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                                const Material *material) {
        if (blockAccess.getBlockMaterial(x, y, z) != material) {
            return -1;
        }

        int_t decay = blockAccess.getBlockMetadata(x, y, z);
        if (decay >= 8) {
            decay = 0;
        }
        return decay;
    }

    Vec3D getFlowVector(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                        const Material *material) {
        Vec3D flowVector(0.0, 0.0, 0.0);
        const int_t centerDecay = getEffectiveFlowDecay(blockAccess, x, y, z, material);

        for (int_t direction = 0; direction < 4; ++direction) {
            int_t neighborX = x;
            int_t neighborZ = z;
            if (direction == 0) {
                --neighborX;
            } else if (direction == 1) {
                --neighborZ;
            } else if (direction == 2) {
                ++neighborX;
            } else if (direction == 3) {
                ++neighborZ;
            }

            const int_t neighborDecay = getEffectiveFlowDecay(blockAccess, neighborX, y, neighborZ, material);
            if (neighborDecay >= 0) {
                const int_t delta = neighborDecay - centerDecay;
                flowVector.xCoord += static_cast<double>(neighborX - x) * static_cast<double>(delta);
                flowVector.zCoord += static_cast<double>(neighborZ - z) * static_cast<double>(delta);
            } else if (!blockAccess.getBlockMaterial(neighborX, y, neighborZ)->getIsSolid()) {
                const int_t belowDecay = getEffectiveFlowDecay(blockAccess, neighborX, y - 1, neighborZ, material);
                if (belowDecay >= 0) {
                    const int_t delta = belowDecay - (centerDecay - 8);
                    flowVector.xCoord += static_cast<double>(neighborX - x) * static_cast<double>(delta);
                    flowVector.zCoord += static_cast<double>(neighborZ - z) * static_cast<double>(delta);
                }
            }
        }

        if (blockAccess.getBlockMetadata(x, y, z) >= 8) {
            bool sideVisible = false;
            if (sideVisible || blockAccess.getBlockMaterial(x, y, z - 1) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x, y, z + 1) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x - 1, y, z) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x + 1, y, z) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x, y + 1, z - 1) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x, y + 1, z + 1) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x - 1, y + 1, z) != material) {
                sideVisible = true;
            }
            if (sideVisible || blockAccess.getBlockMaterial(x + 1, y + 1, z) != material) {
                sideVisible = true;
            }

            if (sideVisible) {
                std::unique_ptr<Vec3D> normalized = flowVector.normalize();
                flowVector = normalized->setComponents(normalized->xCoord, normalized->yCoord - 6.0,
                                                       normalized->zCoord);
            }
        }

        return flowVector;
    }
}

BlockFluid::BlockFluid(const int_t blockID, Material *material)
    : Block(blockID, (material == Material::lava ? 14 : 12) * 16 + 13, material) {
    if (material == Material::lava) {
        fluidType = 2;
    }
    setBlockBounds(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    slipperiness = 0.8f;
    setTickOnLoad(true);
    if (blockID >= 0 && blockID < static_cast<int_t>(lightOpacity.size())) {
        lightOpacity[blockID] = material == Material::lava ? 255 : 3;
        lightValue[blockID] = material == Material::lava ? 15 : 0;
    }
}

float BlockFluid::getFluidHeightPercent(const int_t metadata) {
    const int_t normalized = metadata >= 8 ? 0 : metadata;
    return static_cast<float>(normalized + 1) / 9.0f;
}

int_t BlockFluid::getRenderType() const {
    return 4;
}

int_t BlockFluid::getBlockTextureFromSide(const int_t side) const {
    return side != 0 && side != 1 ? blockIndexInTexture + 1 : blockIndexInTexture;
}

int_t BlockFluid::getBlockTextureFromSideAndMetadata(const int_t side, const int_t) const {
    return getBlockTextureFromSide(side);
}

bool BlockFluid::shouldSideBeRendered(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                                      const int_t side) const {
    Material *neighborMaterial = blockAccess.getBlockMaterial(x, y, z);
    if (neighborMaterial == material || neighborMaterial == Material::ice) {
        return false;
    }

    if (side == 1) {
        return true;
    }

    return Block::shouldSideBeRendered(blockAccess, x, y, z, side);
}

bool BlockFluid::canCollideCheck(const int_t metadata, const bool ignoreLiquid) const {
    return ignoreLiquid && metadata == 0;
}

float BlockFluid::getBlockBrightness(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z) const {
    const float brightness = blockAccess.getBrightness(x, y, z);
    const float brightnessAbove = blockAccess.getBrightness(x, y + 1, z);
    return brightness > brightnessAbove ? brightness : brightnessAbove;
}

int_t BlockFluid::getRenderBlockPass() const {
    return material == Material::water ? 1 : 0;
}

void BlockFluid::velocityToAddToEntity(World &world, const int_t x, const int_t y, const int_t z, Entity &entity,
                                       Vec3D &velocity) const {
    Vec3D flowVector = getFlowVector(world, x, y, z, material);
    std::unique_ptr<Vec3D> normalized = flowVector.normalize();
    velocity.xCoord += normalized->xCoord;
    velocity.yCoord += normalized->yCoord;
    velocity.zCoord += normalized->zCoord;
}

double BlockFluid::getFlowDirection(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                                    Material *material) {
    const Vec3D flowVector = getFlowVector(blockAccess, x, y, z, material);
    if (flowVector.xCoord == 0.0 && flowVector.zCoord == 0.0) {
        return -1000.0;
    }

    return std::atan2(flowVector.zCoord, flowVector.xCoord) - std::acos(-1.0) * 0.5;
}

int_t BlockFluid::tickRate() const {
    return material == Material::water ? 5 : (material == Material::lava ? 30 : 0);
}
