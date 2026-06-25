//
// Created by Keir Davis on 24/06/2026.
//

#include "RenderGlobal.h"

#include <algorithm>
#include <cmath>

#include <glad/glad.h>

#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "WorldRenderer.h"
#include "game/block/Block.h"
#include "game/client/Minecraft.h"
#include "game/client/options/GameSettings.h"
#include "game/world/World.h"

RenderGlobal::RenderGlobal(Minecraft &minecraft, RenderEngine &renderEngine) : minecraft(minecraft),
                                                                               renderEngine(renderEngine),
                                                                               globalRenderBlocks(
                                                                                   std::make_unique<RenderBlocks>()) {
}

RenderGlobal::~RenderGlobal() = default;

void RenderGlobal::changeWorld(World *world) {
    if (theWorld != nullptr) {
        theWorld->setRenderGlobal(nullptr);
    }
    worldRenderers.clear();
    theWorld = world;
    if (theWorld != nullptr) {
        theWorld->setRenderGlobal(this);
    }
}

int_t RenderGlobal::getRenderRadius() const {
    if (minecraft.options == nullptr) {
        return 4;
    }

    switch (minecraft.options->renderDistance) {
        case 0:
            return 5;
        case 1:
            return 4;
        case 2:
            return 3;
        default:
            return 2;
    }
}

WorldRenderer &RenderGlobal::getWorldRenderer(const int_t chunkX, const int_t chunkY, const int_t chunkZ) {
    const long_t key = rendererKey(chunkX, chunkY, chunkZ);
    const auto found = worldRenderers.find(key);
    if (found != worldRenderers.end()) {
        return *found->second;
    }

    constexpr int_t sectionSize = 16;
    const int_t list = static_cast<int_t>(glGenLists(1));
    auto renderer = std::make_unique<WorldRenderer>(theWorld, chunkX * sectionSize, chunkY * sectionSize,
                                                    chunkZ * sectionSize, sectionSize, list);
    WorldRenderer &rendererRef = *renderer;
    worldRenderers.emplace(key, std::move(renderer));
    return rendererRef;
}

long_t RenderGlobal::rendererKey(const int_t chunkX, const int_t chunkY, const int_t chunkZ) {
    return (static_cast<long_t>(chunkX) & 0x3FFFFFFL) << 38 | (static_cast<long_t>(chunkZ) & 0x3FFFFFFL) << 12 |
           (static_cast<long_t>(chunkY) & 0xFFFL);
}

void RenderGlobal::renderWorld(float) {
    if (theWorld == nullptr) {
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/terrain.png"));

    constexpr int_t chunkSize = 16;
    const int_t renderRadius = getRenderRadius();
    const int_t centerX = static_cast<int_t>(std::floor(minecraft.getPlayerPosX()));
    const int_t centerZ = static_cast<int_t>(std::floor(minecraft.getPlayerPosZ()));
    const int_t centerChunkX = centerX >= 0 ? centerX / chunkSize : (centerX - chunkSize + 1) / chunkSize;
    const int_t centerChunkZ = centerZ >= 0 ? centerZ / chunkSize : (centerZ - chunkSize + 1) / chunkSize;

    int_t updatedThisFrame = 0;
    for (int_t chunkX = centerChunkX - renderRadius; chunkX <= centerChunkX + renderRadius; ++chunkX) {
        for (int_t chunkZ = centerChunkZ - renderRadius; chunkZ <= centerChunkZ + renderRadius; ++chunkZ) {
            for (int_t chunkY = 0; chunkY < 8; ++chunkY) {
                WorldRenderer &renderer = getWorldRenderer(chunkX, chunkY, chunkZ);
                if (renderer.needsUpdate && updatedThisFrame < 8) {
                    renderer.updateRenderer(*globalRenderBlocks);
                    ++updatedThisFrame;
                }
                renderer.render();
            }
        }
    }
}

void RenderGlobal::markBlockRangeNeedsUpdate(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1) {
    if (x0 > x1) {
        std::swap(x0, x1);
    }
    if (y0 > y1) {
        std::swap(y0, y1);
    }
    if (z0 > z1) {
        std::swap(z0, z1);
    }

    constexpr int_t sectionSize = 16;
    const int_t minChunkX = x0 >= 0 ? x0 / sectionSize : (x0 - sectionSize + 1) / sectionSize;
    const int_t maxChunkX = x1 >= 0 ? x1 / sectionSize : (x1 - sectionSize + 1) / sectionSize;
    const int_t minChunkY = std::max<int_t>(0, y0 / sectionSize);
    const int_t maxChunkY = std::min<int_t>(7, y1 / sectionSize);
    const int_t minChunkZ = z0 >= 0 ? z0 / sectionSize : (z0 - sectionSize + 1) / sectionSize;
    const int_t maxChunkZ = z1 >= 0 ? z1 / sectionSize : (z1 - sectionSize + 1) / sectionSize;

    for (int_t chunkX = minChunkX; chunkX <= maxChunkX; ++chunkX) {
        for (int_t chunkY = minChunkY; chunkY <= maxChunkY; ++chunkY) {
            for (int_t chunkZ = minChunkZ; chunkZ <= maxChunkZ; ++chunkZ) {
                const auto found = worldRenderers.find(rendererKey(chunkX, chunkY, chunkZ));
                if (found != worldRenderers.end()) {
                    found->second->markDirty();
                }
            }
        }
    }
}

void RenderGlobal::markAllRenderersNeedsUpdate() {
    for (auto &[key, renderer]: worldRenderers) {
        (void) key;
        renderer->markDirty();
    }
}
