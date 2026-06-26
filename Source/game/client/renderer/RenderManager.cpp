//
// Created by Keir Davis on 25/06/2026.
//

#include "RenderManager.h"

#include <glad/glad.h>

#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/Render.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/RenderItem.h"
#include "game/client/options/GameSettings.h"
#include "game/entity/Entity.h"
#include "game/entity/EntityItem.h"
#include "game/entity/EntityPlayer.h"
#include "game/client/renderer/RenderEntity.h"

RenderManager RenderManager::instance;

double RenderManager::renderPosX = 0.0;
double RenderManager::renderPosY = 0.0;
double RenderManager::renderPosZ = 0.0;

RenderManager::RenderManager() {
    entityRenderMap[typeid(Entity)] = std::make_unique<RenderEntity>();
    entityRenderMap[typeid(EntityItem)] = std::make_unique<RenderItem>();

    for (auto &entry: entityRenderMap) {
        entry.second->setRenderManager(*this);
    }
}

Render *RenderManager::getEntityRenderObject(Entity &entity) {
    auto found = entityRenderMap.find(typeid(entity));
    if (found != entityRenderMap.end()) {
        return found->second.get();
    }

    found = entityRenderMap.find(typeid(Entity));
    return found == entityRenderMap.end() ? nullptr : found->second.get();
}

void RenderManager::cacheActiveRenderInfo(World &world, RenderEngine &engine, FontRenderer &font,
                                          EntityPlayer &playerEntity, GameSettings &settings,
                                          float partialTicks) {
    worldObj = &world;
    renderEngine = &engine;
    options = &settings;
    player = &playerEntity;
    fontRenderer = &font;

    playerViewY = playerEntity.prevRotationYaw +
                  (playerEntity.rotationYaw - playerEntity.prevRotationYaw) * partialTicks;

    playerViewX = playerEntity.prevRotationPitch +
                  (playerEntity.rotationPitch - playerEntity.prevRotationPitch) * partialTicks;

    viewerPosX = playerEntity.lastTickPosX + (playerEntity.posX - playerEntity.lastTickPosX) * partialTicks;
    viewerPosY = playerEntity.lastTickPosY + (playerEntity.posY - playerEntity.lastTickPosY) * partialTicks;
    viewerPosZ = playerEntity.lastTickPosZ + (playerEntity.posZ - playerEntity.lastTickPosZ) * partialTicks;

    renderPosX = viewerPosX;
    renderPosY = viewerPosY;
    renderPosZ = viewerPosZ;
}

void RenderManager::renderEntity(Entity &entity, float partialTicks) {
    const double x = entity.lastTickPosX + (entity.posX - entity.lastTickPosX) * partialTicks;
    const double y = entity.lastTickPosY + (entity.posY - entity.lastTickPosY) * partialTicks;
    const double z = entity.lastTickPosZ + (entity.posZ - entity.lastTickPosZ) * partialTicks;

    const float yaw = entity.prevRotationYaw +
                      (entity.rotationYaw - entity.prevRotationYaw) * partialTicks;

    const float brightness = entity.getBrightness(partialTicks);
    glColor3f(brightness, brightness, brightness);

    renderEntityWithPosYaw(
        entity,
        x - renderPosX,
        y - renderPosY,
        z - renderPosZ,
        yaw,
        partialTicks
    );
}

void RenderManager::renderEntityWithPosYaw(Entity &entity, double x, double y, double z,
                                           float yaw, float partialTicks) {
    Render *renderer = getEntityRenderObject(entity);

    if (renderer != nullptr) {
        renderer->doRender(entity, x, y, z, yaw, partialTicks);
        renderer->doRenderShadowAndFire(entity, x, y, z, yaw, partialTicks);
    }
}

void RenderManager::set(World &world) {
    worldObj = &world;
}

double RenderManager::getDistanceToCamera(double x, double y, double z) const {
    const double dx = x - viewerPosX;
    const double dy = y - viewerPosY;
    const double dz = z - viewerPosZ;

    return dx * dx + dy * dy + dz * dz;
}

FontRenderer *RenderManager::getFontRenderer() {
    return fontRenderer;
}
