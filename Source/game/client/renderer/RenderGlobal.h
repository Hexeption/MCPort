//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERGLOBAL_H
#define MCPORT_RENDERGLOBAL_H

#include <memory>
#include <vector>
#include <array>

#include "game/world/IWorldAccess.h"
#include "java/String.h"
#include "java/Type.h"
#include "RenderList.h"

class Block;
class Minecraft;
class World;
class WorldRenderer;

#include "game/entity/EntityPlayer.h"
#include "game/phys/AxisAlignedBB.h"
#include "game/phys/ICamera.h"
#include "game/phys/MovingObjectPosition.h"
#include "game/util/Vec3D.h"
#include "game/client/renderer/RenderBlocks.h"
#include "game/client/renderer/RenderEngine.h"

class RenderGlobal : public IWorldAccess {
private:
    Minecraft &minecraft;
    RenderEngine &renderEngine;
    World *theWorld = nullptr;
    std::unique_ptr<RenderBlocks> globalRenderBlocks;
    std::vector<std::unique_ptr<WorldRenderer> > worldRenderers;
    std::vector<WorldRenderer *> sortedWorldRenderers;
    std::vector<WorldRenderer *> worldRenderersToUpdate;
    std::vector<WorldRenderer *> glRenderLists;
    std::array<RenderList, 4> allRenderLists;

    int_t renderChunksWide = 0;
    int_t renderChunksTall = 0;
    int_t renderChunksDeep = 0;
    int_t glRenderListBase = 0;
    int_t minBlockX = 0;
    int_t minBlockY = 0;
    int_t minBlockZ = 0;
    int_t maxBlockX = 0;
    int_t maxBlockY = 0;
    int_t maxBlockZ = 0;
    int_t renderersLoaded = 0;
    int_t renderersBeingClipped = 0;
    int_t renderersBeingOccluded = 0;
    int_t renderersBeingRendered = 0;
    int_t renderersSkippingRenderPass = 0;
    int_t renderDistance = -1;
    int_t frustumCheckOffset = 0;
    int_t cloudTickCounter = 0;
    int_t starGLCallList = 0;
    int_t glSkyList = 0;
    int_t glSkyList2 = 0;
    int_t countEntitiesTotal = 0;
    int_t countEntitiesRendered = 0;
    int_t countEntitiesHidden = 0;
    double prevSortX = -9999.0;
    double prevSortY = -9999.0;
    double prevSortZ = -9999.0;

    int_t getRenderRadius() const;

    WorldRenderer *getWorldRenderer(int_t x, int_t y, int_t z);

    void loadRenderers();

    void markRenderersForNewPosition(int_t x, int_t y, int_t z);

    int_t renderSortedRenderers(int_t start, int_t end, int_t pass, double partialTicks);

public:
    float damagePartialTime = 0.0f;

    RenderGlobal(Minecraft &minecraft, RenderEngine &renderEngine);

    ~RenderGlobal();

    void changeWorld(World *world);

    void renderWorld(float partialTicks);

    void renderEntities(const Vec3D &cameraPos, ICamera &camera, float partialTicks);

    void clipRenderersByFrustum(ICamera &camera, float partialTicks);

    int_t sortAndRender(EntityPlayer &player, int_t pass, double partialTicks);

    void renderAllRenderLists(int_t pass, double partialTicks);

    void renderSky(float partialTicks);

    void renderClouds(float partialTicks);

    void renderCloudsFancy(float partialTicks);

    void updateClouds();

    bool updateRenderers(EntityPlayer &player, bool force);

    void drawBlockBreaking(EntityPlayer &player, const MovingObjectPosition &hit, int_t mode, float partialTicks);

    void drawSelectionBox(EntityPlayer &player, const MovingObjectPosition &hit, int_t mode, float partialTicks);

    void markBlockRangeNeedsUpdate(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    void updateAllRenderers() override;

    jstring getDebugInfoRenders();

    jstring getDebugInfoEntities();

private:
    static void drawOutlinedBoundingBox(const AxisAlignedBB &box);

    void renderStars();
};


#endif //MCPORT_RENDERGLOBAL_H
