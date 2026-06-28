//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_GUIINVENTORY_H
#define MCPORT_GUIINVENTORY_H

#include <memory>

#include "GuiContainer.h"
#include "GuiTMI.h"
#include "game/inventory/CraftingInventoryPlayerCB.h"


class GuiInventory final : public GuiContainer {
private:
    std::unique_ptr<CraftingInventoryPlayerCB> craftingInventory;
    InventoryPlayer *playerInventory = nullptr;
    float xSize_lo = 0.0f;
    float ySize_lo = 0.0f;

    GuiTMI tmi;

    int_t tmiPanelX() const { return width - GuiTMI::PANEL_W; }

public:
    explicit GuiInventory(InventoryPlayer &inventory);

    void initGui() override;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;

    void handleMouseInput() override;

    void keyTyped(char_t ch, int_t key) override;

    bool doesGuiPauseGame() override;

    void toggleTMI() { tmi.toggle(); }
    void showTMI() { tmi.show(); }

protected:
    void mouseClicked(int_t mouseX, int_t mouseY, int_t mouseButton) override;

    void drawGuiContainerForegroundLayer() override;

    void drawGuiContainerBackgroundLayer(float partialTicks) override;
};

#endif //MCPORT_GUIINVENTORY_H
