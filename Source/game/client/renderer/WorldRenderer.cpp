//
// Created by Keir Davis on 25/06/2026.
//

#include "WorldRenderer.h"

#include <algorithm>

#include <glad/glad.h>

#include "RenderBlocks.h"
#include "Tessellator.h"
#include "game/block/Block.h"
#include "game/world/World.h"
#include "game/world/chunk/ChunkCache.h"

WorldRenderer::WorldRenderer(World *world, const int_t x, const int_t y, const int_t z, const int_t size,
                             const int_t glRenderList)
    : worldObj(world), sizeWidth(size), sizeHeight(size), sizeDepth(size), glRenderList(glRenderList) {
    setPosition(x, y, z);
}

WorldRenderer::~WorldRenderer() {
    releaseList();
}

WorldRenderer::WorldRenderer(WorldRenderer &&other) noexcept
    : worldObj(other.worldObj),
      posX(other.posX),
      posY(other.posY),
      posZ(other.posZ),
      sizeWidth(other.sizeWidth),
      sizeHeight(other.sizeHeight),
      sizeDepth(other.sizeDepth),
      needsUpdate(other.needsUpdate),
      skipRenderPass(other.skipRenderPass),
      glRenderList(other.glRenderList) {
    other.glRenderList = 0;
}

WorldRenderer &WorldRenderer::operator=(WorldRenderer &&other) noexcept {
    if (this != &other) {
        releaseList();
        worldObj = other.worldObj;
        posX = other.posX;
        posY = other.posY;
        posZ = other.posZ;
        sizeWidth = other.sizeWidth;
        sizeHeight = other.sizeHeight;
        sizeDepth = other.sizeDepth;
        needsUpdate = other.needsUpdate;
        skipRenderPass = other.skipRenderPass;
        glRenderList = other.glRenderList;
        other.glRenderList = 0;
    }
    return *this;
}

void WorldRenderer::setPosition(const int_t x, const int_t y, const int_t z) {
    if (x == posX && y == posY && z == posZ && glRenderList != 0) {
        return;
    }

    posX = x;
    posY = y;
    posZ = z;
    markDirty();
}

void WorldRenderer::updateRenderer(RenderBlocks &renderBlocks) {
    if (!needsUpdate || worldObj == nullptr || glRenderList == 0) {
        return;
    }

    needsUpdate = false;
    skipRenderPass = true;
    glNewList(static_cast<GLuint>(glRenderList), GL_COMPILE);
    Tessellator &tessellator = Tessellator::instance;
    tessellator.startDrawingQuads();

    bool renderedAny = false;
    const int_t maxX = posX + sizeWidth;
    const int_t maxY = std::min<int_t>(128, posY + sizeHeight);
    const int_t maxZ = posZ + sizeDepth;
    ChunkCache chunkCache(*worldObj, posX - 1, posY - 1, posZ - 1, maxX + 1, maxY + 1, maxZ + 1);
    RenderBlocks chunkRenderBlocks(chunkCache);
    for (int_t y = std::max<int_t>(0, posY); y < maxY; ++y) {
        for (int_t z = posZ; z < maxZ; ++z) {
            for (int_t x = posX; x < maxX; ++x) {
                const int_t blockId = worldObj->getBlockId(x, y, z);
                Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                                   ? Block::blocksList[blockId]
                                   : nullptr;
                if (block == nullptr) {
                    continue;
                }

                renderedAny = chunkRenderBlocks.renderBlockByRenderType(block, x, y, z) || renderedAny;
            }
        }
    }

    tessellator.draw();
    glEndList();
    skipRenderPass = !renderedAny;
}

void WorldRenderer::markDirty() {
    needsUpdate = true;
}

void WorldRenderer::stopRendering() {
    worldObj = nullptr;
    skipRenderPass = true;
    needsUpdate = false;
}

void WorldRenderer::render() const {
    if (!skipRenderPass && glRenderList != 0) {
        glCallList(static_cast<GLuint>(glRenderList));
    }
}

void WorldRenderer::releaseList() {
    if (glRenderList != 0) {
        glDeleteLists(static_cast<GLuint>(glRenderList), 1);
        glRenderList = 0;
    }
}
