//
// Created by Keir Davis on 24/06/2026.
//

#include "Block.h"

Block *Block::stone = new Block(1);

Block::Block(const int_t blockIndexInTexture) : blockIndexInTexture(blockIndexInTexture) {
}

int_t Block::getRenderType() const {
    return 0;
}

void Block::setBlockBoundsForItemRender() {
    minX = 0.0;
    minY = 0.0;
    minZ = 0.0;
    maxX = 1.0;
    maxY = 1.0;
    maxZ = 1.0;
}

int_t Block::getBlockTextureFromSide(int_t) const {
    return blockIndexInTexture;
}
