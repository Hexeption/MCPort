//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_WORLDRENDERER_H
#define MCPORT_WORLDRENDERER_H

#include <memory>

#include "java/Type.h"

class RenderBlocks;
class World;

class WorldRenderer {
public:
    World *worldObj = nullptr;
    int_t posX = 0;
    int_t posY = 0;
    int_t posZ = 0;
    int_t sizeWidth = 16;
    int_t sizeHeight = 16;
    int_t sizeDepth = 16;
    bool needsUpdate = true;
    bool skipRenderPass = true;
    static int_t chunksUpdated;

    WorldRenderer(World *world, int_t x, int_t y, int_t z, int_t size, int_t glRenderList);

    ~WorldRenderer();

    WorldRenderer(const WorldRenderer &) = delete;

    WorldRenderer &operator=(const WorldRenderer &) = delete;

    WorldRenderer(WorldRenderer &&other) noexcept;

    WorldRenderer &operator=(WorldRenderer &&other) noexcept;

    void setPosition(int_t x, int_t y, int_t z);

    void updateRenderer(RenderBlocks &renderBlocks);

    void markDirty();

    void stopRendering();

    void render() const;

private:
    int_t glRenderList = 0;

    void releaseList();
};

#endif //MCPORT_WORLDRENDERER_H
