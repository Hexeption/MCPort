//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYRENDERER_H
#define MCPORT_ENTITYRENDERER_H

#include <memory>

#include "java/System.h"
#include "java/Type.h"
#include "game/client/renderer/ItemRenderer.h"
#include "game/phys/Frustum.h"

class Material;
class Minecraft;
class Entity;

class EntityRenderer {
private:
    Minecraft &mc;
    float farPlaneDistance = 0.0f;
    float fogColorRed = 0.0f;
    float fogColorGreen = 0.0f;
    float fogColorBlue = 0.0f;
    float prevFogColor = 0.0f;
    float fogColor = 0.0f;
    long_t prevFrameTime = System::currentTimeMillis();
    int_t rendererUpdateCount = 0;
    std::unique_ptr<ItemRenderer> itemRenderer;
    Entity *pointedEntity = nullptr;

    void updateFogColor(float partialTicks);

    void getMouseOver(float partialTicks);

    void setupCameraTransform(float partialTicks);

    void setupFog(int_t mode);

    bool isPlayerInsideMaterial(Material *material) const;

    void renderWarpedTextureOverlay(float partialTicks);

    float getFOVModifier(float partialTicks) const;

    void hurtCameraEffect(float partialTicks);

    void setupViewBobbing(float partialTicks);

    void orientCamera(float partialTicks);

    void setupCameraTransform(float partialTicks, int_t pass);

    void renderHand(float partialTicks, int_t pass);

    void addRainParticles();

    void renderSnow(float partialTicks);

public:
    explicit EntityRenderer(Minecraft &minecraft);

    void updateRenderer();

    void renderWorld(float partialTicks);

    void updateCameraAndRender(float partialTicks);

    void setupOverlayRendering();
};

#endif //MCPORT_ENTITYRENDERER_H
