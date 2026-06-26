//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockFlower.h"

#include "game/world/World.h"

BlockFlower::BlockFlower(int_t blockID, int_t blockIndexInTexture) : Block(
    blockID, blockIndexInTexture, Material::plants) {
    setTickOnLoad(true);
    float var3 = 0.2f;
    setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 3.0F, 0.5F + var3);
}

bool BlockFlower::canPlaceBlockAt(World &world, int_t x, int_t y, int_t z) const {
    return canThisPlantGrowOnThisBlockID(world.getBlockId(x, y - 1, z));
}

bool BlockFlower::canThisPlantGrowOnThisBlockID(int blockID) const {
    return blockID == Block::grass->blockID || blockID == Block::dirt->blockID;
}

void BlockFlower::onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId) {
    Block::onNeighborBlockChange(world, x, y, z, neighborBlockId);
    checkFlowerChange(world, x, y, z);
}

void BlockFlower::updateTick(World &world, int_t x, int_t y, int_t z, Random &random) {
    checkFlowerChange(world, x, y, z);
}

void BlockFlower::checkFlowerChange(World &world, int_t x, int_t y, int_t z) {
    if (!canBlockStay(world, x, y, z)) {
        dropBlockAsItem(world, x, y, z, world.getBlockMetadata(x, y, z));
        world.setBlockWithNotify(x, y, z, 0);
    }
}

bool BlockFlower::canBlockStay(World &world, int x, int y, int z) {
    return (world.getBlockLightValue(x, y, z) >= 8 || world.canBlockSeeTheSky(x, y, z) && canThisPlantGrowOnThisBlockID(
                world.getBlockId(x, y - 1, z)));
}

AxisAlignedBB BlockFlower::getCollisionBoundingBoxFromPool(const World &world, int_t x, int_t y, int_t z) const {
    return {};
}

bool BlockFlower::isOpaqueCube() const {
    return false;
}

bool BlockFlower::renderAsNormalBlock() {
    return false;
}

int_t BlockFlower::getRenderType() const {
    return 1;
}
