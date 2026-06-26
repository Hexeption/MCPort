//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockLeaves.h"

#include "game/block/Material.h"
#include "game/world/World.h"

BlockLeaves::BlockLeaves(int_t blockID, int_t blockIndexInTexture) : BlockLeavesBase(
    blockID, blockIndexInTexture, Material::leaves, false) {
    leafTexIndex = blockIndexInTexture;
    setTickOnLoad(true);
}

int_t BlockLeaves::quantityDropped(Random random) {
    return random.nextInt(20) == 0 ? 1 : 0;
}

int_t BlockLeaves::idDropped(int_t var1, Random random) {
    return Block::sapling->blockID;
}

void BlockLeaves::onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId) {
    decayCounter = 0;
    updateCurrentLeaves(world, x, y, z);
    BlockLeavesBase::onNeighborBlockChange(world, x, y, z, neighborBlockId);
}

void BlockLeaves::updateConnectedLeaves(World &world, int_t x, int_t y, int_t z, int_t strength) {
    if (world.getBlockId(x, y, z) == blockID) {
        const int_t metadata = world.getBlockMetadata(x, y, z);
        if (metadata != 0 && metadata == strength - 1) {
            updateCurrentLeaves(world, x, y, z);
        }
    }
}

void BlockLeaves::updateCurrentLeaves(World &world, int_t x, int_t y, int_t z) {
    if (decayCounter++ < 100) {
        int_t strength = world.getBlockMaterial(x, y - 1, z)->getIsSolid() ? 16 : 0;
        int_t metadata = world.getBlockMetadata(x, y, z);
        if (metadata == 0) {
            metadata = 1;
            world.setBlockMetadataWithNotify(x, y, z, 1);
        }

        strength = getConnectionStrength(world, x, y - 1, z, strength);
        strength = getConnectionStrength(world, x, y, z - 1, strength);
        strength = getConnectionStrength(world, x, y, z + 1, strength);
        strength = getConnectionStrength(world, x - 1, y, z, strength);
        strength = getConnectionStrength(world, x + 1, y, z, strength);

        int_t newMetadata = strength - 1;
        if (newMetadata < 10) {
            newMetadata = 1;
        }

        if (newMetadata != metadata) {
            world.setBlockMetadataWithNotify(x, y, z, newMetadata);
            updateConnectedLeaves(world, x, y - 1, z, metadata);
            updateConnectedLeaves(world, x, y + 1, z, metadata);
            updateConnectedLeaves(world, x, y, z - 1, metadata);
            updateConnectedLeaves(world, x, y, z + 1, metadata);
            updateConnectedLeaves(world, x - 1, y, z, metadata);
            updateConnectedLeaves(world, x + 1, y, z, metadata);
        }
    }
}

int_t BlockLeaves::getConnectionStrength(World &world, int_t x, int_t y, int_t z, int_t strength) {
    const int_t blockId = world.getBlockId(x, y, z);
    if (blockId == Block::wood->blockID) {
        return 16;
    } else {
        if (blockId == blockID) {
            const int_t metadata = world.getBlockMetadata(x, y, z);
            if (metadata != 0 && metadata > strength) {
                return metadata;
            }
        }
        return strength;
    }
}

void BlockLeaves::updateTick(World &world, int_t x, int_t y, int_t z, Random &random) {
    const int_t metadata = world.getBlockMetadata(x, y, z);
    if (metadata == 0) {
        decayCounter = 0;
        updateCurrentLeaves(world, x, y, z);
    } else if (metadata == 1) {
        removeLeaves(world, x, y, z);
    } else if (random.nextInt(10) == 0) {
        updateCurrentLeaves(world, x, y, z);
    }
}

void BlockLeaves::removeLeaves(World &world, int_t x, int_t y, int_t z) {
    dropBlockAsItem(world, x, y, z, world.getBlockMetadata(x, y, z));
    world.setBlockWithNotify(x, y, z, 0);
}

bool BlockLeaves::isOpaqueCube() const {
    return !graphicsLevel;
}

void BlockLeaves::setGraphicsLevel(bool fancyLeaves) {
    graphicsLevel = fancyLeaves;
    blockIndexInTexture = leafTexIndex + (fancyLeaves ? 0 : 1);
}
