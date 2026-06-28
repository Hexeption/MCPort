//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_RENDERMANAGER_H
#define MCPORT_RENDERMANAGER_H


#include <memory>
#include <typeindex>
#include <unordered_map>

#include "java/Type.h"
#include "game/client/renderer/ItemRenderer.h"

#include "game/entity/Entity.h"
#include "game/entity/EntityItem.h"
#include "game/entity/EntityPlayer.h"
#include "game/client/renderer/FontRenderer.h"
#include "game/client/options/GameSettings.h"
#include "game/client/renderer/Render.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/RenderEntity.h"
#include "game/world/World.h"

class RenderManager {
private:
    std::unordered_map<std::type_index, std::unique_ptr<Render> > entityRenderMap;
    FontRenderer *fontRenderer = nullptr;

    RenderManager();

public:
    static RenderManager instance;

    static double renderPosX;
    static double renderPosY;
    static double renderPosZ;

    RenderEngine *renderEngine = nullptr;
    ItemRenderer *itemRenderer = nullptr;
    std::unique_ptr<ItemRenderer> ownedItemRenderer;
    World *worldObj = nullptr;
    EntityPlayer *player = nullptr;
    float playerViewY = 0.0f;
    float playerViewX = 0.0f;
    GameSettings *options = nullptr;
    double viewerPosX = 0.0;
    double viewerPosY = 0.0;
    double viewerPosZ = 0.0;

    Render *getEntityRenderObject(Entity &entity);

    void cacheActiveRenderInfo(World &world, RenderEngine &engine, FontRenderer &font, EntityPlayer &playerEntity,
                               GameSettings &settings, float partialTicks);

    void renderEntity(Entity &entity, float partialTicks);

    void renderEntityWithPosYaw(Entity &entity, double x, double y, double z, float yaw, float partialTicks);

    void set(World &world);

    double getDistanceToCamera(double x, double y, double z) const;

    FontRenderer *getFontRenderer();
};


#endif //MCPORT_RENDERMANAGER_H
