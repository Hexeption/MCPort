//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityRenderer.h"

#include <cmath>
#include <memory>

#include <glad/glad.h>

#include "RenderGlobal.h"
#include "game/block/Material.h"
#include "game/client/Minecraft.h"
#include "game/client/options/GameSettings.h"
#include "game/client/player/PlayerController.h"
#include "game/entity/EntityPlayerSP.h"
#include "game/util/Vec3D.h"
#include "game/world/World.h"
#include "lwjgl/GLContext.h"
#include "utils/GLU.h"

EntityRenderer::EntityRenderer(Minecraft &minecraft) : mc(minecraft) {
}

void EntityRenderer::updateRenderer() {
    if (mc.theWorld == nullptr || mc.thePlayer == nullptr || mc.options == nullptr) {
        return;
    }

    prevFogColor = fogColor;
    const int_t playerX = static_cast<int_t>(std::floor(mc.thePlayer->posX));
    const int_t playerY = static_cast<int_t>(std::floor(mc.thePlayer->posY));
    const int_t playerZ = static_cast<int_t>(std::floor(mc.thePlayer->posZ));
    const float brightness = mc.theWorld->getBrightness(playerX, playerY, playerZ);
    const float distanceFactor = static_cast<float>(3 - mc.options->renderDistance) / 3.0f;
    const float targetFogColor = brightness * (1.0f - distanceFactor) + distanceFactor;
    fogColor += (targetFogColor - fogColor) * 0.1f;
}

void EntityRenderer::renderWorld(const float partialTicks) {
    if (mc.theWorld == nullptr || mc.thePlayer == nullptr || mc.options == nullptr || mc.renderGlobal == nullptr) {
        return;
    }

    getMouseOver(partialTicks);
    updateFogColor(partialTicks);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupCameraTransform(partialTicks);
    glEnable(GL_FOG);
    setupFog(1);
    setupFog(0);
    mc.renderGlobal->renderWorld(partialTicks);
    if (mc.objectMouseOver != nullptr) {
        glDisable(GL_ALPHA_TEST);
        mc.renderGlobal->drawBlockBreaking(*mc.thePlayer, *mc.objectMouseOver, 0, partialTicks);
        mc.renderGlobal->drawSelectionBox(*mc.thePlayer, *mc.objectMouseOver, 0, partialTicks);
        glEnable(GL_ALPHA_TEST);
    }
    glDisable(GL_FOG);
}

void EntityRenderer::updateFogColor(const float partialTicks) {
    float distanceBlend = 1.0f / static_cast<float>(4 - mc.options->renderDistance);
    distanceBlend = 1.0f - std::pow(distanceBlend, 0.25f);
    std::unique_ptr<Vec3D> skyColor = mc.theWorld->getSkyColor(partialTicks);
    const float skyRed = static_cast<float>(skyColor->xCoord);
    const float skyGreen = static_cast<float>(skyColor->yCoord);
    const float skyBlue = static_cast<float>(skyColor->zCoord);
    std::unique_ptr<Vec3D> worldFogColor = mc.theWorld->getFogColor(partialTicks);
    fogColorRed = static_cast<float>(worldFogColor->xCoord);
    fogColorGreen = static_cast<float>(worldFogColor->yCoord);
    fogColorBlue = static_cast<float>(worldFogColor->zCoord);
    fogColorRed += (skyRed - fogColorRed) * distanceBlend;
    fogColorGreen += (skyGreen - fogColorGreen) * distanceBlend;
    fogColorBlue += (skyBlue - fogColorBlue) * distanceBlend;

    if (isPlayerInsideMaterial(Material::water)) {
        fogColorRed = 0.02f;
        fogColorGreen = 0.02f;
        fogColorBlue = 0.2f;
    } else if (isPlayerInsideMaterial(Material::lava)) {
        fogColorRed = 0.6f;
        fogColorGreen = 0.1f;
        fogColorBlue = 0.0f;
    }

    const float brightnessBlend = prevFogColor + (fogColor - prevFogColor) * partialTicks;
    fogColorRed *= brightnessBlend;
    fogColorGreen *= brightnessBlend;
    fogColorBlue *= brightnessBlend;
    if (mc.options->anaglyph) {
        const float anaglyphRed = (fogColorRed * 30.0f + fogColorGreen * 59.0f + fogColorBlue * 11.0f) / 100.0f;
        const float anaglyphGreen = (fogColorRed * 30.0f + fogColorGreen * 70.0f) / 100.0f;
        const float anaglyphBlue = (fogColorRed * 30.0f + fogColorBlue * 70.0f) / 100.0f;
        fogColorRed = anaglyphRed;
        fogColorGreen = anaglyphGreen;
        fogColorBlue = anaglyphBlue;
    }

    glClearColor(fogColorRed, fogColorGreen, fogColorBlue, 0.0f);
}

void EntityRenderer::getMouseOver(const float partialTicks) {
    if (mc.thePlayer == nullptr) {
        mc.objectMouseOver = nullptr;
        return;
    }

    const double reachDistance = mc.playerController != nullptr ? mc.playerController->getBlockReachDistance() : 3.0;
    MovingObjectPosition hit = mc.thePlayer->rayTrace(reachDistance, partialTicks);
    if (hit.hitVec == nullptr) {
        mc.objectMouseOver = nullptr;
        return;
    }

    mc.objectMouseOver = std::make_unique<MovingObjectPosition>(std::move(hit));
}

void EntityRenderer::setupCameraTransform(const float partialTicks) {
    farPlaneDistance = static_cast<float>(256 >> mc.options->renderDistance);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLU::gluPerspective(70.0f, static_cast<float>(mc.displayWidth) / static_cast<float>(mc.displayHeight), 0.05f,
                        farPlaneDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const double cameraX = mc.thePlayer->prevPosX + (mc.thePlayer->posX - mc.thePlayer->prevPosX) * partialTicks;
    const double cameraY = mc.thePlayer->prevPosY + (mc.thePlayer->posY - mc.thePlayer->prevPosY) * partialTicks;
    const double cameraZ = mc.thePlayer->prevPosZ + (mc.thePlayer->posZ - mc.thePlayer->prevPosZ) * partialTicks;
    const float cameraYaw = mc.thePlayer->prevRotationYaw + (mc.thePlayer->rotationYaw - mc.thePlayer->prevRotationYaw)
                            * partialTicks;
    const float cameraPitch = mc.thePlayer->prevRotationPitch + (mc.thePlayer->rotationPitch -
                              mc.thePlayer->prevRotationPitch) * partialTicks;
    glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraYaw + 180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(static_cast<float>(-cameraX), static_cast<float>(-cameraY), static_cast<float>(-cameraZ));
}

void EntityRenderer::setupFog(const int_t mode) {
    const float fogColorBuffer[] = {fogColorRed, fogColorGreen, fogColorBlue, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColorBuffer);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    if (isPlayerInsideMaterial(Material::water)) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.1f);
    } else if (isPlayerInsideMaterial(Material::lava)) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 2.0f);
    } else {
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, farPlaneDistance * 0.25f);
        glFogf(GL_FOG_END, farPlaneDistance);
        if (mode < 0) {
            glFogf(GL_FOG_START, 0.0f);
            glFogf(GL_FOG_END, farPlaneDistance * 0.8f);
        }

        if (lwjgl::GLContext::getCapabilities()["GL_NV_fog_distance"]) {
            glFogi(GL_FOG_DISTANCE_MODE_NV, GL_EYE_RADIAL_NV);
        }
    }

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
}

bool EntityRenderer::isPlayerInsideMaterial(Material *material) const {
    if (mc.theWorld == nullptr || mc.thePlayer == nullptr || material == nullptr) {
        return false;
    }

    const int_t x = static_cast<int_t>(std::floor(mc.thePlayer->posX));
    const double eyeY = mc.thePlayer->posY + static_cast<double>(mc.thePlayer->getEyeHeight());
    const int_t y = static_cast<int_t>(std::floor(eyeY));
    const int_t z = static_cast<int_t>(std::floor(mc.thePlayer->posZ));
    return mc.theWorld->getBlockMaterial(x, y, z) == material;
}
