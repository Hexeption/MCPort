//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERBLOCKS_H
#define MCPORT_RENDERBLOCKS_H

#include "java/Type.h"

class Block;

class RenderBlocks {
public:
    void renderBlockAsItem(Block *block, float alpha);

    void renderBottomFace(Block *block, double x, double y, double z, int_t texture);

    void renderTopFace(Block *block, double x, double y, double z, int_t texture);

    void renderEastFace(Block *block, double x, double y, double z, int_t texture);

    void renderWestFace(Block *block, double x, double y, double z, int_t texture);

    void renderNorthFace(Block *block, double x, double y, double z, int_t texture);

    void renderSouthFace(Block *block, double x, double y, double z, int_t texture);
};

#endif //MCPORT_RENDERBLOCKS_H
