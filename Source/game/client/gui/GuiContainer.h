//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_GUICONTAINER_H
#define MCPORT_GUICONTAINER_H

#include <memory>
#include <optional>
#include <vector>

#include <glad/glad.h>

#include "GuiScreen.h"
#include "game/client/renderer/RenderItem.h"
#include "slot/SlotInventory.h"

class GuiContainer : public GuiScreen {
protected:
    static RenderItem itemRenderer;

    virtual void drawGuiContainerForegroundLayer();

    virtual void drawGuiContainerBackgroundLayer(float partialTicks) = 0;

    void drawSlotInventory(SlotInventory &slot);

    SlotInventory *getSlotAtPosition(int_t mouseX, int_t mouseY);

    void drawItemTooltip(int_t mouseX, int_t mouseY, const ItemStack &stack);

    void keyTyped(char_t ch, int_t key) override;

public:
    int_t xSize = 176;
    int_t ySize = 166;
    std::vector<std::unique_ptr<SlotInventory> > inventorySlots;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;

    void mouseClicked(int_t mouseX, int_t mouseY, int_t mouseButton) override;

    void mouseMovedOrUp(int_t mouseX, int_t mouseY, int_t mouseButton) override;

    void onGuiClosed() override;

    bool doesGuiPauseGame() override;
};

#endif //MCPORT_GUICONTAINER_H
