//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CRAFTINGINVENTORYWORKBENCHCB_H
#define MCPORT_CRAFTINGINVENTORYWORKBENCHCB_H

#include "CraftingInventoryCB.h"

class InventoryCraftResult;
class InventoryCrafting;

class CraftingInventoryWorkbenchCB final : public CraftingInventoryCB {
public:
    InventoryCrafting *craftMatrix = nullptr;
    InventoryCraftResult *craftResult = nullptr;

    CraftingInventoryWorkbenchCB();

    void onCraftMatrixChanged(IInventory &inventory) override;

    void onCraftGuiClosed(EntityPlayer &player) override;
};

#endif //MCPORT_CRAFTINGINVENTORYWORKBENCHCB_H
