//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockGravel.h"

BlockGravel::BlockGravel(const int_t blockID, const int_t blockIndexInTexture)
    : BlockSand(blockID, blockIndexInTexture) {
}

int_t BlockGravel::idDropped(const int_t metadata, Random random) {
    return blockID;
}
