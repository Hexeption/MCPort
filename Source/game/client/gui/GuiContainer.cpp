//
// Created by Keir Davis on 26/06/2026.
//

#include "GuiContainer.h"

#include <algorithm>

#include "slot/SlotInventory.h"
#include "game/client/Minecraft.h"
#include "game/client/renderer/RenderHelper.h"
#include "game/client/renderer/RenderItem.h"
#include "game/entity/EntityPlayer.h"
#include "game/item/ItemStack.h"
#include "game/inventory/InventoryPlayer.h"
#include "lwjgl/Keyboard.h"

RenderItem GuiContainer::itemRenderer{};

void GuiContainer::drawGuiContainerForegroundLayer() {
}

void GuiContainer::drawSlotInventory(SlotInventory &slot) {
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    ItemStack *stack = slot.getStack();
    if (stack == nullptr) {
        const int_t background = slot.getBackgroundIconIndex();
        if (background >= 0) {
            glDisable(GL_LIGHTING);
            glBindTexture(GL_TEXTURE_2D, mc->renderEngine->getTexture(u"/gui/items.png"));
            drawTexturedModalRect(slot.xDisplayPosition, slot.yDisplayPosition, background % 16 * 16,
                                  background / 16 * 16, 16, 16);
            glEnable(GL_LIGHTING);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
        glPopAttrib();
        return;
    }

    itemRenderer.renderItemIntoGUI(*fontRenderer, *mc->renderEngine, stack, slot.xDisplayPosition,
                                   slot.yDisplayPosition);
    itemRenderer.renderItemOverlayIntoGUI(*fontRenderer, *mc->renderEngine, stack, slot.xDisplayPosition,
                                          slot.yDisplayPosition);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopAttrib();
}

SlotInventory *GuiContainer::getSlotAtPosition(const int_t mouseX, const int_t mouseY) {
    for (auto &slot: inventorySlots) {
        if (slot->getIsMouseOverSlot(mouseX, mouseY)) {
            return slot.get();
        }
    }

    return nullptr;
}

void GuiContainer::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    drawDefaultBackground();
    const int_t left = (width - xSize) / 2;
    const int_t top = (height - ySize) / 2;

    drawGuiContainerBackgroundLayer(partialTicks);

    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    RenderHelper::enableStandardItemLighting();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(static_cast<float>(left), static_cast<float>(top), 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_NORMALIZE);

    for (auto &slot: inventorySlots) {
        drawSlotInventory(*slot);
        if (slot->getIsMouseOverSlot(mouseX, mouseY)) {
            glDisable(GL_LIGHTING);
            glDisable(GL_DEPTH_TEST);
            drawGradientRect(slot->xDisplayPosition, slot->yDisplayPosition, slot->xDisplayPosition + 16,
                             slot->yDisplayPosition + 16, -2130706433, -2130706433);
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
        }
    }

    if (mc != nullptr && mc->thePlayer != nullptr && mc->thePlayer->inventory.draggedItemStack.has_value()) {
        ItemStack dragged = mc->thePlayer->inventory.draggedItemStack.value();
        glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
        glTranslatef(0.0f, 0.0f, 32.0f);
        itemRenderer.renderItemIntoGUI(*fontRenderer, *mc->renderEngine, &dragged, mouseX - left - 8, mouseY - top - 8);
        itemRenderer.renderItemOverlayIntoGUI(*fontRenderer, *mc->renderEngine, &dragged, mouseX - left - 8,
                                              mouseY - top - 8);
        glPopAttrib();
    }

    glDisable(GL_RESCALE_NORMAL);
    glDisable(GL_NORMALIZE);
    RenderHelper::disableStandardItemLighting();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    drawGuiContainerForegroundLayer();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopAttrib();
}

void GuiContainer::mouseClicked(const int_t mouseX, const int_t mouseY, const int_t mouseButton) {
    if (mouseButton != 0 && mouseButton != 1) {
        return;
    }

    SlotInventory *slot = getSlotAtPosition(mouseX, mouseY);
    if (mc == nullptr || mc->thePlayer == nullptr) {
        return;
    }

    InventoryPlayer &inventory = mc->thePlayer->inventory;
    std::optional<ItemStack> &dragged = inventory.draggedItemStack;

    if (slot != nullptr) {
        ItemStack *slotStack = slot->getStack();
        if (slotStack != nullptr || dragged.has_value()) {
            if (slotStack != nullptr && !dragged.has_value()) {
                int_t amount = mouseButton == 0 ? slotStack->stackSize : (slotStack->stackSize + 1) / 2;
                dragged = slot->inventory->decrStackSize(slot->slotIndex, amount);
                slot->onPickupFromSlot();
            } else if (slotStack == nullptr && dragged.has_value() && slot->isItemValid(*dragged)) {
                int_t amount = mouseButton == 0 ? dragged->stackSize : 1;
                amount = std::min(amount, slot->inventory->getInventoryStackLimit());
                slot->putStack(dragged->splitStack(amount));
                if (dragged->stackSize == 0) {
                    dragged.reset();
                }
            } else if (slotStack != nullptr && dragged.has_value()) {
                if (slot->isItemValid(*dragged)) {
                    if (slotStack->itemID != dragged->itemID) {
                        if (dragged->stackSize <= slot->inventory->getInventoryStackLimit()) {
                            std::optional<ItemStack> oldStack = *slotStack;
                            slot->putStack(dragged);
                            dragged = oldStack;
                        }
                    } else if (mouseButton == 0) {
                        int_t amount = dragged->stackSize;
                        amount = std::min(amount, slot->inventory->getInventoryStackLimit() - slotStack->stackSize);
                        amount = std::min(amount, dragged->getMaxStackSize() - slotStack->stackSize);
                        if (amount > 0) {
                            dragged->splitStack(amount);
                            if (dragged->stackSize == 0) {
                                dragged.reset();
                            }
                            slotStack->stackSize += amount;
                        }
                    } else if (mouseButton == 1) {
                        int_t amount = 1;
                        amount = std::min(amount, slot->inventory->getInventoryStackLimit() - slotStack->stackSize);
                        amount = std::min(amount, dragged->getMaxStackSize() - slotStack->stackSize);
                        if (amount > 0) {
                            dragged->splitStack(amount);
                            if (dragged->stackSize == 0) {
                                dragged.reset();
                            }
                            slotStack->stackSize += amount;
                        }
                    }
                } else if (slotStack->itemID == dragged->itemID && dragged->getMaxStackSize() > 1) {
                    int_t amount = slotStack->stackSize;
                    if (amount > 0 && amount + dragged->stackSize <= dragged->getMaxStackSize()) {
                        dragged->stackSize += amount;
                        slot->inventory->decrStackSize(slot->slotIndex, amount);
                        slot->onPickupFromSlot();
                    }
                }
            }
        }
        slot->onSlotChanged();
        return;
    }

    const int_t left = (width - xSize) / 2;
    const int_t top = (height - ySize) / 2;
    if (dragged.has_value() && (mouseX < left || mouseY < top || mouseX >= left + xSize || mouseY >= top + xSize)) {
        if (mouseButton == 0) {
            mc->thePlayer->dropPlayerItem(dragged.value());
            dragged.reset();
        } else {
            ItemStack split = dragged->splitStack(1);
            mc->thePlayer->dropPlayerItem(split);
            if (dragged->stackSize == 0) {
                dragged.reset();
            }
        }
    }
}

void GuiContainer::mouseMovedOrUp(const int_t, const int_t, const int_t) {
}

void GuiContainer::keyTyped(const char_t ch, const int_t key) {
    if (key == lwjgl::Keyboard::KEY_ESCAPE ||
        (mc != nullptr && mc->options != nullptr && key == mc->options->keyBindInventory.key)) {
        mc->displayGuiScreen(nullptr);
        return;
    }

    GuiScreen::keyTyped(ch, key);
}

void GuiContainer::onGuiClosed() {
    if (mc != nullptr && mc->thePlayer != nullptr && mc->thePlayer->inventory.draggedItemStack.has_value()) {
        mc->thePlayer->dropPlayerItem(mc->thePlayer->inventory.draggedItemStack.value());
        mc->thePlayer->inventory.draggedItemStack.reset();
    }
}

bool GuiContainer::doesGuiPauseGame() {
    return false;
}
