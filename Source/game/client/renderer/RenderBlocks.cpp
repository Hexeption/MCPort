//
// Created by Keir Davis on 24/06/2026.
//

#include "RenderBlocks.h"

#include <glad/glad.h>

#include "Tessellator.h"
#include "game/block/Block.h"

static void textureUV(const int_t texture, double &u0, double &u1, double &v0, double &v1) {
    const int_t var5 = (texture & 15) << 4;
    const int_t var6 = texture & 240;
    u0 = static_cast<double>(static_cast<float>(var5) / 256.0f);
    u1 = static_cast<double>((static_cast<float>(var5) + 15.99f) / 256.0f);
    v0 = static_cast<double>(static_cast<float>(var6) / 256.0f);
    v1 = static_cast<double>((static_cast<float>(var6) + 15.99f) / 256.0f);
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
        renderBottomFace(block, 0.0, 0.0, 0.0, block->getBlockTextureFromSide(0));
        var4.setColorRGBA_F(var5, var5, var5, alpha);
        renderTopFace(block, 0.0, 0.0, 0.0, block->getBlockTextureFromSide(1));
        var4.setColorRGBA_F(var7, var7, var7, alpha);
        renderEastFace(block, 0.0, 0.0, 0.0, block->getBlockTextureFromSide(2));
        renderWestFace(block, 0.0, 0.0, 0.0, block->getBlockTextureFromSide(3));
        var4.setColorRGBA_F(var8, var8, var8, alpha);
        renderNorthFace(block, 0.0, 0.0, 0.0, block->getBlockTextureFromSide(4));
        renderSouthFace(block, 0.0, 0.0, 0.0, block->getBlockTextureFromSide(5));
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
    const double minY = y + block->minY;
    const double maxY = y + block->maxY;
    const double minZ = z + block->minZ;
    const double maxZ = z + block->maxZ;
    var9.addVertexWithUV(minX, maxY, minZ, u1, v0);
    var9.addVertexWithUV(minX, minY, minZ, u1, v1);
    var9.addVertexWithUV(minX, minY, maxZ, u0, v1);
    var9.addVertexWithUV(minX, maxY, maxZ, u0, v0);
}

void RenderBlocks::renderWestFace(Block *block, const double x, const double y, const double z, const int_t texture) {
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
    var9.addVertexWithUV(maxX, maxY, maxZ, u1, v0);
    var9.addVertexWithUV(maxX, minY, maxZ, u1, v1);
    var9.addVertexWithUV(maxX, minY, minZ, u0, v1);
    var9.addVertexWithUV(maxX, maxY, minZ, u0, v0);
}

void RenderBlocks::renderNorthFace(Block *block, const double x, const double y, const double z, const int_t texture) {
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
    var9.addVertexWithUV(maxX, maxY, minZ, u1, v0);
    var9.addVertexWithUV(maxX, minY, minZ, u1, v1);
    var9.addVertexWithUV(minX, minY, minZ, u0, v1);
    var9.addVertexWithUV(minX, maxY, minZ, u0, v0);
}

void RenderBlocks::renderSouthFace(Block *block, const double x, const double y, const double z, const int_t texture) {
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
    var9.addVertexWithUV(minX, maxY, maxZ, u1, v0);
    var9.addVertexWithUV(minX, minY, maxZ, u1, v1);
    var9.addVertexWithUV(maxX, minY, maxZ, u0, v1);
    var9.addVertexWithUV(maxX, maxY, maxZ, u0, v0);
}
