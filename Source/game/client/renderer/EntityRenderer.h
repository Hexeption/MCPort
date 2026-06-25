//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYRENDERER_H
#define MCPORT_ENTITYRENDERER_H

#include "java/System.h"
#include "java/Type.h"

class Material;
class Minecraft;

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

    void updateFogColor(float partialTicks);

    void getMouseOver(float partialTicks);

    void setupCameraTransform(float partialTicks);

    void setupFog(int_t mode);

    bool isPlayerInsideMaterial(Material *material) const;

    void renderWarpedTextureOverlay(float partialTicks);

public:
    explicit EntityRenderer(Minecraft &minecraft);

    void updateRenderer();

    void renderWorld(float partialTicks);

    void updateCameraAndRender(float partialTicks);

    void setupOverlayRendering();
};

#endif //MCPORT_ENTITYRENDERER_H
