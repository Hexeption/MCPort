//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ITEMRENDERER_H
#define MCPORT_ITEMRENDERER_H


#include "game/client/renderer/RenderBlocks.h"
#include "game/client/renderer/model/ModelBiped.h"

class Minecraft;
class ItemStack;

class ItemRenderer {
private:
    Minecraft &mc;
    ItemStack *itemToRender = nullptr;
    float equippedProgress = 0.0f;
    float prevEquippedProgress = 0.0f;
    RenderBlocks renderBlocksInstance;
    ModelBiped armModel;

    void renderInsideOfBlock(float partialTicks, int_t texture);

    void renderWarpedTextureOverlay(float partialTicks);

    void renderFireInFirstPerson(float partialTicks);

public:
    explicit ItemRenderer(Minecraft &minecraft);

    void renderItem(ItemStack &stack);

    void renderItemInFirstPerson(float partialTicks);

    void renderOverlays(float partialTicks);

    void updateEquippedItem();

    void resetEquippedProgress();

    void resetEquippedProgress2();
};


#endif //MCPORT_ITEMRENDERER_H
