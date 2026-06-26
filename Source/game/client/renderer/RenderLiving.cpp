//
// Created by Keir Davis on 26/06/2026.
//

#include "RenderLiving.h"

#include <glad/glad.h>

#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif

#include "game/client/MathHelper.h"
#include "game/entity/EntityLiving.h"

RenderLiving::RenderLiving(ModelBase *model, const float shadowSizeIn) {
    mainModel = model;
    shadowSize = shadowSizeIn;
}

void RenderLiving::setRenderPassModel(ModelBase *model) {
    renderPassModel = model;
}

void RenderLiving::doRenderLiving(EntityLiving &entity, double x, double y, double z,
                                   float yaw, float partialTicks) {
    glPushMatrix();
    glDisable(GL_CULL_FACE);

    mainModel->swingProgress = renderSwingProgress(entity, partialTicks);
    mainModel->isRiding = entity.ridingEntity != nullptr;
    if (renderPassModel != nullptr) {
        renderPassModel->isRiding = mainModel->isRiding;
    }

    const float renderYaw = entity.prevRenderYawOffset +
                            (entity.renderYawOffset - entity.prevRenderYawOffset) * partialTicks;
    const float rotYaw = entity.prevRotationYaw +
                         (entity.rotationYaw - entity.prevRotationYaw) * partialTicks;
    const float rotPitch = entity.prevRotationPitch +
                           (entity.rotationPitch - entity.prevRotationPitch) * partialTicks;

    glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

    const float ageInTicks = handleRotationFloat(entity, partialTicks);
    glRotatef(180.0f - renderYaw, 0.0f, 1.0f, 0.0f);

    if (entity.deathTime > 0) {
        float deathRot = (static_cast<float>(entity.deathTime) + partialTicks - 1.0f) / 20.0f * 1.6f;
        deathRot = MathHelper::sqrt_float(deathRot);
        if (deathRot > 1.0f) deathRot = 1.0f;
        glRotatef(deathRot * getDeathMaxRotation(entity), 0.0f, 0.0f, 1.0f);
    }

    constexpr float scale = 1.0f / 16.0f;
    glEnable(GL_RESCALE_NORMAL);
    glScalef(-1.0f, -1.0f, 1.0f);
    preRenderCallback(entity, partialTicks);
    glTranslatef(0.0f, -24.0f * scale - 0.0078125f, 0.0f);

    const float limbYaw = entity.prevLimbYaw + (entity.limbYaw - entity.prevLimbYaw) * partialTicks;
    float limbSwing = entity.limbSwing - entity.limbYaw * (1.0f - partialTicks);
    if (limbYaw > 1.0f) limbSwing /= limbYaw;

    loadDownloadableImageTexture(entity.skinUrl, entity.getTexture());

    glEnable(GL_ALPHA_TEST);
    mainModel->render(limbSwing, limbYaw > 1.0f ? 1.0f : limbYaw,
                      ageInTicks, rotYaw - renderYaw, rotPitch, scale);

    for (int pass = 0; pass < 4; ++pass) {
        if (shouldRenderPass(entity, pass)) {
            renderPassModel->render(limbSwing, limbYaw > 1.0f ? 1.0f : limbYaw,
                                    ageInTicks, rotYaw - renderYaw, rotPitch, scale);
            glDisable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
        }
    }

    renderEquippedItems(entity, partialTicks);

    const float brightness = entity.getBrightness(partialTicks);
    const int colorMult = getColorMultiplier(entity, brightness, partialTicks);
    const bool showOverlay = ((colorMult >> 24) & 0xFF) > 0 ||
                             entity.hurtTime > 0 || entity.deathTime > 0;

    if (showOverlay) {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_EQUAL);

        if (entity.hurtTime > 0 || entity.deathTime > 0) {
            glColor4f(brightness, 0.0f, 0.0f, 0.4f);
            mainModel->render(limbSwing, limbYaw > 1.0f ? 1.0f : limbYaw,
                              ageInTicks, rotYaw - renderYaw, rotPitch, scale);
        }

        if (((colorMult >> 24) & 0xFF) > 0) {
            const float r = static_cast<float>((colorMult >> 16) & 0xFF) / 255.0f;
            const float g = static_cast<float>((colorMult >> 8) & 0xFF) / 255.0f;
            const float b = static_cast<float>(colorMult & 0xFF) / 255.0f;
            const float a = static_cast<float>((colorMult >> 24) & 0xFF) / 255.0f;
            glColor4f(r, g, b, a);
            mainModel->render(limbSwing, limbYaw > 1.0f ? 1.0f : limbYaw,
                              ageInTicks, rotYaw - renderYaw, rotPitch, scale);
        }

        glDepthFunc(GL_LEQUAL);
        glDisable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_TEXTURE_2D);
    }

    glDisable(GL_RESCALE_NORMAL);
    glEnable(GL_CULL_FACE);
    glPopMatrix();
}

void RenderLiving::doRender(Entity &entity, double x, double y, double z,
                             float yaw, float partialTicks) {
    doRenderLiving(dynamic_cast<EntityLiving &>(entity), x, y, z, yaw, partialTicks);
}

float RenderLiving::renderSwingProgress(EntityLiving &entity, float partialTicks) {
    return entity.getSwingProgress(partialTicks);
}

float RenderLiving::handleRotationFloat(EntityLiving &entity, float partialTicks) {
    return static_cast<float>(entity.ticksExisted) + partialTicks;
}

void RenderLiving::renderEquippedItems(EntityLiving &, float) {}

bool RenderLiving::shouldRenderPass(EntityLiving &, int) { return false; }

float RenderLiving::getDeathMaxRotation(EntityLiving &) { return 90.0f; }

int RenderLiving::getColorMultiplier(EntityLiving &, float, float) { return 0; }

void RenderLiving::preRenderCallback(EntityLiving &, float) {}
