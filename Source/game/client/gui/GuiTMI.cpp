//
// Created by Keir Davis on 26/06/2026.
//

#include "GuiTMI.h"

#include <glad/glad.h>

#include "game/block/Block.h"
#include "game/client/Minecraft.h"
#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/RenderHelper.h"
#include "game/client/renderer/RenderItem.h"
#include "game/inventory/InventoryPlayer.h"
#include "game/item/Item.h"
#include "game/item/ItemStack.h"
#include "java/String.h"
#include "lwjgl/Mouse.h"

RenderItem GuiTMI::itemRenderer{};
bool GuiTMI::sharedVisible = false;

void GuiTMI::init(Minecraft *mc_) {
    mc = mc_;
    itemIds.clear();
    page = 0;
    visible = sharedVisible;

    for (int_t id = 1; id < 256; ++id) {
        if (id < static_cast<int_t>(Block::blocksList.size()) && Block::blocksList[id] != nullptr) {
            itemIds.push_back(id);
        }
    }
    for (int_t id = 256; id < 512; ++id) {
        if (id < static_cast<int_t>(Item::itemsList.size()) && Item::itemsList[id] != nullptr) {
            itemIds.push_back(id);
        }
    }
}

void GuiTMI::toggle() {
    visible = !visible;
    sharedVisible = visible;
}

int_t GuiTMI::stackCountFor(const int_t id) {
    if (id >= 256 && id < static_cast<int_t>(Item::itemsList.size()) && Item::itemsList[id] != nullptr) {
        return Item::itemsList[id]->maxStackSize;
    }
    return 64;
}

void GuiTMI::giveItem(const int_t id, const int_t count) {
    if (mc == nullptr || mc->thePlayer == nullptr) return;
    ItemStack stack(id, count, 0);
    mc->thePlayer->inventory.addItemStackToInventory(stack);
}

void GuiTMI::draw(FontRenderer &font, RenderEngine &engine,
                  const int_t mouseX, const int_t mouseY,
                  const int_t panelX, const int_t screenH) {
    lastScreenH = screenH;
    drawRect(panelX, 0, panelX + PANEL_W, screenH, 0xC0000000);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    RenderHelper::enableStandardItemLighting();
    glPopMatrix();
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_NORMALIZE);

    const int_t ipp = itemsPerPage(screenH);
    const int_t rows = (screenH - FOOTER) / CELL;
    const int_t startIdx = page * ipp;

    for (int_t row = 0; row < rows; ++row) {
        for (int_t col = 0; col < COLS; ++col) {
            const int_t idx = startIdx + row * COLS + col;
            if (idx >= static_cast<int_t>(itemIds.size())) break;

            const int_t x = panelX + 1 + col * CELL;
            const int_t y = row * CELL;
            const int_t id = itemIds[idx];

            glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
            ItemStack stack(id, 1, 0);
            itemRenderer.renderItemIntoGUI(font, engine, &stack, x + 1, y + 1);
            itemRenderer.renderItemOverlayIntoGUI(font, engine, nullptr, x + 1, y + 1);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glPopAttrib();

            if (mouseX >= x && mouseX < x + CELL && mouseY >= y && mouseY < y + CELL) {
                glDisable(GL_LIGHTING);
                glDisable(GL_DEPTH_TEST);
                drawRect(x, y, x + CELL, y + CELL, 0x80FFFFFF);
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_LIGHTING);
            }
        }
    }

    glDisable(GL_RESCALE_NORMAL);
    glDisable(GL_NORMALIZE);
    RenderHelper::disableStandardItemLighting();

    const int_t tp = totalPages(screenH);
    const jstring pageText = u"< " + String::toString(page + 1) + u" / " + String::toString(tp) + u" >";
    const int_t textX = panelX + (PANEL_W - font.getStringWidth(pageText)) / 2;
    font.drawStringWithShadow(pageText, textX, screenH - FOOTER + 2, 0xFFFFFF);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

bool GuiTMI::mouseClicked(const int_t mouseX, const int_t mouseY, const int_t button,
                           const int_t panelX) {
    if (mouseX < panelX || mouseX >= panelX + PANEL_W) return false;
    if (button != 0 && button != 1) return true;

    const int_t col = (mouseX - panelX - 1) / CELL;
    const int_t row = mouseY / CELL;
    if (col < 0 || col >= COLS || row < 0) return true;

    const int_t ipp = itemsPerPage(lastScreenH);
    const int_t idx = page * ipp + row * COLS + col;
    if (idx >= 0 && idx < static_cast<int_t>(itemIds.size())) {
        const int_t id = itemIds[idx];
        const int_t count = button == 1 ? 1 : stackCountFor(id);
        giveItem(id, count);
    }
    return true;
}

void GuiTMI::changePage(const int_t delta) {
    const int_t tp = totalPages(lastScreenH);
    page -= delta / 120;
    if (page < 0) page = 0;
    if (page >= tp) page = tp - 1;
}
