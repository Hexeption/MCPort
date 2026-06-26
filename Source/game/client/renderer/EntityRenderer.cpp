//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityRenderer.h"

#include <cmath>
#include <memory>

#include <glad/glad.h>

#include "RenderGlobal.h"
#include "RenderManager.h"
#include "ItemRenderer.h"
#include "ScaledResolution.h"
#include "Tessellator.h"
#include "game/client/MathHelper.h"
#include "game/block/Material.h"
#include "game/client/Minecraft.h"
#include "game/client/options/GameSettings.h"
#include "game/client/player/PlayerController.h"
#include "game/entity/EntityPlayerSP.h"
#include "game/phys/Frustum.h"
#include "game/util/Vec3D.h"
#include "game/world/World.h"
#include "lwjgl/Display.h"
#include "lwjgl/GLContext.h"
#include "lwjgl/Mouse.h"
#include "utils/GLU.h"

EntityRenderer::EntityRenderer(Minecraft &minecraft) : mc(minecraft) {
    itemRenderer = std::make_unique<ItemRenderer>(minecraft);
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
    ++rendererUpdateCount;
    if (itemRenderer != nullptr) {
        itemRenderer->updateEquippedItem();
    }
}

void EntityRenderer::updateCameraAndRender(float partialTicks) {
    if (!lwjgl::Display::isActive()) {
        if (System::currentTimeMillis() - prevFrameTime > 500L) {
            mc.displayInGameMenu();
        }
    } else {
        prevFrameTime = System::currentTimeMillis();
    }

    int var3;
    if (mc.inGameHasFocus) {
        mc.mouseHelper.mouseXYChange();
        int var2 = mc.mouseHelper.deltaX;
        var3 = mc.mouseHelper.deltaY;
        byte_t var4 = 1;
        if (mc.options->invertMouse) {
            var4 = -1;
        }

        mc.thePlayer->setAngles((float) var2, (float) (var3 * var4));
    }

    if (!mc.skipRenderWorld) {
        ScaledResolution scaledResolution(mc.displayWidth, mc.displayHeight);
        const int_t scaledWidth = scaledResolution.getScaledWidth();
        const int_t scaledHeight = scaledResolution.getScaledHeight();
        int mouseX = lwjgl::Mouse::getX() * scaledWidth / mc.displayWidth;
        int mouseY = scaledHeight - lwjgl::Mouse::getY() * scaledHeight / mc.displayHeight - 1;

        if (mc.theWorld != nullptr) {
            renderWorld(partialTicks);
            mc.ingameGui->renderGameOverlay(partialTicks, mc.currentScreen != nullptr, mouseX, mouseY);
        } else {
            glViewport(0, 0, mc.displayWidth, mc.displayHeight);
            glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            setupOverlayRendering();
        }

        if (mc.currentScreen != nullptr) {
            glClear(GL_DEPTH_BUFFER_BIT);
            mc.currentScreen->drawScreen(mouseX, mouseY, partialTicks);
        }
    }
}

void EntityRenderer::setupOverlayRendering() {
    ScaledResolution var1 = ScaledResolution(mc.displayWidth, mc.displayHeight);
    int var2 = var1.getScaledWidth();
    int var3 = var1.getScaledHeight();
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double) var2, (double) var3, 0.0, 1000.0, 3000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -2000.0F);
}

void EntityRenderer::renderWorld(const float partialTicks) {
    if (mc.theWorld == nullptr || mc.thePlayer == nullptr || mc.options == nullptr || mc.renderGlobal == nullptr) {
        return;
    }

    getMouseOver(partialTicks);
    EntityPlayerSP &player = *mc.thePlayer;
    RenderGlobal &renderGlobal = *mc.renderGlobal;
    EffectRenderer &effectRenderer = *mc.effectRenderer;
    updateFogColor(partialTicks);
    std::unique_ptr<Vec3D> cameraPos = player.getPosition(partialTicks);

    for (int_t pass = 0; pass < 2; ++pass) {
        if (mc.options->anaglyph) {
            if (pass == 0) {
                glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_FALSE);
            } else {
                glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
            }
        }

        glViewport(0, 0, mc.displayWidth, mc.displayHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        setupCameraTransform(partialTicks, pass);
        Frustum frustum;
        frustum.setPosition(cameraPos->xCoord, cameraPos->yCoord, cameraPos->zCoord);
        if (mc.options->renderDistance < 2) {
            setupFog(-1);
            renderGlobal.renderSky(partialTicks);
        }
        glEnable(GL_FOG);
        setupFog(1);
        renderGlobal.clipRenderersByFrustum(frustum, partialTicks);
        renderGlobal.updateRenderers(player, false);
        setupFog(0);
        glEnable(GL_FOG);
        glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/terrain.png"));
        renderGlobal.sortAndRender(player, 0, partialTicks);
        renderGlobal.renderEntities(*cameraPos, frustum, partialTicks);
        effectRenderer.renderLitParticles(player, partialTicks);
        effectRenderer.renderParticles(player, partialTicks);
        if (player.isInsideOfMaterial(Material::water)) {
            glDisable(GL_ALPHA_TEST);
            glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/water.png"));
            renderWarpedTextureOverlay(partialTicks);
            glEnable(GL_ALPHA_TEST);
        }
        if (mc.objectMouseOver != nullptr && !player.isInsideOfMaterial(Material::water)) {
            glDisable(GL_ALPHA_TEST);
            renderGlobal.drawBlockBreaking(player, *mc.objectMouseOver, 0, partialTicks);
            renderGlobal.drawSelectionBox(player, *mc.objectMouseOver, 0, partialTicks);
            glEnable(GL_ALPHA_TEST);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        setupFog(0);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/terrain.png"));
        if (mc.options->fancyGraphics) {
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            const int_t rendered = renderGlobal.sortAndRender(player, 1, partialTicks);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            if (mc.options->anaglyph) {
                if (pass == 0) {
                    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_FALSE);
                } else {
                    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
                }
            }
            if (rendered > 0) {
                renderGlobal.renderAllRenderLists(1, partialTicks);
            }
        } else {
            renderGlobal.sortAndRender(player, 1, partialTicks);
        }
        glDepthMask(true);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        if (mc.objectMouseOver != nullptr && player.isInsideOfMaterial(Material::water)) {
            glDisable(GL_ALPHA_TEST);
            renderGlobal.drawBlockBreaking(player, *mc.objectMouseOver, 0, partialTicks);
            renderGlobal.drawSelectionBox(player, *mc.objectMouseOver, 0, partialTicks);
            glEnable(GL_ALPHA_TEST);
        }
        glDisable(GL_FOG);
        if (mc.theWorld->snowCovered) {
            renderSnow(partialTicks);
        }
        setupFog(0);
        glEnable(GL_FOG);
        renderGlobal.renderClouds(partialTicks);
        glDisable(GL_FOG);
        setupFog(1);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderHand(partialTicks, pass);
        if (!mc.options->anaglyph) {
            break;
        }
    }

    if (mc.options->anaglyph) {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    }
}

void EntityRenderer::renderWarpedTextureOverlay(const float partialTicks) {
    Tessellator &tessellator = Tessellator::instance;
    const float brightness = mc.thePlayer->getBrightness(partialTicks);
    glColor4f(brightness, brightness, brightness, 0.5f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();

    const float textureScale = 4.0f;
    const float minX = -1.0f;
    const float maxX = 1.0f;
    const float minY = -1.0f;
    const float maxY = 1.0f;
    const float z = -0.5f;
    const float uOffset = -mc.thePlayer->rotationYaw / 64.0f;
    const float vOffset = mc.thePlayer->rotationPitch / 64.0f;
    tessellator.startDrawingQuads();
    tessellator.addVertexWithUV(minX, minY, z, textureScale + uOffset, textureScale + vOffset);
    tessellator.addVertexWithUV(maxX, minY, z, uOffset, textureScale + vOffset);
    tessellator.addVertexWithUV(maxX, maxY, z, uOffset, vOffset);
    tessellator.addVertexWithUV(minX, maxY, z, textureScale + uOffset, vOffset);
    tessellator.draw();

    glPopMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
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
    setupCameraTransform(partialTicks, 0);
}

void EntityRenderer::setupCameraTransform(const float partialTicks, const int_t pass) {
    farPlaneDistance = static_cast<float>(256 >> mc.options->renderDistance);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (mc.options->anaglyph) {
        glTranslatef(static_cast<float>(-(pass * 2 - 1)) * 0.07f, 0.0f, 0.0f);
    }
    GLU::gluPerspective(getFOVModifier(partialTicks), static_cast<float>(mc.displayWidth) / mc.displayHeight, 0.05f,
                        farPlaneDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (mc.options->anaglyph) {
        glTranslatef(static_cast<float>(pass * 2 - 1) * 0.1f, 0.0f, 0.0f);
    }
    hurtCameraEffect(partialTicks);
    if (mc.options->viewBobbing) {
        setupViewBobbing(partialTicks);
    }

    orientCamera(partialTicks);
}

float EntityRenderer::getFOVModifier(const float partialTicks) const {
    if (mc.thePlayer == nullptr) {
        return 70.0f;
    }

    float fov = isPlayerInsideMaterial(Material::water) ? 60.0f : 70.0f;
    if (mc.thePlayer->health <= 0) {
        const float deathTime = static_cast<float>(mc.thePlayer->deathTime) + partialTicks;
        fov /= (1.0f - 500.0f / (deathTime + 500.0f)) * 2.0f + 1.0f;
    }
    return fov;
}

void EntityRenderer::hurtCameraEffect(const float partialTicks) {
    if (mc.thePlayer == nullptr) {
        return;
    }

    const float hurtTime = static_cast<float>(mc.thePlayer->hurtTime) - partialTicks;
    if (mc.thePlayer->health <= 0) {
        const float deathTime = static_cast<float>(mc.thePlayer->deathTime) + partialTicks;
        glRotatef(40.0f - 8000.0f / (deathTime + 200.0f), 0.0f, 0.0f, 1.0f);
    }

    if (hurtTime >= 0.0f) {
        float hurtFraction = hurtTime / static_cast<float>(mc.thePlayer->maxHurtTime);
        hurtFraction = MathHelper::sin(hurtFraction * hurtFraction * hurtFraction * hurtFraction *
                                       static_cast<float>(std::acos(-1.0)));
        const float attackedAtYaw = mc.thePlayer->attackedAtYaw;
        glRotatef(-attackedAtYaw, 0.0f, 1.0f, 0.0f);
        glRotatef(-hurtFraction * 14.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(attackedAtYaw, 0.0f, 1.0f, 0.0f);
    }
}

void EntityRenderer::setupViewBobbing(const float partialTicks) {
    if (mc.options == nullptr || mc.options->thirdPersonView || mc.thePlayer == nullptr) {
        return;
    }

    const float walk = mc.thePlayer->prevDistanceWalkedModified +
                       (mc.thePlayer->distanceWalkedModified - mc.thePlayer->prevDistanceWalkedModified) * partialTicks;
    const float cameraYaw = mc.thePlayer->prevCameraYaw +
                            (mc.thePlayer->cameraYaw - mc.thePlayer->prevCameraYaw) * partialTicks;
    const float cameraPitch = mc.thePlayer->prevCameraPitch +
                              (mc.thePlayer->cameraPitch - mc.thePlayer->prevCameraPitch) * partialTicks;

    glTranslatef(MathHelper::sin(walk * static_cast<float>(std::acos(-1.0))) * cameraYaw * 0.5f,
                 -std::fabs(MathHelper::cos(walk * static_cast<float>(std::acos(-1.0))) * cameraYaw), 0.0f);
    glRotatef(MathHelper::sin(walk * static_cast<float>(std::acos(-1.0))) * cameraYaw * 3.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(std::fabs(MathHelper::cos(walk * static_cast<float>(std::acos(-1.0)) + 0.2f) * cameraYaw) * 5.0f,
              1.0f, 0.0f, 0.0f);
    glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
}

void EntityRenderer::orientCamera(const float partialTicks) {
    if (mc.thePlayer == nullptr) {
        return;
    }

    const double cameraX = mc.thePlayer->prevPosX + (mc.thePlayer->posX - mc.thePlayer->prevPosX) * partialTicks;
    const double cameraY = mc.thePlayer->prevPosY + (mc.thePlayer->posY - mc.thePlayer->prevPosY) * partialTicks;
    const double cameraZ = mc.thePlayer->prevPosZ + (mc.thePlayer->posZ - mc.thePlayer->prevPosZ) * partialTicks;
    if (mc.options != nullptr && mc.options->thirdPersonView) {
        double cameraDistance = 4.0;
        const float yaw = mc.thePlayer->rotationYaw;
        const float pitch = mc.thePlayer->rotationPitch;
        const double radians = std::acos(-1.0) / 180.0;
        const double xOffset = -std::sin(static_cast<double>(yaw) * radians) *
                               std::cos(static_cast<double>(pitch) * radians) * cameraDistance;
        const double zOffset = std::cos(static_cast<double>(yaw) * radians) *
                               std::cos(static_cast<double>(pitch) * radians) * cameraDistance;
        const double yOffset = -std::sin(static_cast<double>(pitch) * radians) * cameraDistance;

        for (int_t i = 0; i < 8; ++i) {
            const float sampleX = static_cast<float>((i & 1) * 2 - 1) * 0.1f;
            const float sampleY = static_cast<float>((i >> 1 & 1) * 2 - 1) * 0.1f;
            const float sampleZ = static_cast<float>((i >> 2 & 1) * 2 - 1) * 0.1f;
            Vec3D start(cameraX + static_cast<double>(sampleX),
                        cameraY + static_cast<double>(sampleY),
                        cameraZ + static_cast<double>(sampleZ));
            Vec3D end(cameraX - xOffset + static_cast<double>(sampleX) + static_cast<double>(sampleZ),
                      cameraY - yOffset + static_cast<double>(sampleY),
                      cameraZ - zOffset + static_cast<double>(sampleZ));
            MovingObjectPosition hit = mc.theWorld->rayTraceBlocks(start, end);
            if (hit.hitVec != nullptr) {
                const double distance = hit.hitVec->distanceTo(Vec3D(cameraX, cameraY, cameraZ));
                if (distance < cameraDistance) {
                    cameraDistance = distance;
                }
            }
        }

        glRotatef(mc.thePlayer->rotationPitch - mc.thePlayer->prevRotationPitch, 1.0f, 0.0f, 0.0f);
        glRotatef(mc.thePlayer->rotationYaw - mc.thePlayer->prevRotationYaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, static_cast<float>(-cameraDistance));
        glRotatef(mc.thePlayer->prevRotationYaw - mc.thePlayer->rotationYaw, 0.0f, 1.0f, 0.0f);
        glRotatef(mc.thePlayer->prevRotationPitch - mc.thePlayer->rotationPitch, 1.0f, 0.0f, 0.0f);
    } else {
        glTranslatef(0.0f, 0.0f, -0.1f);
    }
    glRotatef(mc.thePlayer->prevRotationPitch + (mc.thePlayer->rotationPitch - mc.thePlayer->prevRotationPitch) *
              partialTicks, 1.0f, 0.0f, 0.0f);
    glRotatef(mc.thePlayer->prevRotationYaw + (mc.thePlayer->rotationYaw - mc.thePlayer->prevRotationYaw) *
              partialTicks + 180.0f, 0.0f, 1.0f, 0.0f);
}

void EntityRenderer::renderHand(const float partialTicks, const int_t pass) {
    glLoadIdentity();
    if (mc.options->anaglyph) {
        glTranslatef(static_cast<float>(pass * 2 - 1) * 0.1f, 0.0f, 0.0f);
    }

    glPushMatrix();
    hurtCameraEffect(partialTicks);
    if (mc.options->viewBobbing) {
        setupViewBobbing(partialTicks);
    }

    if (!mc.options->thirdPersonView && itemRenderer != nullptr) {
        itemRenderer->renderItemInFirstPerson(partialTicks);
    }

    glPopMatrix();
    if (!mc.options->thirdPersonView && itemRenderer != nullptr) {
        itemRenderer->renderOverlays(partialTicks);
        hurtCameraEffect(partialTicks);
    }

    if (mc.options->viewBobbing) {
        setupViewBobbing(partialTicks);
    }
}

void EntityRenderer::addRainParticles() {
}

void EntityRenderer::renderSnow(const float partialTicks) {
    (void) partialTicks;
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
    return mc.thePlayer->isInsideOfMaterial(material);
}
