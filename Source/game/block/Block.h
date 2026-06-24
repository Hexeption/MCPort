//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_BLOCK_H
#define MCPORT_BLOCK_H

#include "java/Type.h"

class Block {
public:
    static Block *stone;

    double minX = 0.0;
    double minY = 0.0;
    double minZ = 0.0;
    double maxX = 1.0;
    double maxY = 1.0;
    double maxZ = 1.0;

    explicit Block(int_t blockIndexInTexture);

    int_t getRenderType() const;

    void setBlockBoundsForItemRender();

    int_t getBlockTextureFromSide(int_t side) const;

private:
    int_t blockIndexInTexture;
};

#endif //MCPORT_BLOCK_H
