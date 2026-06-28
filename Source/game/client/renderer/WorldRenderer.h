//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_WORLDRENDERER_H
#define MCPORT_WORLDRENDERER_H

#include <array>
#include <memory>

#include "game/phys/AxisAlignedBB.h"
#include "java/Type.h"

#include "game/client/renderer/RenderBlocks.h"
#include "game/entity/Entity.h"
#include "game/phys/ICamera.h"
#include "game/world/World.h"

class WorldRenderer {
public:
    World *worldObj = nullptr;
    int_t posX = 0;
    int_t posY = 0;
    int_t posZ = 0;
    int_t sizeWidth = 16;
    int_t sizeHeight = 16;
    int_t sizeDepth = 16;
    int_t posXMinus = 0;
    int_t posYMinus = 0;
    int_t posZMinus = 0;
    int_t posXClip = 0;
    int_t posYClip = 0;
    int_t posZClip = 0;
    int_t posXPlus = 0;
    int_t posYPlus = 0;
    int_t posZPlus = 0;
    float rendererRadius = 0.0f;
    bool isInFrustum = false;
    bool isVisible = true;
    bool isWaitingOnOcclusionQuery = false;
    int_t glOcclusionQuery = 0;
    bool isChunkLit = false;
    bool isInitialized = false;
    AxisAlignedBB rendererBoundingBox;
    bool needsUpdate = true;
    std::array<bool, 2> skipRenderPass{true, true};
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

    void render(int_t pass) const;

    float distanceToEntitySquared(const Entity &entity) const;

    void setDontDraw();

    void updateInFrustum(class ICamera &camera);

    bool skipAllRenderPasses() const;

    int_t getGLCallListForPass(int_t pass) const;

    void callOcclusionQueryList() const;

    void setupGLTranslation() const;

private:
    int_t glRenderList = 0;

    void releaseList();
};

#endif //MCPORT_WORLDRENDERER_H
