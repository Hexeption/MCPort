//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_GUIINVENTORY_H
#define MCPORT_GUIINVENTORY_H

#include <memory>

#include "GuiContainer.h"
#include "game/inventory/CraftingInventoryPlayerCB.h"

class InventoryPlayer;

class GuiInventory final : public GuiContainer {
private:
    std::unique_ptr<CraftingInventoryPlayerCB> craftingInventory;
    InventoryPlayer *playerInventory = nullptr;
    float xSize_lo = 0.0f;
    float ySize_lo = 0.0f;

public:
    explicit GuiInventory(InventoryPlayer &inventory);

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;

    void keyTyped(char_t ch, int_t key) override;

    bool doesGuiPauseGame() override;

protected:
    void drawGuiContainerForegroundLayer() override;

    void drawGuiContainerBackgroundLayer(float partialTicks) override;
};

#endif //MCPORT_GUIINVENTORY_H
