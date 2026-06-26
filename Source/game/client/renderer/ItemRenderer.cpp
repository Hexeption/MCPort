//
// Created by Keir Davis on 25/06/2026.
//

#include "ItemRenderer.h"
#include <glad/glad.h>

#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif

#include <cmath>

#include "game/block/Block.h"
#include "game/block/Material.h"
#include "game/client/MathHelper.h"
#include "game/client/Minecraft.h"
#include "game/client/renderer/RenderHelper.h"
#include "game/client/renderer/Tessellator.h"
#include "game/entity/EntityPlayerSP.h"
#include "game/item/ItemStack.h"
#include "game/world/World.h"

ItemRenderer::ItemRenderer(Minecraft &minecraft) : mc(minecraft) {
}

void ItemRenderer::renderItem(ItemStack &stack) {
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    if (stack.itemID < 256 &&
        Block::blocksList[stack.itemID] != nullptr &&
        RenderBlocks::renderItemIn3d(Block::blocksList[stack.itemID]->getRenderType())) {
        glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/terrain.png"));
        renderBlocksInstance.renderBlockOnInventory(Block::blocksList[stack.itemID]);
    } else {
        if (stack.itemID < 256) {
            glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/terrain.png"));
        } else {
            glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/gui/items.png"));
        }

        Tessellator &t = Tessellator::instance;

        const float u0 = static_cast<float>(stack.getIconIndex() % 16 * 16) / 256.0f;
        const float u1 = static_cast<float>(stack.getIconIndex() % 16 * 16 + 16) / 256.0f;
        const float v0 = static_cast<float>(stack.getIconIndex() / 16 * 16) / 256.0f;
        const float v1 = static_cast<float>(stack.getIconIndex() / 16 * 16 + 16) / 256.0f;

        constexpr float size = 1.0f;
        constexpr float xOffset = 0.0f;
        constexpr float yOffset = 0.3f;
        constexpr float thickness = 1.0f / 16.0f;

        glEnable(GL_RESCALE_NORMAL);
        glTranslatef(-xOffset, -yOffset, 0.0f);
        glScalef(1.5f, 1.5f, 1.5f);
        glRotatef(50.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(335.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(-(15.0f / 16.0f), -(1.0f / 16.0f), 0.0f);

        t.startDrawingQuads();
        t.setNormal(0.0f, 0.0f, 1.0f);
        t.addVertexWithUV(0.0, 0.0, 0.0, u1, v1);
        t.addVertexWithUV(size, 0.0, 0.0, u0, v1);
        t.addVertexWithUV(size, 1.0, 0.0, u0, v0);
        t.addVertexWithUV(0.0, 1.0, 0.0, u1, v0);
        t.draw();

        t.startDrawingQuads();
        t.setNormal(0.0f, 0.0f, -1.0f);
        t.addVertexWithUV(0.0, 1.0, -thickness, u1, v0);
        t.addVertexWithUV(size, 1.0, -thickness, u0, v0);
        t.addVertexWithUV(size, 0.0, -thickness, u0, v1);
        t.addVertexWithUV(0.0, 0.0, -thickness, u1, v1);
        t.draw();

        t.startDrawingQuads();
        t.setNormal(-1.0f, 0.0f, 0.0f);
        for (int_t i = 0; i < 16; ++i) {
            const float p = static_cast<float>(i) / 16.0f;
            const float u = u1 + (u0 - u1) * p - 0.001953125f;
            const float x = size * p;
            t.addVertexWithUV(x, 0.0, -thickness, u, v1);
            t.addVertexWithUV(x, 0.0, 0.0, u, v1);
            t.addVertexWithUV(x, 1.0, 0.0, u, v0);
            t.addVertexWithUV(x, 1.0, -thickness, u, v0);
        }
        t.draw();

        t.startDrawingQuads();
        t.setNormal(1.0f, 0.0f, 0.0f);
        for (int_t i = 0; i < 16; ++i) {
            const float p = static_cast<float>(i) / 16.0f;
            const float u = u1 + (u0 - u1) * p - 0.001953125f;
            const float x = size * p + 1.0f / 16.0f;
            t.addVertexWithUV(x, 1.0, -thickness, u, v0);
            t.addVertexWithUV(x, 1.0, 0.0, u, v0);
            t.addVertexWithUV(x, 0.0, 0.0, u, v1);
            t.addVertexWithUV(x, 0.0, -thickness, u, v1);
        }
        t.draw();

        t.startDrawingQuads();
        t.setNormal(0.0f, 1.0f, 0.0f);
        for (int_t i = 0; i < 16; ++i) {
            const float p = static_cast<float>(i) / 16.0f;
            const float v = v1 + (v0 - v1) * p - 0.001953125f;
            const float y = size * p + 1.0f / 16.0f;
            t.addVertexWithUV(0.0, y, 0.0, u1, v);
            t.addVertexWithUV(size, y, 0.0, u0, v);
            t.addVertexWithUV(size, y, -thickness, u0, v);
            t.addVertexWithUV(0.0, y, -thickness, u1, v);
        }
        t.draw();

        t.startDrawingQuads();
        t.setNormal(0.0f, -1.0f, 0.0f);
        for (int_t i = 0; i < 16; ++i) {
            const float p = static_cast<float>(i) / 16.0f;
            const float v = v1 + (v0 - v1) * p - 0.001953125f;
            const float y = size * p;
            t.addVertexWithUV(size, y, 0.0, u0, v);
            t.addVertexWithUV(0.0, y, 0.0, u1, v);
            t.addVertexWithUV(0.0, y, -thickness, u1, v);
            t.addVertexWithUV(size, y, -thickness, u0, v);
        }
        t.draw();

        glDisable(GL_RESCALE_NORMAL);
    }

    glPopMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopAttrib();
}

void ItemRenderer::renderItemInFirstPerson(const float partialTicks) {
    static constexpr float PI = static_cast<float>(3.14159265358979323846);

    const float ep = prevEquippedProgress + (equippedProgress - prevEquippedProgress) * partialTicks;
    EntityPlayerSP &player = *mc.thePlayer;

    glPushMatrix();
    glRotatef(player.prevRotationPitch + (player.rotationPitch - player.prevRotationPitch) * partialTicks,
              1.0f, 0.0f, 0.0f);
    glRotatef(player.prevRotationYaw + (player.rotationYaw - player.prevRotationYaw) * partialTicks,
              0.0f, 1.0f, 0.0f);
    RenderHelper::enableStandardItemLighting();
    glPopMatrix();

    const float brightness = mc.theWorld->getBrightness(
        MathHelper::floor_double(player.posX),
        MathHelper::floor_double(player.posY),
        MathHelper::floor_double(player.posZ));
    glColor4f(brightness, brightness, brightness, 1.0f);

    if (itemToRender != nullptr) {
        glPushMatrix();
        constexpr float es = 0.8f;
        const float swing = player.getSwingProgress(partialTicks);
        const float swingY = MathHelper::sin(swing * PI);
        const float swingZ = MathHelper::sin(MathHelper::sqrt_float(swing) * PI);
        glTranslatef(-swingZ * 0.4f, MathHelper::sin(MathHelper::sqrt_float(swing) * PI * 2.0f) * 0.2f, -swingY * 0.2f);
        glTranslatef(0.7f * es, -0.65f * es - (1.0f - ep) * 0.6f, -0.9f * es);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glEnable(GL_RESCALE_NORMAL);
        const float swing2 = player.getSwingProgress(partialTicks);
        const float swingY2 = MathHelper::sin(swing2 * swing2 * PI);
        const float swingZ2 = MathHelper::sin(MathHelper::sqrt_float(swing2) * PI);
        glRotatef(-swingY2 * 20.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(-swingZ2 * 20.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(-swingZ2 * 80.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.4f, 0.4f, 0.4f);
        renderItem(*itemToRender);
        glPopMatrix();
    } else {
        glPushMatrix();
        constexpr float es = 0.8f;
        const float swing = player.getSwingProgress(partialTicks);
        const float swingY = MathHelper::sin(swing * PI);
        const float swingZ = MathHelper::sin(MathHelper::sqrt_float(swing) * PI);
        glTranslatef(-swingZ * 0.3f, MathHelper::sin(MathHelper::sqrt_float(swing) * PI * 2.0f) * 0.4f, -swingY * 0.4f);
        glTranslatef(0.8f * es, -(12.0f / 16.0f) * es - (1.0f - ep) * 0.6f, -0.9f * es);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glEnable(GL_RESCALE_NORMAL);
        const float swing2 = player.getSwingProgress(partialTicks);
        const float swingY2 = MathHelper::sin(swing2 * swing2 * PI);
        const float swingZ2 = MathHelper::sin(MathHelper::sqrt_float(swing2) * PI);
        glRotatef(swingZ2 * 70.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(-swingY2 * 20.0f, 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(player.getTexture()));
        glTranslatef(-1.0f, 3.6f, 3.5f);
        glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(200.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(5.6f, 0.0f, 0.0f);
        armModel.swingProgress = 0.0f;
        armModel.setRotationAngles(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f / 16.0f);
        armModel.bipedRightArm.render(1.0f / 16.0f);
        glPopMatrix();
    }

    glDisable(GL_RESCALE_NORMAL);
    RenderHelper::disableStandardItemLighting();
}

void ItemRenderer::renderOverlays(const float partialTicks) {
    glDisable(GL_ALPHA_TEST);

    if (mc.thePlayer->isInsideOfMaterial(Material::water)) {
        glBindTexture(GL_TEXTURE_2D, mc.renderEngine->getTexture(u"/water.png"));
        renderWarpedTextureOverlay(partialTicks);
    }

    glEnable(GL_ALPHA_TEST);
}

void ItemRenderer::updateEquippedItem() {
    prevEquippedProgress = equippedProgress;
    ItemStack *current = mc.thePlayer->inventory.getCurrentItem();
    constexpr float step = 0.4f;
    float target = (current == itemToRender) ? 1.0f : 0.0f;
    float delta = target - equippedProgress;
    if (delta < -step) delta = -step;
    if (delta > step) delta = step;
    equippedProgress += delta;
    if (equippedProgress < 0.1f) {
        itemToRender = current;
    }
}

void ItemRenderer::resetEquippedProgress() {
    equippedProgress = 0.0f;
    prevEquippedProgress = 0.0f;
}

void ItemRenderer::resetEquippedProgress2() {
    resetEquippedProgress();
}

void ItemRenderer::renderWarpedTextureOverlay(float partialTicks) {
    Tessellator var2 = Tessellator::instance;
    float var3 = mc.thePlayer->getBrightness(partialTicks);
    glColor4f(var3, var3, var3, 0.5F);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    float var4 = 4.0F;
    float var5 = -1.0F;
    float var6 = 1.0F;
    float var7 = -1.0F;
    float var8 = 1.0F;
    float var9 = -0.5F;
    float var10 = -mc.thePlayer->rotationYaw / 64.0F;
    float var11 = mc.thePlayer->rotationPitch / 64.0F;
    var2.startDrawingQuads();
    var2.addVertexWithUV(var5, var7, var9, (var4 + var10), (var4 + var11));
    var2.addVertexWithUV(var6, var7, var9, (0.0F + var10), (var4 + var11));
    var2.addVertexWithUV(var6, var8, var9, (0.0F + var10), (0.0F + var11));
    var2.addVertexWithUV(var5, var8, var9, (var4 + var10), (0.0F + var11));
    var2.draw();
    glPopMatrix();
    glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
    glDisable(GL_BLEND);
}
