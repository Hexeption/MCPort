//
// Created by Keir Davis on 25/06/2026.
//

#include "RenderItem.h"
#include <glad/glad.h>

#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif

#include <iostream>
#include <ostream>

#include "game/block/Block.h"
#include "game/client/MathHelper.h"
#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/RenderBlocks.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/RenderManager.h"
#include "game/client/renderer/Tessellator.h"
#include "game/entity/Entity.h"
#include "game/entity/EntityItem.h"
#include "game/item/ItemStack.h"
#include "game/phys/AxisAlignedBB.h"
#include "java/String.h"

RenderItem::RenderItem() {
    shadowSize = 0.15f;
    shadowOpaque = 12.0f / 16.0f;
}

void RenderItem::doRenderItem(EntityItem &entity, double x, double y, double z, float, float partialTicks) {
    random.setSeed(187L);

    ItemStack &stack = entity.item;

    glPushMatrix();

    const float hover =
            MathHelper::sin((static_cast<float>(entity.age) + partialTicks) / 10.0f + entity.hoverStart) * 0.1f + 0.1f;
    const float spin =
            ((static_cast<float>(entity.age) + partialTicks) / 20.0f + entity.hoverStart) *
            (180.0f / 3.14159265358979323846f);

    int_t renderCount = 1;
    if (stack.stackSize > 1) renderCount = 2;
    if (stack.stackSize > 5) renderCount = 3;
    if (stack.stackSize > 20) renderCount = 4;

    glTranslatef(static_cast<float>(x), static_cast<float>(y) + hover, static_cast<float>(z));
    glEnable(GL_RESCALE_NORMAL);

    if (stack.itemID < 256 && Block::blocksList[stack.itemID] != nullptr && RenderBlocks::renderItemIn3d(
            Block::blocksList[stack.itemID]->getRenderType())) {
        glRotatef(spin, 0.0f, 1.0f, 0.0f);
        loadTexture(u"/terrain.png");

        float scale = 0.25f;
        if (!Block::blocksList[stack.itemID]->renderAsNormalBlock()) {
            scale = 0.5f;
        }

        glScalef(scale, scale, scale);

        for (int_t i = 0; i < renderCount; ++i) {
            glPushMatrix();

            if (i > 0) {
                const float ox = (random.nextFloat() * 2.0f - 1.0f) * 0.2f / scale;
                const float oy = (random.nextFloat() * 2.0f - 1.0f) * 0.2f / scale;
                const float oz = (random.nextFloat() * 2.0f - 1.0f) * 0.2f / scale;
                glTranslatef(ox, oy, oz);
            }

            itemRenderBlocks.renderBlockOnInventory(Block::blocksList[stack.itemID]);
            glPopMatrix();
        }
    } else {
        glScalef(0.5f, 0.5f, 0.5f);

        const int_t icon = stack.getIconIndex();

        if (stack.itemID < 256) {
            loadTexture(u"/terrain.png");
        } else {
            loadTexture(u"/gui/items.png");
        }

        Tessellator &t = Tessellator::instance;

        const float u0 = static_cast<float>(icon % 16 * 16) / 256.0f;
        const float u1 = static_cast<float>(icon % 16 * 16 + 16) / 256.0f;
        const float v0 = static_cast<float>(icon / 16 * 16) / 256.0f;
        const float v1 = static_cast<float>(icon / 16 * 16 + 16) / 256.0f;

        constexpr float width = 1.0f;
        constexpr float halfWidth = 0.5f;
        constexpr float yOffset = 0.25f;

        for (int_t i = 0; i < renderCount; ++i) {
            glPushMatrix();

            if (i > 0) {
                const float ox = (random.nextFloat() * 2.0f - 1.0f) * 0.3f;
                const float oy = (random.nextFloat() * 2.0f - 1.0f) * 0.3f;
                const float oz = (random.nextFloat() * 2.0f - 1.0f) * 0.3f;
                glTranslatef(ox, oy, oz);
            }

            glRotatef(180.0f - renderManager->playerViewY, 0.0f, 1.0f, 0.0f);

            t.startDrawingQuads();
            t.setNormal(0.0f, 1.0f, 0.0f);
            t.addVertexWithUV(0.0f - halfWidth, 0.0f - yOffset, 0.0, u0, v1);
            t.addVertexWithUV(width - halfWidth, 0.0f - yOffset, 0.0, u1, v1);
            t.addVertexWithUV(width - halfWidth, 1.0f - yOffset, 0.0, u1, v0);
            t.addVertexWithUV(0.0f - halfWidth, 1.0f - yOffset, 0.0, u0, v0);
            t.draw();

            glPopMatrix();
        }
    }

    glDisable(GL_RESCALE_NORMAL);
    glPopMatrix();
}

void RenderItem::renderItemIntoGUI(FontRenderer &, RenderEngine &engine, ItemStack *stack, int_t x, int_t y) {
    if (stack == nullptr) {
        return;
    }

    if (stack->itemID < 256 &&
        Block::blocksList[stack->itemID] != nullptr &&
        RenderBlocks::renderItemIn3d(Block::blocksList[stack->itemID]->getRenderType())) {
        engine.bindTexture(engine.getTexture(u"/terrain.png"));

        Block *block = Block::blocksList[stack->itemID];

        glPushMatrix();
        glTranslatef(static_cast<float>(x - 2), static_cast<float>(y + 3), 0.0f);
        glScalef(10.0f, 10.0f, 10.0f);
        glTranslatef(1.0f, 0.5f, 8.0f);
        glRotatef(210.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        itemRenderBlocks.renderBlockOnInventory(block);
        glPopMatrix();
    } else if (stack->getIconIndex() >= 0) {
        glDisable(GL_LIGHTING);

        if (stack->itemID < 256) {
            engine.bindTexture(engine.getTexture(u"/terrain.png"));
        } else {
            engine.bindTexture(engine.getTexture(u"/gui/items.png"));
        }

        renderIcon(x, y, stack->getIconIndex() % 16 * 16, stack->getIconIndex() / 16 * 16, 16, 16);

        glEnable(GL_LIGHTING);
    }

    glEnable(GL_CULL_FACE);
}

void RenderItem::renderItemOverlayIntoGUI(FontRenderer &font, RenderEngine &, ItemStack *stack, int_t x, int_t y) {
    if (stack == nullptr) {
        return;
    }

    if (stack->stackSize > 1) {
        const jstring text = String::toString(stack->stackSize);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        font.drawStringWithShadow(
            text,
            x + 19 - 2 - font.getStringWidth(text),
            y + 6 + 3,
            16777215
        );

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }

    if (stack->itemDmg > 0) {
        const int_t barWidth = 13 - stack->itemDmg * 13 / stack->getMaxDamage();
        const int_t green = 255 - stack->itemDmg * 255 / stack->getMaxDamage();

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        Tessellator &t = Tessellator::instance;
        const int_t color = (255 - green) << 16 | green << 8;
        const int_t background = ((255 - green) / 4) << 16 | 16128;

        renderQuad(t, x + 2, y + 13, 13, 2, 0);
        renderQuad(t, x + 2, y + 13, 12, 1, background);
        renderQuad(t, x + 2, y + 13, barWidth, 1, color);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void RenderItem::renderQuad(Tessellator &t, int_t x, int_t y, int_t width, int_t height, int_t color) {
    t.startDrawingQuads();
    t.setColorOpaque_I(color);
    t.addVertex(x, y, 0.0);
    t.addVertex(x, y + height, 0.0);
    t.addVertex(x + width, y + height, 0.0);
    t.addVertex(x + width, y, 0.0);
    t.draw();
}

void RenderItem::renderIcon(int_t x, int_t y, int_t u, int_t v, int_t width, int_t height) {
    constexpr float z = 0.0f;
    constexpr float tex = 0.00390625f;

    Tessellator &t = Tessellator::instance;
    t.startDrawingQuads();
    t.addVertexWithUV(x, y + height, z, static_cast<float>(u) * tex, static_cast<float>(v + height) * tex);
    t.addVertexWithUV(x + width, y + height, z, static_cast<float>(u + width) * tex,
                      static_cast<float>(v + height) * tex);
    t.addVertexWithUV(x + width, y, z, static_cast<float>(u + width) * tex, static_cast<float>(v) * tex);
    t.addVertexWithUV(x, y, z, static_cast<float>(u) * tex, static_cast<float>(v) * tex);
    t.draw();
}

void RenderItem::renderAABB(const AxisAlignedBB &box) {
    Tessellator &t = Tessellator::instance;
    t.startDrawingQuads();
    t.addVertex(box.minX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.minX, box.minY, box.minZ);
    t.addVertex(box.minX, box.minY, box.maxZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.maxZ);
    t.addVertex(box.minX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);
    t.addVertex(box.minX, box.minY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.minX, box.maxY, box.minZ);
    t.addVertex(box.minX, box.minY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.maxZ);
    t.addVertex(box.minX, box.maxY, box.minZ);
    t.addVertex(box.minX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.minY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.minZ);
    t.addVertex(box.maxX, box.maxY, box.maxZ);
    t.addVertex(box.maxX, box.minY, box.maxZ);
    t.draw();
}

void RenderItem::doRender(Entity &entity, double x, double y, double z, float yaw, float partialTicks) {
    doRenderItem(static_cast<EntityItem &>(entity), x, y, z, yaw, partialTicks);
}
