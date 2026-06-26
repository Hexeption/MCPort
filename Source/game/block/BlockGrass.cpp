//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockGrass.h"

#include "game/world/IBlockAccess.h"
#include "game/world/World.h"

BlockGrass::BlockGrass(int_t blockID) : Block(blockID, 3, Material::grass) {
    setTickOnLoad(true);
}

int_t BlockGrass::getBlockTextureFromSide(const int_t side) const {
    if (side == 1) return 0;
    if (side == 0) return 2;
    return 3;
}

int_t BlockGrass::getBlockTexture(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const {
    if (side == 1) {
        return 0;
    } else if (side == 0) {
        return 2;
    } else {
        Material *material = blockAccess.getBlockMaterial(x, y + 1, z);
        return material != Material::snow && material != Material::craftedSnow ? 3 : 68;
    }
}

void BlockGrass::updateTick(World &world, int_t x, int_t y, int_t z, Random &random) {
    if (world.getBlockLightValue(x, y + 1, z) < 4 && world.getBlockMaterial(x, y + 1, z)->getCanBlockGrass()) {
        if (random.nextInt(4) != 0) {
            return;
        }

        world.setBlockWithNotify(x, y, z, Block::dirt->blockID);
    } else if (world.getBlockLightValue(x, y + 1, z) >= 9) {
        int_t x1 = x + random.nextInt(3) - 1;
        int_t z1 = z + random.nextInt(3) - 1;
        int_t y1 = y + random.nextInt(2) - 1;
        if (world.getBlockId(x1, y1, z1) == Block::dirt->blockID || world.getBlockLightValue(x1, y1 + 1, z1) >= 4 && !
            world.getBlockMaterial(x1, y1 + 1, z1)->getCanBlockGrass()) {
            world.setBlockWithNotify(x, y, z, Block::grass->blockID);
        }
    }
}

int_t BlockGrass::idDropped(int_t var1, Random random) {
    return Block::dirt->idDropped(0, random);
}
