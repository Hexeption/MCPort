//
// Created by Keir Davis on 26/06/2026.
//

#include "GuiCrafting.h"

#include <glad/glad.h>

#include "slot/SlotCrafting.h"
#include "slot/SlotInventory.h"
#include "game/client/Minecraft.h"
#include "game/inventory/InventoryCraftResult.h"
#include "game/inventory/InventoryCrafting.h"
#include "game/inventory/InventoryPlayer.h"

GuiCrafting::GuiCrafting(InventoryPlayer &playerInventory) {
    craftingInventory = std::make_unique<CraftingInventoryWorkbenchCB>();

    inventorySlots.emplace_back(std::make_unique<SlotCrafting>(*this, *craftingInventory->craftMatrix,
                                                               *craftingInventory->craftResult, 0, 124, 35));

    for (int_t row = 0; row < 3; ++row) {
        for (int_t col = 0; col < 3; ++col) {
            inventorySlots.emplace_back(std::make_unique<SlotInventory>(*this, *craftingInventory->craftMatrix,
                                                                        col + row * 3,
                                                                        30 + col * 18, 17 + row * 18));
        }
    }

    for (int_t row = 0; row < 3; ++row) {
        for (int_t col = 0; col < 9; ++col) {
            inventorySlots.emplace_back(std::make_unique<SlotInventory>(*this, playerInventory,
                                                                        col + (row + 1) * 9,
                                                                        8 + col * 18, 84 + row * 18));
        }
    }

    for (int_t slot = 0; slot < 9; ++slot) {
        inventorySlots.emplace_back(std::make_unique<SlotInventory>(*this, playerInventory,
                                                                    slot, 8 + slot * 18, 142));
    }
}

void GuiCrafting::onGuiClosed() {
    GuiContainer::onGuiClosed();
    if (mc != nullptr) {
        craftingInventory->onCraftGuiClosed(*mc->thePlayer);
    }
}

void GuiCrafting::drawGuiContainerForegroundLayer() {
    if (fontRenderer != nullptr) {
        fontRenderer->drawString(u"Crafting", 28, 6, 4210752);
        fontRenderer->drawString(u"Inventory", 8, ySize - 96 + 2, 4210752);
    }
}

void GuiCrafting::drawGuiContainerBackgroundLayer(const float) {
    const int_t texture = mc->renderEngine->getTexture(u"/gui/crafting.png");
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    const int_t left = (width - xSize) / 2;
    const int_t top = (height - ySize) / 2;
    drawTexturedModalRect(left, top, 0, 0, xSize, ySize);
}
