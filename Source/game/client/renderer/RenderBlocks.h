//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERBLOCKS_H
#define MCPORT_RENDERBLOCKS_H

#include "java/Type.h"

class Block;
class IBlockAccess;

class RenderBlocks {
private:
    IBlockAccess *blockAccess = nullptr;

public:
    RenderBlocks() = default;

    explicit RenderBlocks(IBlockAccess &blockAccess);

    bool renderBlockByRenderType(Block *block, int_t x, int_t y, int_t z);

    bool renderStandardBlock(Block *block, int_t x, int_t y, int_t z);

    bool renderStandardBlockWithColorMultiplier(Block *block, int_t x, int_t y, int_t z, float red, float green,
                                                float blue);

    void renderBlockAsItem(Block *block, float alpha);

    void renderBottomFace(Block *block, double x, double y, double z, int_t texture);

    void renderTopFace(Block *block, double x, double y, double z, int_t texture);

    void renderEastFace(Block *block, double x, double y, double z, int_t texture);

    void renderWestFace(Block *block, double x, double y, double z, int_t texture);

    void renderNorthFace(Block *block, double x, double y, double z, int_t texture);

    void renderSouthFace(Block *block, double x, double y, double z, int_t texture);
};

#endif //MCPORT_RENDERBLOCKS_H
