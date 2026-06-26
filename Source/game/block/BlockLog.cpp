//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockLog.h"

BlockLog::BlockLog(int_t blockID) : Block(blockID, Material::wood) {
    blockIndexInTexture = 20;
}

int_t BlockLog::getBlockTextureFromSide(int_t side) const {
    return side == 1 ? 21 : (side == 0 ? 21 : 20);
}

int_t BlockLog::quantityDropped(Random random) {
    return 1;
}

int_t BlockLog::idDropped(int_t var1, Random random) {
    return Block::wood->blockID;
}
