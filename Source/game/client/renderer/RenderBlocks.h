//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERBLOCKS_H
#define MCPORT_RENDERBLOCKS_H

#include "java/Type.h"

#include "game/block/Block.h"
#include "game/world/IBlockAccess.h"
#include "game/block/Material.h"

class RenderBlocks {
private:
    IBlockAccess *blockAccess = nullptr;
    int_t overrideBlockTexture = -1;

    float getFluidHeight(int_t x, int_t y, int_t z, const Material *material) const;

public:
    RenderBlocks() = default;

    explicit RenderBlocks(IBlockAccess &blockAccess);

    void renderBlockUsingTexture(Block *block, int_t x, int_t y, int_t z, int_t texture);

    bool renderBlockByRenderType(Block *block, int_t x, int_t y, int_t z);

    bool renderStandardBlock(Block *block, int_t x, int_t y, int_t z);

    bool renderStandardBlockWithColorMultiplier(Block *block, int_t x, int_t y, int_t z, float red, float green,
                                                float blue);

    bool renderBlockFluids(Block *block, int_t x, int_t y, int_t z);

    bool renderBlockFallingSand(Block *block, IBlockAccess &blockAccessIn, int_t x, int_t y, int_t z);

    bool renderBlockReed(Block *block, int_t x, int_t y, int_t z);

    void renderBlockAsItem(Block *block, float alpha);

    void renderCrossedSquares(Block block, int meta, double x, double y, double z);

    void renderBottomFace(Block *block, double x, double y, double z, int_t texture);

    void renderTopFace(Block *block, double x, double y, double z, int_t texture);

    void renderEastFace(Block *block, double x, double y, double z, int_t texture);

    void renderWestFace(Block *block, double x, double y, double z, int_t texture);

    void renderNorthFace(Block *block, double x, double y, double z, int_t texture);

    void renderSouthFace(Block *block, double x, double y, double z, int_t texture);

    static bool renderItemIn3d(int_t renderType);

    void renderBlockOnInventory(Block *block);
};

#endif //MCPORT_RENDERBLOCKS_H
