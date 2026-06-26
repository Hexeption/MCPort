//
// Created by Keir Davis on 24/06/2026.
//

#include "Block.h"

#include "game/entity/EntityPlayer.h"
#include "game/world/IBlockAccess.h"
#include "game/world/World.h"
#include "BlockFlowing.h"
#include "BlockGrass.h"
#include "BlockGravel.h"
#include "BlockLeaves.h"
#include "BlockLog.h"
#include "BlockSand.h"
#include "BlockSapling.h"
#include "BlockStationary.h"
#include "game/entity/EntityItem.h"
#include "game/item/ItemStack.h"

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
std::array<bool, 256> Block::tickOnLoad = {};
std::array<bool, 256> Block::opaqueCubeLookup = {};
Block *Block::stone = (new Block(1, 1, Material::rock))->setHardness(1.5f);
Block *Block::grass = (new BlockGrass(2))->setHardness(0.6f);
Block *Block::dirt = (new Block(3, 2, Material::grass))->setHardness(0.5f);
Block *Block::cobblestone = (new Block(4, 16, Material::rock))->setHardness(2.0f);
Block *Block::sand = (new BlockSand(12, 18))->setHardness(0.5f);
Block *Block::bedrock = (new Block(7, 17, Material::rock))->setHardness(-1.0F);
Block *Block::gravel = (new BlockGravel(13, 19))->setHardness(0.6f);
Block *Block::obsidian = (new Block(49, 37, Material::rock))->setHardness(10.0f);
Block *Block::waterMoving = (new BlockFlowing(8, Material::water))->setHardness(100.0f);
Block *Block::waterStill = (new BlockStationary(9, Material::water))->setHardness(100.0f);
Block *Block::lavaMoving = (new BlockFlowing(10, Material::lava))->setHardness(0.0f);
Block *Block::lavaStill = (new BlockStationary(11, Material::lava))->setHardness(100.0f);
Block *Block::leaves = (new BlockLeaves(18, 52))->setHardness(0.2f)->setLightOpacity(1);
Block *Block::wood = (new BlockLog(17))->setHardness(2.0f);
Block *Block::sapling = (new BlockSapling(6, 15))->setHardness(0.0f)->setLightValue(1);
Block *Block::ladder = nullptr;

Block::Block(const int_t blockID, Material *material) : blockID(blockID), material(material), blockIndexInTexture(0) {
    blockTextures.fill(0);

    if (blockID >= 0 && blockID < static_cast<int_t>(blocksList.size())) {
        if (blocksList[blockID] != nullptr) {
            throw std::runtime_error("Duplicate block id");
        }

        blocksList[blockID] = this;
        lightOpacity[blockID] = isOpaqueCube() ? 255 : 0;
        opaqueCubeLookup[blockID] = isOpaqueCube();
    }

    setBlockBounds(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

Block::Block(const int_t blockID, const int_t blockIndexInTexture, Material *material)
    : Block(blockID, material) {
    this->blockIndexInTexture = blockIndexInTexture;
    blockTextures.fill(blockIndexInTexture);
}

Block::Block(const int_t blockID, const int_t topTexture, const int_t bottomTexture, const int_t sideTexture,
             Material *material) : Block(blockID, sideTexture, material) {
    blockTextures[0] = bottomTexture;
    blockTextures[1] = topTexture;
}

Block *Block::setLightOpacity(int_t opacity) {
    lightOpacity[blockID] = opacity;
    return this;
}

Block *Block::setLightValue(int_t value) {
    lightValue[blockID] = value;
    return this;
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

int_t Block::getBlockTexture(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const {
    return getBlockTextureFromSideAndMetadata(side, blockAccess.getBlockMetadata(x, y, z));
}

int_t Block::getBlockTextureFromSide(const int_t side) const {
    if (side >= 0 && side < static_cast<int_t>(blockTextures.size())) {
        return blockTextures[side];
    }
    return blockIndexInTexture;
}

int_t Block::getBlockTextureFromSideAndMetadata(const int_t side, const int_t) const {
    return getBlockTextureFromSide(side);
}

bool Block::isOpaqueCube() const {
    return material != nullptr && material->isSolid();
}

float Block::getBlockBrightness(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z) const {
    return blockAccess.getBrightness(x, y, z);
}

bool Block::shouldSideBeRendered(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                                 const int_t side) const {
    if (side == 0 && minY > 0.0) {
        return true;
    }
    if (side == 1 && maxY < 1.0) {
        return true;
    }
    if (side == 2 && minZ > 0.0) {
        return true;
    }
    if (side == 3 && maxZ < 1.0) {
        return true;
    }
    if (side == 4 && minX > 0.0) {
        return true;
    }
    if (side == 5 && maxX < 1.0) {
        return true;
    }
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

void Block::setTickOnLoad(const bool value) {
    if (blockID >= 0 && blockID < static_cast<int_t>(tickOnLoad.size())) {
        tickOnLoad[blockID] = value;
    }
}

int_t Block::getRenderBlockPass() const {
    return 0;
}

void Block::onBlockDestroyedByPlayer(World &, int_t, int_t, int_t, int_t) {
}

int Block::quantityDropped(Random random) {
    return 1;
}

int_t Block::idDropped(int_t var1, Random random) {
    return blockID;
}

void Block::onBlockAdded(World &, int_t, int_t, int_t) {
}

void Block::onNeighborBlockChange(World &, int_t, int_t, int_t, int_t) {
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

void Block::dropBlockAsItem(World &world, int_t x, int_t y, int_t z, int_t metadata) {
    dropBlockAsItemWithChance(world, x, y, z, metadata, 1.0f);
}

void Block::dropBlockAsItemWithChance(World &world, int_t x, int_t y, int_t z, int_t metadata, float chance) {
    if (world.multiplayerWorld) {
        return;
    }

    const int_t count = quantityDropped(world.rand);

    for (int_t i = 0; i < count; ++i) {
        if (world.rand.nextFloat() <= chance) {
            const int_t droppedId = idDropped(metadata, world.rand);

            if (droppedId > 0) {
                constexpr float offset = 0.7f;

                const double dx =
                        static_cast<double>(world.rand.nextFloat() * offset) +
                        static_cast<double>(1.0f - offset) * 0.5;

                const double dy =
                        static_cast<double>(world.rand.nextFloat() * offset) +
                        static_cast<double>(1.0f - offset) * 0.5;

                const double dz =
                        static_cast<double>(world.rand.nextFloat() * offset) +
                        static_cast<double>(1.0f - offset) * 0.5;

                auto item = std::make_unique<EntityItem>(
                    world,
                    static_cast<double>(x) + dx,
                    static_cast<double>(y) + dy,
                    static_cast<double>(z) + dz,
                    ItemStack(droppedId)
                );

                item->delayBeforeCanPickup = 10;
                world.spawnEntityInWorld(std::move(item));
            }
        }
    }
}

bool Block::canCollideCheck(int_t, bool) const {
    return true;
}

void Block::velocityToAddToEntity(World &, int_t, int_t, int_t, Entity &, Vec3D &) const {
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

void Block::harvestBlock(World &world, int_t x, int_t y, int_t z, int_t metadata) {
    dropBlockAsItem(world, x, y, z, metadata);
}

bool Block::renderAsNormalBlock() {
    return true;
}

bool Block::canBlockStay(World &world, int x, int y, int z) {
    return true;
}
