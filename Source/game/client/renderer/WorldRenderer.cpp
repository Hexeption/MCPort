//
// Created by Keir Davis on 25/06/2026.
//

#include "WorldRenderer.h"

#include <algorithm>

#include <glad/glad.h>

#include "RenderBlocks.h"
#include "RenderItem.h"
#include "Tessellator.h"
#include "game/block/Block.h"
#include "game/client/MathHelper.h"
#include "game/entity/Entity.h"
#include "game/phys/ICamera.h"
#include "game/world/World.h"
#include "game/world/chunk/Chunk.h"
#include "game/world/chunk/ChunkCache.h"

int_t WorldRenderer::chunksUpdated = 0;

WorldRenderer::WorldRenderer(World *world, const int_t x, const int_t y, const int_t z, const int_t size,
                             const int_t glRenderList)
    : worldObj(world), sizeWidth(size), sizeHeight(size), sizeDepth(size), glRenderList(glRenderList) {
    rendererRadius = MathHelper::sqrt_float(static_cast<float>(sizeWidth * sizeWidth + sizeHeight * sizeHeight +
                                                               sizeDepth * sizeDepth)) /
                     2.0f;
    posX = -999;
    setPosition(x, y, z);
    needsUpdate = false;
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
      posXMinus(other.posXMinus),
      posYMinus(other.posYMinus),
      posZMinus(other.posZMinus),
      posXClip(other.posXClip),
      posYClip(other.posYClip),
      posZClip(other.posZClip),
      posXPlus(other.posXPlus),
      posYPlus(other.posYPlus),
      posZPlus(other.posZPlus),
      rendererRadius(other.rendererRadius),
      isInFrustum(other.isInFrustum),
      isVisible(other.isVisible),
      isWaitingOnOcclusionQuery(other.isWaitingOnOcclusionQuery),
      glOcclusionQuery(other.glOcclusionQuery),
      isChunkLit(other.isChunkLit),
      isInitialized(other.isInitialized),
      rendererBoundingBox(other.rendererBoundingBox),
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
        posXMinus = other.posXMinus;
        posYMinus = other.posYMinus;
        posZMinus = other.posZMinus;
        posXClip = other.posXClip;
        posYClip = other.posYClip;
        posZClip = other.posZClip;
        posXPlus = other.posXPlus;
        posYPlus = other.posYPlus;
        posZPlus = other.posZPlus;
        rendererRadius = other.rendererRadius;
        isInFrustum = other.isInFrustum;
        isVisible = other.isVisible;
        isWaitingOnOcclusionQuery = other.isWaitingOnOcclusionQuery;
        glOcclusionQuery = other.glOcclusionQuery;
        isChunkLit = other.isChunkLit;
        isInitialized = other.isInitialized;
        rendererBoundingBox = other.rendererBoundingBox;
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

    setDontDraw();
    posX = x;
    posY = y;
    posZ = z;
    posXPlus = x + sizeWidth / 2;
    posYPlus = y + sizeHeight / 2;
    posZPlus = z + sizeDepth / 2;
    posXClip = x & 1023;
    posYClip = y;
    posZClip = z & 1023;
    posXMinus = x - posXClip;
    posYMinus = y - posYClip;
    posZMinus = z - posZClip;
    constexpr float pad = 2.0f;
    rendererBoundingBox = AxisAlignedBB::getBoundingBox(
        static_cast<double>(static_cast<float>(x) - pad),
        static_cast<double>(static_cast<float>(y) - pad),
        static_cast<double>(static_cast<float>(z) - pad),
        static_cast<double>(static_cast<float>(x + sizeWidth) + pad),
        static_cast<double>(static_cast<float>(y + sizeHeight) + pad),
        static_cast<double>(static_cast<float>(z + sizeDepth) + pad)
    );
    glNewList(static_cast<GLuint>(glRenderList + 2), GL_COMPILE);
    RenderItem::renderAABB(AxisAlignedBB::getBoundingBox(
        static_cast<double>(static_cast<float>(posXClip) - pad),
        static_cast<double>(static_cast<float>(posYClip) - pad),
        static_cast<double>(static_cast<float>(posZClip) - pad),
        static_cast<double>(static_cast<float>(posXClip + sizeWidth) + pad),
        static_cast<double>(static_cast<float>(posYClip + sizeHeight) + pad),
        static_cast<double>(static_cast<float>(posZClip + sizeDepth) + pad)
    ));
    glEndList();
    markDirty();
}

void WorldRenderer::setupGLTranslation() const {
    glTranslatef(static_cast<float>(posXClip), static_cast<float>(posYClip), static_cast<float>(posZClip));
}

void WorldRenderer::updateRenderer(RenderBlocks &renderBlocks) {
    if (!needsUpdate || worldObj == nullptr || glRenderList == 0) {
        return;
    }
    ++chunksUpdated;

    needsUpdate = false;
    const int_t maxX = posX + sizeWidth;
    const int_t maxY = posY + sizeHeight;
    const int_t maxZ = posZ + sizeDepth;

    Chunk::isLit = false;
    ChunkCache chunkCache(*worldObj, posX - 1, posY - 1, posZ - 1, maxX + 1, maxY + 1, maxZ + 1);
    RenderBlocks chunkRenderBlocks(chunkCache);
    for (int_t pass = 0; pass < 2; ++pass) {
        skipRenderPass[pass] = true;
        bool renderedAny = false;
        bool startedDrawing = false;
        bool renderMore = false;
        Tessellator &tessellator = Tessellator::instance;

        for (int_t y = posY; y < maxY; ++y) {
            for (int_t z = posZ; z < maxZ; ++z) {
                for (int_t x = posX; x < maxX; ++x) {
                    const int_t blockId = chunkCache.getBlockId(x, y, z);
                    if (blockId <= 0) {
                        continue;
                    }

                    if (!startedDrawing) {
                        startedDrawing = true;
                        glNewList(static_cast<GLuint>(glRenderList + pass), GL_COMPILE);
                        glPushMatrix();
                        setupGLTranslation();
                        constexpr float scale = 1.000001f;
                        glTranslatef(static_cast<float>(-sizeDepth) / 2.0f, static_cast<float>(-sizeHeight) / 2.0f,
                                     static_cast<float>(-sizeDepth) / 2.0f);
                        glScalef(scale, scale, scale);
                        glTranslatef(static_cast<float>(sizeDepth) / 2.0f, static_cast<float>(sizeHeight) / 2.0f,
                                     static_cast<float>(sizeDepth) / 2.0f);
                        tessellator.startDrawingQuads();
                        tessellator.setTranslationD(static_cast<double>(-posX), static_cast<double>(-posY),
                                                    static_cast<double>(-posZ));
                    }

                    Block *block = blockId < static_cast<int_t>(Block::blocksList.size())
                                       ? Block::blocksList[blockId]
                                       : nullptr;
                    if (block == nullptr) {
                        continue;
                    }
                    const int_t renderPass = block->getRenderBlockPass();
                    if (renderPass != pass) {
                        renderMore = true;
                        continue;
                    }

                    renderedAny = chunkRenderBlocks.renderBlockByRenderType(block, x, y, z) || renderedAny;
                }
            }
        }

        if (startedDrawing) {
            tessellator.draw();
            glPopMatrix();
            glEndList();
            tessellator.setTranslationD(0.0, 0.0, 0.0);
        } else {
            renderedAny = false;
        }
        skipRenderPass[pass] = !renderedAny;
        if (!renderMore) {
            break;
        }
    }

    isChunkLit = Chunk::isLit;
    isInitialized = true;
}

void WorldRenderer::markDirty() {
    needsUpdate = true;
}

void WorldRenderer::stopRendering() {
    setDontDraw();
    worldObj = nullptr;
}

void WorldRenderer::setDontDraw() {
    skipRenderPass = {true, true};
    isInFrustum = false;
    isInitialized = false;
}

void WorldRenderer::updateInFrustum(ICamera &camera) {
    isInFrustum = camera.isBoundingBoxInFrustum(rendererBoundingBox);
}

void WorldRenderer::render(const int_t pass) const {
    if (!skipRenderPass[pass] && glRenderList != 0) {
        glCallList(static_cast<GLuint>(glRenderList + pass));
    }
}

float WorldRenderer::distanceToEntitySquared(const Entity &entity) const {
    const float dx = static_cast<float>(entity.posX - static_cast<double>(posXPlus));
    const float dy = static_cast<float>(entity.posY - static_cast<double>(posYPlus));
    const float dz = static_cast<float>(entity.posZ - static_cast<double>(posZPlus));
    return dx * dx + dy * dy + dz * dz;
}

bool WorldRenderer::skipAllRenderPasses() const {
    return isInitialized && skipRenderPass[0] && skipRenderPass[1];
}

int_t WorldRenderer::getGLCallListForPass(const int_t pass) const {
    return !isInFrustum ? -1 : (!skipRenderPass[pass] ? glRenderList + pass : -1);
}

void WorldRenderer::callOcclusionQueryList() const {
    glCallList(static_cast<GLuint>(glRenderList + 2));
}

void WorldRenderer::releaseList() {
    if (glRenderList != 0) {
        glDeleteLists(static_cast<GLuint>(glRenderList), 3);
        glRenderList = 0;
    }
}
