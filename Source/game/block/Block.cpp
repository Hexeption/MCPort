//
// Created by Keir Davis on 24/06/2026.
//

#include "Block.h"

#include "game/entity/EntityPlayer.h"
#include "game/world/IBlockAccess.h"
#include "game/world/World.h"

struct BlockMaterialBootstrap {
    BlockMaterialBootstrap() {
        Material::init();
    }
};

static BlockMaterialBootstrap blockMaterialBootstrap;

std::array<Block *, 256> Block::blocksList = {};
std::array<int_t, 256> Block::lightOpacity = [] {
    std::array<int_t, 256> values{};
    values.fill(0);
    return values;
}();
std::array<int_t, 256> Block::lightValue = {};
Block *Block::stone = (new Block(1, 1, Material::rock))->setHardness(1.5f);
Block *Block::grass = (new Block(2, 0, 2, 3, Material::grass))->setHardness(0.6f);
Block *Block::dirt = (new Block(3, 2, Material::grass))->setHardness(0.5f);
Block *Block::sand = (new Block(12, 18, Material::sand))->setHardness(0.5f);

Block::Block(const int_t blockID, const int_t blockIndexInTexture, Material *material) : blockID(blockID),
    material(material), blockIndexInTexture(blockIndexInTexture) {
    blockTextures.fill(blockIndexInTexture);
    if (blockID >= 0 && blockID < static_cast<int_t>(blocksList.size())) {
        blocksList[blockID] = this;
        lightOpacity[blockID] = isOpaqueCube() ? 255 : 0;
    }
}

Block::Block(const int_t blockID, const int_t topTexture, const int_t bottomTexture, const int_t sideTexture,
             Material *material) : Block(blockID, sideTexture, material) {
    blockTextures[0] = bottomTexture;
    blockTextures[1] = topTexture;
}

Block *Block::setHardness(const float newHardness) {
    hardness = newHardness;
    return this;
}

int_t Block::getRenderType() const {
    return 0;
}

void Block::setBlockBoundsForItemRender() {
}

int_t Block::getBlockTextureFromSide(const int_t side) const {
    if (side >= 0 && side < static_cast<int_t>(blockTextures.size())) {
        return blockTextures[side];
    }
    return blockIndexInTexture;
}

bool Block::isOpaqueCube() const {
    return material != nullptr && material->isSolid();
}

float Block::getBlockBrightness(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z) const {
    return blockAccess.getBrightness(x, y, z);
}

bool Block::shouldSideBeRendered(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                                 int_t) const {
    return !blockAccess.isBlockNormalCube(x, y, z);
}

void Block::setBlockBounds(const float newMinX, const float newMinY, const float newMinZ, const float newMaxX,
                           const float newMaxY, const float newMaxZ) {
    minX = static_cast<double>(newMinX);
    minY = static_cast<double>(newMinY);
    minZ = static_cast<double>(newMinZ);
    maxX = static_cast<double>(newMaxX);
    maxY = static_cast<double>(newMaxY);
    maxZ = static_cast<double>(newMaxZ);
}

AxisAlignedBB Block::getSelectedBoundingBoxFromPool(const World &, const int_t x, const int_t y,
                                                    const int_t z) const {
    return AxisAlignedBB::getBoundingBox(static_cast<double>(x) + minX, static_cast<double>(y) + minY,
                                         static_cast<double>(z) + minZ, static_cast<double>(x) + maxX,
                                         static_cast<double>(y) + maxY, static_cast<double>(z) + maxZ);
}

AxisAlignedBB Block::getCollisionBoundingBoxFromPool(const World &, const int_t x, const int_t y,
                                                     const int_t z) const {
    return AxisAlignedBB::getBoundingBox(static_cast<double>(x) + minX, static_cast<double>(y) + minY,
                                         static_cast<double>(z) + minZ, static_cast<double>(x) + maxX,
                                         static_cast<double>(y) + maxY, static_cast<double>(z) + maxZ);
}

void Block::setBlockBoundsBasedOnState(IBlockAccess &, int_t, int_t, int_t) {
}

void Block::updateTick(World &world, int_t x, int_t y, int_t z, Random &random) {
}

void Block::randomDisplayTick(World &world, int_t x, int_t y, int_t z, Random &random) {
}

bool Block::canPlaceBlockAt(World &world, const int_t x, const int_t y, const int_t z) const {
    const int_t blockId = world.getBlockId(x, y, z);
    return blockId == 0 || (blockId >= 0 && blockId < static_cast<int_t>(blocksList.size()) &&
                            blocksList[blockId] != nullptr && blocksList[blockId]->material != nullptr &&
                            blocksList[blockId]->material->getIsLiquid());
}

int_t Block::tickRate() const {
    return 10;
}

void Block::onBlockDestroyedByPlayer(World &, int_t, int_t, int_t, int_t) {
}

void Block::onBlockClicked(World &, int_t, int_t, int_t, EntityPlayer &) {
}

float Block::blockStrength(EntityPlayer &player) const {
    if (hardness < 0.0f) {
        return 0.0f;
    }

    if (!player.canHarvestBlock(*this)) {
        return 1.0f / hardness / 100.0f;
    }

    return player.getCurrentPlayerStrVsBlock(*this) / hardness / 30.0f;
}

bool Block::canCollideCheck(int_t, bool) const {
    return isOpaqueCube();
}

MovingObjectPosition Block::collisionRayTrace(World &, const int_t x, const int_t y, const int_t z,
                                              const Vec3D &start, const Vec3D &end) {
    const std::unique_ptr<Vec3D> localStart = start.addVector(static_cast<double>(-x), static_cast<double>(-y),
                                                              static_cast<double>(-z));
    const std::unique_ptr<Vec3D> localEnd = end.addVector(static_cast<double>(-x), static_cast<double>(-y),
                                                          static_cast<double>(-z));
    std::unique_ptr<Vec3D> minXHit = localStart->getIntermediateWithXValue(*localEnd, minX);
    std::unique_ptr<Vec3D> maxXHit = localStart->getIntermediateWithXValue(*localEnd, maxX);
    std::unique_ptr<Vec3D> minYHit = localStart->getIntermediateWithYValue(*localEnd, minY);
    std::unique_ptr<Vec3D> maxYHit = localStart->getIntermediateWithYValue(*localEnd, maxY);
    std::unique_ptr<Vec3D> minZHit = localStart->getIntermediateWithZValue(*localEnd, minZ);
    std::unique_ptr<Vec3D> maxZHit = localStart->getIntermediateWithZValue(*localEnd, maxZ);

    if (!isVecInsideYZBounds(minXHit)) {
        minXHit = nullptr;
    }
    if (!isVecInsideYZBounds(maxXHit)) {
        maxXHit = nullptr;
    }
    if (!isVecInsideXZBounds(minYHit)) {
        minYHit = nullptr;
    }
    if (!isVecInsideXZBounds(maxYHit)) {
        maxYHit = nullptr;
    }
    if (!isVecInsideXYBounds(minZHit)) {
        minZHit = nullptr;
    }
    if (!isVecInsideXYBounds(maxZHit)) {
        maxZHit = nullptr;
    }

    Vec3D *hit = nullptr;
    if (minXHit != nullptr && (hit == nullptr || localStart->distanceTo(*minXHit) < localStart->distanceTo(*hit))) {
        hit = minXHit.get();
    }
    if (maxXHit != nullptr && (hit == nullptr || localStart->distanceTo(*maxXHit) < localStart->distanceTo(*hit))) {
        hit = maxXHit.get();
    }
    if (minYHit != nullptr && (hit == nullptr || localStart->distanceTo(*minYHit) < localStart->distanceTo(*hit))) {
        hit = minYHit.get();
    }
    if (maxYHit != nullptr && (hit == nullptr || localStart->distanceTo(*maxYHit) < localStart->distanceTo(*hit))) {
        hit = maxYHit.get();
    }
    if (minZHit != nullptr && (hit == nullptr || localStart->distanceTo(*minZHit) < localStart->distanceTo(*hit))) {
        hit = minZHit.get();
    }
    if (maxZHit != nullptr && (hit == nullptr || localStart->distanceTo(*maxZHit) < localStart->distanceTo(*hit))) {
        hit = maxZHit.get();
    }

    if (hit == nullptr) {
        return {};
    }

    int_t sideHit = -1;
    if (hit == minXHit.get()) {
        sideHit = 4;
    }
    if (hit == maxXHit.get()) {
        sideHit = 5;
    }
    if (hit == minYHit.get()) {
        sideHit = 0;
    }
    if (hit == maxYHit.get()) {
        sideHit = 1;
    }
    if (hit == minZHit.get()) {
        sideHit = 2;
    }
    if (hit == maxZHit.get()) {
        sideHit = 3;
    }

    return MovingObjectPosition(x, y, z, sideHit,
                                hit->addVector(static_cast<double>(x), static_cast<double>(y),
                                               static_cast<double>(z)));
}

bool Block::isVecInsideYZBounds(const std::unique_ptr<Vec3D> &vec) const {
    return vec != nullptr && vec->yCoord >= minY && vec->yCoord <= maxY && vec->zCoord >= minZ && vec->zCoord <= maxZ;
}

bool Block::isVecInsideXZBounds(const std::unique_ptr<Vec3D> &vec) const {
    return vec != nullptr && vec->xCoord >= minX && vec->xCoord <= maxX && vec->zCoord >= minZ && vec->zCoord <= maxZ;
}

bool Block::isVecInsideXYBounds(const std::unique_ptr<Vec3D> &vec) const {
    return vec != nullptr && vec->xCoord >= minX && vec->xCoord <= maxX && vec->yCoord >= minY && vec->yCoord <= maxY;
}
