//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockSand.h"

#include <memory>

#include "Material.h"
#include "game/entity/EntityFallingSand.h"
#include "game/world/World.h"

bool BlockSand::fallInstantly = false;

BlockSand::BlockSand(const int_t blockID, const int_t blockIndexInTexture)
    : Block(blockID, blockIndexInTexture, Material::sand) {
}

void BlockSand::onBlockAdded(World &world, const int_t x, const int_t y, const int_t z) {
    world.scheduleBlockUpdate(x, y, z, blockID);
}

void BlockSand::onNeighborBlockChange(World &world, const int_t x, const int_t y, const int_t z,
                                      const int_t neighborBlockId) {
    world.scheduleBlockUpdate(x, y, z, blockID);
}

void BlockSand::updateTick(World &world, const int_t x, const int_t y, const int_t z, Random &random) {
    tryToFall(world, x, y, z);
}

void BlockSand::tryToFall(World &world, const int_t x, const int_t y, const int_t z) {
    if (canFallBelow(world, x, y - 1, z) && y >= 0) {
        auto entity = std::make_unique<EntityFallingSand>(
            world,
            static_cast<float>(x) + 0.5f,
            static_cast<float>(y) + 0.5f,
            static_cast<float>(z) + 0.5f,
            blockID
        );
        if (fallInstantly) {
            while (!entity->isDead) {
                entity->onUpdate();
            }
        } else {
            world.spawnEntityInWorld(std::move(entity));
        }
    }
}

int_t BlockSand::tickRate() const {
    return 3;
}

bool BlockSand::canFallBelow(World &world, const int_t x, const int_t y, const int_t z) {
    const int_t blockId = world.getBlockId(x, y, z);
    if (blockId == 0) {
        return true;
    }
    const Block *block = Block::blocksList[blockId];
    if (block == nullptr) {
        return false;
    }
    if (block->material == Material::fire) {
        return true;
    }
    return block->material == Material::water || block->material == Material::lava;
}
