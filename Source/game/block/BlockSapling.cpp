//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockSapling.h"

#include "game/world/World.h"
#include "game/world/gen/features/WorldGenTrees.h"

BlockSapling::BlockSapling(int_t blockID, int_t blockIndexInTexture) : BlockFlower(blockID, blockIndexInTexture) {
    float var3 = 0.4f;
    setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 2.0F, 0.5F + var3);
}

void BlockSapling::updateTick(World &world, int_t x, int_t y, int_t z, Random &random) {
    BlockFlower::updateTick(world, x, y, z, random);

    if (world.getBlockLightValue(x, y, z) >= 9 && random.nextInt(5) == 0) {
        int blockMeta = world.getBlockMetadata(x, y, z);
        if (blockMeta < 15) {
            world.setBlockAndMetadataWithNotify(x, y, z, blockID, blockMeta + 1);
        } else {
            world.setBlock(x, y, z, 0);

            // todo: add big tree
            auto tree = WorldGenTrees();
            if (!tree.generate(world, random, x, y, z)) {
                world.setBlock(x, y, z, blockID);
            }
        }
    }
}
