//
// Created by Keir Davis on 26/06/2026.
//

#include "GuiInventory.h"

#include <glad/glad.h>

#include "slot/SlotArmor.h"
#include "slot/SlotCrafting.h"
#include "game/client/Minecraft.h"
#include "game/inventory/CraftingInventoryPlayerCB.h"
#include "game/inventory/InventoryCraftResult.h"
#include "game/inventory/InventoryCrafting.h"
#include "game/inventory/InventoryPlayer.h"
#include "lwjgl/Keyboard.h"
#include "lwjgl/Mouse.h"

void GuiInventory::initGui() {
    tmi.init(mc);
}

GuiInventory::GuiInventory(InventoryPlayer &inventory) : playerInventory(&inventory) {
    allowUserInput = true;
    craftingInventory = std::make_unique<CraftingInventoryPlayerCB>(inventory.craftingInventory);

    inventorySlots.emplace_back(std::make_unique<SlotCrafting>(*this, *craftingInventory->craftMatrix,
                                                               *craftingInventory->craftResult, 0, 144, 36));
    for (int_t row = 0; row < 2; ++row) {
        for (int_t col = 0; col < 2; ++col) {
            inventorySlots.emplace_back(std::make_unique<SlotInventory>(*this, *craftingInventory->craftMatrix,
                                                                        col + row * 2, 88 + col * 18,
                                                                        26 + row * 18));
        }
    }

    for (int_t slot = 0; slot < 4; ++slot) {
        inventorySlots.emplace_back(std::make_unique<SlotArmor>(*this, *this, inventory,
                                                                static_cast<int_t>(inventory.getSizeInventory()) - 1 -
                                                                slot,
                                                                8, 8 + slot * 18, slot));
    }

    for (int_t row = 0; row < 3; ++row) {
        for (int_t col = 0; col < 9; ++col) {
            inventorySlots.emplace_back(std::make_unique<SlotInventory>(*this, inventory, col + (row + 1) * 9,
                                                                        8 + col * 18, 84 + row * 18));
        }
    }

    for (int_t slot = 0; slot < 9; ++slot) {
        inventorySlots.emplace_back(std::make_unique<SlotInventory>(*this, inventory, slot, 8 + slot * 18, 142));
    }
}

void GuiInventory::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    GuiContainer::drawScreen(mouseX, mouseY, partialTicks);
    xSize_lo = static_cast<float>(mouseX);
    ySize_lo = static_cast<float>(mouseY);

    if (tmi.visible && mc != nullptr) {
        tmi.draw(*fontRenderer, *mc->renderEngine, mouseX, mouseY,
                 tmiPanelX(), height);
    }
}

void GuiInventory::handleMouseInput() {
    if (tmi.visible) {
        const int_t wheel = lwjgl::Mouse::getEventDWheel();
        if (wheel != 0) tmi.changePage(wheel);
    }
    GuiScreen::handleMouseInput();
}

void GuiInventory::mouseClicked(const int_t mouseX, const int_t mouseY, const int_t mouseButton) {
    if (tmi.visible && tmi.mouseClicked(mouseX, mouseY, mouseButton, tmiPanelX())) {
        return;
    }
    GuiContainer::mouseClicked(mouseX, mouseY, mouseButton);
}

void GuiInventory::keyTyped(const char_t ch, const int_t key) {
    if (key == lwjgl::Keyboard::KEY_O) {
        tmi.toggle();
        return;
    }
    GuiContainer::keyTyped(ch, key);
}

bool GuiInventory::doesGuiPauseGame() {
    return false;
}

void GuiInventory::drawGuiContainerForegroundLayer() {
    if (fontRenderer != nullptr) {
        fontRenderer->drawString(u"Crafting", 86, 16, 4210752);
    }
}

void GuiInventory::drawGuiContainerBackgroundLayer(const float) {
    const int_t texture = mc->renderEngine->getTexture(u"/gui/inventory.png");
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    const int_t left = (width - xSize) / 2;
    const int_t top = (height - ySize) / 2;
    drawTexturedModalRect(left, top, 0, 0, xSize, ySize);
}
