//
// Created by Keir Davis on 25/06/2026.
//

#include "BlockStationary.h"

#include "game/world/World.h"

BlockStationary::BlockStationary(const int_t blockID, Material *material)
    : BlockFluid(blockID, material) {
    setTickOnLoad(false);
    if (material == Material::lava) {
        setTickOnLoad(true);
    }
}

void BlockStationary::setNotStationary(World &world, const int_t x, const int_t y, const int_t z) const {
    const int_t metadata = world.getBlockMetadata(x, y, z);
    world.editingBlocks = true;
    world.setBlockAndMetadata(x, y, z, blockID - 1, metadata);
    world.markBlocksDirty(x, y, z, x, y, z);
    world.scheduleBlockUpdate(x, y, z, blockID - 1);
    world.editingBlocks = false;
}

void BlockStationary::onNeighborBlockChange(World &world, const int_t x, const int_t y, const int_t z,
                                            const int_t neighborBlockId) {
    BlockFluid::onNeighborBlockChange(world, x, y, z, neighborBlockId);
    const int_t blockId = world.getBlockId(x, y, z);
    if (blockId != blockID) {
        return;
    }

    setNotStationary(world, x, y, z);
}

void BlockStationary::updateTick(World &, const int_t, const int_t, const int_t, Random &) {
}
