//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_RENDERITEM_H
#define MCPORT_RENDERITEM_H


#include "Render.h"
#include "game/client/renderer/RenderBlocks.h"
#include "java/Random.h"

class Entity;
class EntityItem;
class FontRenderer;
class ItemStack;
class RenderEngine;
class Tessellator;
class AxisAlignedBB;

class RenderItem : public Render {
private:
    RenderBlocks itemRenderBlocks;
    Random random;

    void renderQuad(Tessellator &t, int_t x, int_t y, int_t width, int_t height, int_t color);

public:
    RenderItem();

    void doRenderItem(EntityItem &entity, double x, double y, double z, float yaw, float partialTicks);

    void renderItemIntoGUI(FontRenderer &font, RenderEngine &engine, ItemStack *stack, int_t x, int_t y);

    void renderItemOverlayIntoGUI(FontRenderer &font, RenderEngine &engine, ItemStack *stack, int_t x, int_t y);

    void renderIcon(int_t x, int_t y, int_t u, int_t v, int_t width, int_t height);

    static void renderAABB(const AxisAlignedBB &box);

    void doRender(Entity &entity, double x, double y, double z, float yaw, float partialTicks) override;
};


#endif //MCPORT_RENDERITEM_H
