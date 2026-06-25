//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_RENDERGLOBAL_H
#define MCPORT_RENDERGLOBAL_H

#include <memory>
#include <unordered_map>

#include "game/world/IWorldAccess.h"
#include "java/Type.h"

class Block;
class AxisAlignedBB;
class EntityPlayer;
class Minecraft;
class MovingObjectPosition;
class RenderBlocks;
class RenderEngine;
class World;
class WorldRenderer;

class RenderGlobal : public IWorldAccess {
private:
    Minecraft &minecraft;
    RenderEngine &renderEngine;
    World *theWorld = nullptr;
    std::unique_ptr<RenderBlocks> globalRenderBlocks;
    std::unordered_map<long_t, std::unique_ptr<WorldRenderer> > worldRenderers;

    int_t getRenderRadius() const;

    WorldRenderer &getWorldRenderer(int_t chunkX, int_t chunkY, int_t chunkZ);

    static long_t rendererKey(int_t chunkX, int_t chunkY, int_t chunkZ);

public:
    float damagePartialTime = 0.0f;

    RenderGlobal(Minecraft &minecraft, RenderEngine &renderEngine);

    ~RenderGlobal();

    void changeWorld(World *world);

    void renderWorld(float partialTicks);

    void drawBlockBreaking(EntityPlayer &player, const MovingObjectPosition &hit, int_t mode, float partialTicks);

    void drawSelectionBox(EntityPlayer &player, const MovingObjectPosition &hit, int_t mode, float partialTicks);

    void markBlockRangeNeedsUpdate(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    void updateAllRenderers() override;

private:
    static void drawOutlinedBoundingBox(const AxisAlignedBB &box);
};


#endif //MCPORT_RENDERGLOBAL_H
