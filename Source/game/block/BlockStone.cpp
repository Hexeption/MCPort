//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockStone.h"

BlockStone::BlockStone(int_t blockID, int_t blockIndexInTexture) : Block(blockID, blockIndexInTexture, Material::rock) {
}

int_t BlockStone::idDropped(int_t var1, Random random) {
    return Block::cobblestone->blockID;
}
