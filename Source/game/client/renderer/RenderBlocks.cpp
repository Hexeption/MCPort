//
// Created by Keir Davis on 24/06/2026.
//

#include "RenderBlocks.h"

#include <glad/glad.h>

#include "Tessellator.h"
#include "game/block/Block.h"
#include "game/world/IBlockAccess.h"

static void textureUV(const int_t texture, double &u0, double &u1, double &v0, double &v1) {
    const int_t var5 = (texture & 15) << 4;
    const int_t var6 = texture & 240;
    u0 = static_cast<double>(static_cast<float>(var5) / 256.0f);
    u1 = static_cast<double>((static_cast<float>(var5) + 15.99f) / 256.0f);
    v0 = static_cast<double>(static_cast<float>(var6) / 256.0f);
    v1 = static_cast<double>((static_cast<float>(var6) + 15.99f) / 256.0f);
}

RenderBlocks::RenderBlocks(IBlockAccess &blockAccess) : blockAccess(&blockAccess) {
}

void RenderBlocks::renderBlockUsingTexture(Block *block, const int_t x, const int_t y, const int_t z,
                                           const int_t texture) {
    if (block == nullptr) {
        return;
    }

    overrideBlockTexture = texture;
    renderBlockByRenderType(block, x, y, z);
    overrideBlockTexture = -1;
}

bool RenderBlocks::renderBlockByRenderType(Block *block, const int_t x, const int_t y, const int_t z) {
    if (block == nullptr) {
        return false;
    }
    if (blockAccess != nullptr) {
        block->setBlockBoundsBasedOnState(*blockAccess, x, y, z);
    }
    return block->getRenderType() == 0 && renderStandardBlock(block, x, y, z);
}

bool RenderBlocks::renderStandardBlock(Block *block, const int_t x, const int_t y, const int_t z) {
    return renderStandardBlockWithColorMultiplier(block, x, y, z, 1.0f, 1.0f, 1.0f);
}

bool RenderBlocks::renderStandardBlockWithColorMultiplier(Block *block, const int_t x, const int_t y, const int_t z,
                                                          const float red, const float green, const float blue) {
    if (blockAccess == nullptr || block == nullptr) {
        return false;
    }

    Tessellator &tessellator = Tessellator::instance;
    bool renderedAny = false;
    const auto textureForSide = [this, block](const int_t side) {
        return overrideBlockTexture >= 0 ? overrideBlockTexture : block->getBlockTextureFromSide(side);
    };

    if (block->shouldSideBeRendered(*blockAccess, x, y - 1, z, 0)) {
        const float brightness = block->getBlockBrightness(*blockAccess, x, y - 1, z);
        tessellator.setColorOpaque_F(0.5f * brightness * red, 0.5f * brightness * green,
                                     0.5f * brightness * blue);
        renderBottomFace(block, x, y, z, textureForSide(0));
        renderedAny = true;
    }
    if (block->shouldSideBeRendered(*blockAccess, x, y + 1, z, 1)) {
        const float brightness = block->getBlockBrightness(*blockAccess, x, y + 1, z);
        tessellator.setColorOpaque_F(brightness * red, brightness * green, brightness * blue);
        renderTopFace(block, x, y, z, textureForSide(1));
        renderedAny = true;
    }
    if (block->shouldSideBeRendered(*blockAccess, x, y, z - 1, 2)) {
        const float brightness = block->getBlockBrightness(*blockAccess, x, y, z - 1);
        tessellator.setColorOpaque_F(0.6f * brightness * red, 0.6f * brightness * green,
                                     0.6f * brightness * blue);
        renderEastFace(block, x, y, z, textureForSide(2));
        renderedAny = true;
    }
    if (block->shouldSideBeRendered(*blockAccess, x, y, z + 1, 3)) {
        const float brightness = block->getBlockBrightness(*blockAccess, x, y, z + 1);
        tessellator.setColorOpaque_F(0.6f * brightness * red, 0.6f * brightness * green,
                                     0.6f * brightness * blue);
        renderWestFace(block, x, y, z, textureForSide(3));
        renderedAny = true;
    }
    if (block->shouldSideBeRendered(*blockAccess, x - 1, y, z, 4)) {
        const float brightness = block->getBlockBrightness(*blockAccess, x - 1, y, z);
        tessellator.setColorOpaque_F(0.8f * brightness * red, 0.8f * brightness * green,
                                     0.8f * brightness * blue);
        renderNorthFace(block, x, y, z, textureForSide(4));
        renderedAny = true;
    }
    if (block->shouldSideBeRendered(*blockAccess, x + 1, y, z, 5)) {
        const float brightness = block->getBlockBrightness(*blockAccess, x + 1, y, z);
        tessellator.setColorOpaque_F(0.8f * brightness * red, 0.8f * brightness * green,
                                     0.8f * brightness * blue);
        renderSouthFace(block, x, y, z, textureForSide(5));
        renderedAny = true;
    }

    return renderedAny;
}

void RenderBlocks::renderBlockAsItem(Block *block, const float alpha) {
    const int_t var3 = block->getRenderType();
    Tessellator &var4 = Tessellator::instance;
    if (var3 == 0) {
        block->setBlockBoundsForItemRender();
        glTranslatef(-0.5f, -0.5f, -0.5f);
        const float var5 = 0.5f;
        const float var6 = 1.0f;
        const float var7 = 0.8f;
        const float var8 = 0.6f;
        var4.startDrawingQuads();
        var4.setColorRGBA_F(var6, var6, var6, alpha);
        renderBottomFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0 ? overrideBlockTexture
                                                                        : block->getBlockTextureFromSide(0));
        var4.setColorRGBA_F(var5, var5, var5, alpha);
        renderTopFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0 ? overrideBlockTexture
                                                                      : block->getBlockTextureFromSide(1));
        var4.setColorRGBA_F(var7, var7, var7, alpha);
        renderEastFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0 ? overrideBlockTexture
                                                                       : block->getBlockTextureFromSide(2));
        renderWestFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0 ? overrideBlockTexture
                                                                       : block->getBlockTextureFromSide(3));
        var4.setColorRGBA_F(var8, var8, var8, alpha);
        renderNorthFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0 ? overrideBlockTexture
                                                                        : block->getBlockTextureFromSide(4));
        renderSouthFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0 ? overrideBlockTexture
                                                                        : block->getBlockTextureFromSide(5));
        var4.draw();
        glTranslatef(0.5f, 0.5f, 0.5f);
    }
}

void RenderBlocks::renderBottomFace(Block *block, const double x, const double y, const double z, const int_t texture) {
    double u0;
    double u1;
    double v0;
    double v1;
    textureUV(texture, u0, u1, v0, v1);
    Tessellator &var9 = Tessellator::instance;
    const double minX = x + block->minX;
    const double maxX = x + block->maxX;
    const double minY = y + block->minY;
    const double minZ = z + block->minZ;
    const double maxZ = z + block->maxZ;
    var9.addVertexWithUV(minX, minY, maxZ, u0, v1);
    var9.addVertexWithUV(minX, minY, minZ, u0, v0);
    var9.addVertexWithUV(maxX, minY, minZ, u1, v0);
    var9.addVertexWithUV(maxX, minY, maxZ, u1, v1);
}

void RenderBlocks::renderTopFace(Block *block, const double x, const double y, const double z, const int_t texture) {
    double u0;
    double u1;
    double v0;
    double v1;
    textureUV(texture, u0, u1, v0, v1);
    Tessellator &var9 = Tessellator::instance;
    const double minX = x + block->minX;
    const double maxX = x + block->maxX;
    const double maxY = y + block->maxY;
    const double minZ = z + block->minZ;
    const double maxZ = z + block->maxZ;
    var9.addVertexWithUV(maxX, maxY, maxZ, u1, v1);
    var9.addVertexWithUV(maxX, maxY, minZ, u1, v0);
    var9.addVertexWithUV(minX, maxY, minZ, u0, v0);
    var9.addVertexWithUV(minX, maxY, maxZ, u0, v1);
}

void RenderBlocks::renderEastFace(Block *block, const double x, const double y, const double z, const int_t texture) {
    double u0;
    double u1;
    double v0;
    double v1;
    textureUV(texture, u0, u1, v0, v1);
    Tessellator &var9 = Tessellator::instance;
    const double minX = x + block->minX;
    const double maxX = x + block->maxX;
    const double minY = y + block->minY;
    const double maxY = y + block->maxY;
    const double minZ = z + block->minZ;
    var9.addVertexWithUV(minX, maxY, minZ, u1, v0);
    var9.addVertexWithUV(maxX, maxY, minZ, u0, v0);
    var9.addVertexWithUV(maxX, minY, minZ, u0, v1);
    var9.addVertexWithUV(minX, minY, minZ, u1, v1);
}

void RenderBlocks::renderWestFace(Block *block, const double x, const double y, const double z, const int_t texture) {
    double u0;
    double u1;
    double v0;
    double v1;
    textureUV(texture, u0, u1, v0, v1);
    Tessellator &var9 = Tessellator::instance;
    const double minX = x + block->minX;
    const double maxX = x + block->maxX;
    const double minY = y + block->minY;
    const double maxY = y + block->maxY;
    const double maxZ = z + block->maxZ;
    var9.addVertexWithUV(minX, maxY, maxZ, u0, v0);
    var9.addVertexWithUV(minX, minY, maxZ, u0, v1);
    var9.addVertexWithUV(maxX, minY, maxZ, u1, v1);
    var9.addVertexWithUV(maxX, maxY, maxZ, u1, v0);
}

void RenderBlocks::renderNorthFace(Block *block, const double x, const double y, const double z, const int_t texture) {
    double u0;
    double u1;
    double v0;
    double v1;
    textureUV(texture, u0, u1, v0, v1);
    Tessellator &var9 = Tessellator::instance;
    const double minX = x + block->minX;
    const double minY = y + block->minY;
    const double maxY = y + block->maxY;
    const double minZ = z + block->minZ;
    const double maxZ = z + block->maxZ;
    var9.addVertexWithUV(minX, maxY, maxZ, u1, v0);
    var9.addVertexWithUV(minX, maxY, minZ, u0, v0);
    var9.addVertexWithUV(minX, minY, minZ, u0, v1);
    var9.addVertexWithUV(minX, minY, maxZ, u1, v1);
}

void RenderBlocks::renderSouthFace(Block *block, const double x, const double y, const double z, const int_t texture) {
    double u0;
    double u1;
    double v0;
    double v1;
    textureUV(texture, u0, u1, v0, v1);
    Tessellator &var9 = Tessellator::instance;
    const double maxX = x + block->maxX;
    const double minY = y + block->minY;
    const double maxY = y + block->maxY;
    const double minZ = z + block->minZ;
    const double maxZ = z + block->maxZ;
    var9.addVertexWithUV(maxX, minY, maxZ, u0, v1);
    var9.addVertexWithUV(maxX, minY, minZ, u1, v1);
    var9.addVertexWithUV(maxX, maxY, minZ, u1, v0);
    var9.addVertexWithUV(maxX, maxY, maxZ, u0, v0);
}
