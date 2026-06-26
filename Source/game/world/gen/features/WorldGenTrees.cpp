//
// Created by Keir Davis on 26/06/2026.
//

#include "WorldGenTrees.h"

#include "game/block/Block.h"

bool WorldGenTrees::generate(World &world, Random &random, int_t x, int_t y, int_t z) {
    const int_t height = random.nextInt(3) + 4;
    bool canGrow = true;

    if (y < 1 || y + height + 1 > 128) {
        return false;
    }

    for (int_t yy = y; yy <= y + 1 + height; ++yy) {
        int_t radius = 1;

        if (yy == y) {
            radius = 0;
        }

        if (yy >= y + 1 + height - 2) {
            radius = 2;
        }

        for (int_t xx = x - radius; xx <= x + radius && canGrow; ++xx) {
            for (int_t zz = z - radius; zz <= z + radius && canGrow; ++zz) {
                if (yy >= 0 && yy < 128) {
                    const int_t blockId = world.getBlockId(xx, yy, zz);

                    if (blockId != 0 && blockId != Block::leaves->blockID) {
                        canGrow = false;
                    }
                } else {
                    canGrow = false;
                }
            }
        }
    }

    if (!canGrow) {
        return false;
    }

    const int_t blockBelow = world.getBlockId(x, y - 1, z);

    if ((blockBelow != Block::grass->blockID && blockBelow != Block::dirt->blockID) ||
        y >= 128 - height - 1) {
        return false;
    }

    world.setBlock(x, y - 1, z, Block::dirt->blockID);

    for (int_t yy = y - 3 + height; yy <= y + height; ++yy) {
        const int_t topOffset = yy - (y + height);
        const int_t radius = 1 - topOffset / 2;

        for (int_t xx = x - radius; xx <= x + radius; ++xx) {
            const int_t dx = xx - x;

            for (int_t zz = z - radius; zz <= z + radius; ++zz) {
                const int_t dz = zz - z;

                if ((std::abs(dx) != radius ||
                     std::abs(dz) != radius ||
                     random.nextInt(2) != 0 && topOffset != 0) &&
                    !Block::opaqueCubeLookup[world.getBlockId(xx, yy, zz)]) {
                    world.setBlock(xx, yy, zz, Block::leaves->blockID);
                }
            }
        }
    }

    for (int_t yy = 0; yy < height; ++yy) {
        const int_t blockId = world.getBlockId(x, y + yy, z);

        if (blockId == 0 || blockId == Block::leaves->blockID) {
            world.setBlock(x, y + yy, z, Block::wood->blockID);
        }
    }

    return true;
}
