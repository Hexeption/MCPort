//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_GUICRAFTING_H
#define MCPORT_GUICRAFTING_H

#include <memory>

#include "GuiContainer.h"
#include "game/inventory/CraftingInventoryWorkbenchCB.h"


class GuiCrafting final : public GuiContainer {
private:
    std::unique_ptr<CraftingInventoryWorkbenchCB> craftingInventory;

public:
    explicit GuiCrafting(InventoryPlayer &playerInventory);

    void onGuiClosed() override;

protected:
    void drawGuiContainerForegroundLayer() override;

    void drawGuiContainerBackgroundLayer(float partialTicks) override;
};

#endif //MCPORT_GUICRAFTING_H
