//
// Created by Keir Davis on 24/06/2026.
//

#include "RenderBlocks.h"

#include <glad/glad.h>

#include "Tessellator.h"
#include "game/client/MathHelper.h"
#include "game/block/Block.h"
#include "game/block/BlockFluid.h"
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
    const int_t renderType = block->getRenderType();
    if (renderType == 0) {
        return renderStandardBlock(block, x, y, z);
    }
    if (renderType == 4) {
        return renderBlockFluids(block, x, y, z);
    }
    return false;
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
        renderBottomFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0
                                                   ? overrideBlockTexture
                                                   : block->getBlockTextureFromSide(0));
        var4.setColorRGBA_F(var5, var5, var5, alpha);
        renderTopFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0
                                                ? overrideBlockTexture
                                                : block->getBlockTextureFromSide(1));
        var4.setColorRGBA_F(var7, var7, var7, alpha);
        renderEastFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0
                                                 ? overrideBlockTexture
                                                 : block->getBlockTextureFromSide(2));
        renderWestFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0
                                                 ? overrideBlockTexture
                                                 : block->getBlockTextureFromSide(3));
        var4.setColorRGBA_F(var8, var8, var8, alpha);
        renderNorthFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0
                                                  ? overrideBlockTexture
                                                  : block->getBlockTextureFromSide(4));
        renderSouthFace(block, 0.0, 0.0, 0.0, overrideBlockTexture >= 0
                                                  ? overrideBlockTexture
                                                  : block->getBlockTextureFromSide(5));
        var4.draw();
        glTranslatef(0.5f, 0.5f, 0.5f);
    }
}

bool RenderBlocks::renderBlockFluids(Block *block, const int_t x, const int_t y, const int_t z) {
    if (blockAccess == nullptr || block == nullptr) {
        return false;
    }

    Tessellator &tessellator = Tessellator::instance;
    const bool renderTop = block->shouldSideBeRendered(*blockAccess, x, y + 1, z, 1);
    const bool renderBottom = block->shouldSideBeRendered(*blockAccess, x, y - 1, z, 0);
    const bool renderNorth = block->shouldSideBeRendered(*blockAccess, x, y, z - 1, 2);
    const bool renderSouth = block->shouldSideBeRendered(*blockAccess, x, y, z + 1, 3);
    const bool renderWest = block->shouldSideBeRendered(*blockAccess, x - 1, y, z, 4);
    const bool renderEast = block->shouldSideBeRendered(*blockAccess, x + 1, y, z, 5);
    if (!renderTop && !renderBottom && !renderNorth && !renderSouth && !renderWest && !renderEast) {
        return false;
    }

    const Material *material = block->material;
    const int_t metadata = blockAccess->getBlockMetadata(x, y, z);
    const float height = getFluidHeight(x, y, z, material);
    const float heightNorth = getFluidHeight(x, y, z + 1, material);
    const float heightNorthEast = getFluidHeight(x + 1, y, z + 1, material);
    const float heightEast = getFluidHeight(x + 1, y, z, material);

    bool renderedAny = false;
    constexpr float topBrightness = 1.0f;
    constexpr float bottomBrightness = 0.5f;
    constexpr float sideBrightness = 0.8f;
    constexpr float sideBrightness2 = 0.6f;

    if (renderTop) {
        renderedAny = true;
        int_t texture = block->getBlockTextureFromSideAndMetadata(1, metadata);
        const float flowDirection = static_cast<float>(BlockFluid::getFlowDirection(
            *blockAccess, x, y, z, block->material));
        if (flowDirection > -999.0f) {
            texture = block->getBlockTextureFromSideAndMetadata(2, metadata);
        }

        const int_t textureX = (texture & 15) << 4;
        const int_t textureY = texture & 240;
        double centerU = (static_cast<double>(textureX) + 8.0) / 256.0;
        double centerV = (static_cast<double>(textureY) + 8.0) / 256.0;
        if (flowDirection >= -999.0f) {
            centerU = static_cast<double>(static_cast<float>(textureX + 16) / 256.0f);
            centerV = static_cast<double>(static_cast<float>(textureY + 16) / 256.0f);
        }
        const float brightness = block->getBlockBrightness(*blockAccess, x, y, z);
        tessellator.setColorOpaque_F(topBrightness * brightness, topBrightness * brightness,
                                     topBrightness * brightness);
        double uOffset = 0.0;
        double vOffset = 0.0;
        const float topFlowDirection = flowDirection < -999.0f ? 0.0f : flowDirection;
        uOffset = static_cast<double>(MathHelper::sin(topFlowDirection) * 8.0f / 256.0f);
        vOffset = static_cast<double>(MathHelper::cos(topFlowDirection) * 8.0f / 256.0f);

        tessellator.addVertexWithUV(static_cast<double>(x + 0), static_cast<double>(y) + height,
                                    static_cast<double>(z + 0), centerU - vOffset - uOffset,
                                    centerV - vOffset + uOffset);
        tessellator.addVertexWithUV(static_cast<double>(x + 0), static_cast<double>(y) + heightNorth,
                                    static_cast<double>(z + 1), centerU - vOffset + uOffset,
                                    centerV + vOffset + uOffset);
        tessellator.addVertexWithUV(static_cast<double>(x + 1), static_cast<double>(y) + heightNorthEast,
                                    static_cast<double>(z + 1), centerU + vOffset + uOffset,
                                    centerV + vOffset - uOffset);
        tessellator.addVertexWithUV(static_cast<double>(x + 1), static_cast<double>(y) + heightEast,
                                    static_cast<double>(z + 0), centerU + vOffset - uOffset,
                                    centerV - vOffset - uOffset);
    }

    if (renderBottom) {
        renderedAny = true;
        const float brightness = block->getBlockBrightness(*blockAccess, x, y - 1, z);
        tessellator.setColorOpaque_F(bottomBrightness * brightness, bottomBrightness * brightness,
                                     bottomBrightness * brightness);
        renderBottomFace(block, static_cast<double>(x), static_cast<double>(y), static_cast<double>(z),
                         block->getBlockTextureFromSide(0));
    }

    for (int_t side = 0; side < 4; ++side) {
        const bool renderSide = side == 0 ? renderNorth : side == 1 ? renderSouth : side == 2 ? renderWest : renderEast;
        if (!renderSide) {
            continue;
        }

        renderedAny = true;
        int_t brightnessX = x;
        int_t brightnessZ = z;
        float firstHeight;
        float secondHeight;
        float firstX;
        float secondX;
        float firstZ;
        float secondZ;
        if (side == 0) {
            brightnessZ = z - 1;
            firstHeight = height;
            secondHeight = heightEast;
            firstX = static_cast<float>(x);
            secondX = static_cast<float>(x + 1);
            firstZ = static_cast<float>(z);
            secondZ = static_cast<float>(z);
        } else if (side == 1) {
            brightnessZ = z + 1;
            firstHeight = heightNorthEast;
            secondHeight = heightNorth;
            firstX = static_cast<float>(x + 1);
            secondX = static_cast<float>(x);
            firstZ = static_cast<float>(z + 1);
            secondZ = static_cast<float>(z + 1);
        } else if (side == 2) {
            brightnessX = x - 1;
            firstHeight = heightNorth;
            secondHeight = height;
            firstX = static_cast<float>(x);
            secondX = static_cast<float>(x);
            firstZ = static_cast<float>(z + 1);
            secondZ = static_cast<float>(z);
        } else {
            brightnessX = x + 1;
            firstHeight = heightEast;
            secondHeight = heightNorthEast;
            firstX = static_cast<float>(x + 1);
            secondX = static_cast<float>(x + 1);
            firstZ = static_cast<float>(z);
            secondZ = static_cast<float>(z + 1);
        }

        const int_t texture = block->getBlockTextureFromSideAndMetadata(side + 2, metadata);
        const int_t textureX = (texture & 15) << 4;
        const int_t textureY = texture & 240;
        const double u0 = static_cast<double>(static_cast<float>(textureX) / 256.0f);
        const double u1 = (static_cast<double>(textureX + 16) - 0.01) / 256.0;
        const double v0 = static_cast<double>((static_cast<float>(textureY) + (1.0f - firstHeight) * 16.0f) / 256.0f);
        const double v1 = static_cast<double>((static_cast<float>(textureY) + (1.0f - secondHeight) * 16.0f) / 256.0f);
        const double vBottom = (static_cast<double>(textureY + 16) - 0.01) / 256.0;
        const float brightness = block->getBlockBrightness(*blockAccess, brightnessX, y, brightnessZ);
        const float shade = side < 2 ? sideBrightness : sideBrightness2;
        tessellator.setColorOpaque_F(shade * brightness, shade * brightness, shade * brightness);
        tessellator.addVertexWithUV(static_cast<double>(firstX), static_cast<double>(y) + firstHeight,
                                    static_cast<double>(firstZ), u0, v0);
        tessellator.addVertexWithUV(static_cast<double>(secondX), static_cast<double>(y) + secondHeight,
                                    static_cast<double>(secondZ), u1, v1);
        tessellator.addVertexWithUV(static_cast<double>(secondX), static_cast<double>(y), static_cast<double>(secondZ),
                                    u1, vBottom);
        tessellator.addVertexWithUV(static_cast<double>(firstX), static_cast<double>(y), static_cast<double>(firstZ),
                                    u0, vBottom);
    }

    return renderedAny;
}

float RenderBlocks::getFluidHeight(const int_t x, const int_t y, const int_t z, const Material *material) const {
    int_t weight = 0;
    float total = 0.0f;

    for (int_t corner = 0; corner < 4; ++corner) {
        const int_t sampleX = x - (corner & 1);
        const int_t sampleZ = z - ((corner >> 1) & 1);
        if (blockAccess->getBlockMaterial(sampleX, y + 1, sampleZ) == material) {
            return 1.0f;
        }

        Material *sampleMaterial = blockAccess->getBlockMaterial(sampleX, y, sampleZ);
        if (sampleMaterial != material) {
            if (!sampleMaterial->getIsSolid()) {
                ++weight;
                ++total;
            }
            continue;
        }

        const int_t metadata = blockAccess->getBlockMetadata(sampleX, y, sampleZ);
        if (metadata >= 8 || metadata == 0) {
            total += BlockFluid::getFluidHeightPercent(metadata) * 10.0f;
            weight += 10;
        }
        total += BlockFluid::getFluidHeightPercent(metadata);
        ++weight;
    }

    if (weight == 0) {
        return 1.0f;
    }

    return 1.0f - total / static_cast<float>(weight);
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

bool RenderBlocks::renderItemIn3d(const int_t renderType) {
    return renderType == 0;
}

void RenderBlocks::renderBlockOnInventory(Block *block) {
    if (block == nullptr) {
        return;
    }

    renderBlockAsItem(block, 1.0f);
}
