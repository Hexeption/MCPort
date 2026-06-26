//
// Created by Keir Davis on 26/06/2026.
//

#include "CraftingInventoryWorkbenchCB.h"

#include "CraftingManager.h"
#include "InventoryCraftResult.h"
#include "InventoryCrafting.h"
#include "game/entity/EntityPlayer.h"
#include "game/item/ItemStack.h"

CraftingInventoryWorkbenchCB::CraftingInventoryWorkbenchCB() {
    craftMatrix = new InventoryCrafting(*this, 3, 3);
    craftResult = new InventoryCraftResult();
}

void CraftingInventoryWorkbenchCB::onCraftMatrixChanged(IInventory &) {
    std::vector<int_t> grid(9);

    for (int_t col = 0; col < 3; ++col) {
        for (int_t row = 0; row < 3; ++row) {
            int_t id = -1;
            if (ItemStack *stack = craftMatrix->getStackInSlot(col + row * 3)) {
                id = stack->itemID;
            }
            grid[col + row * 3] = id;
        }
    }

    auto result = CraftingManager::getInstance().findMatchingRecipe(grid);
    craftResult->setInventorySlotContents(0, result.value_or(ItemStack(0, 0)));
}

void CraftingInventoryWorkbenchCB::onCraftGuiClosed(EntityPlayer &player) {
    CraftingInventoryCB::onCraftGuiClosed(player);

    for (int_t slot = 0; slot < 9; ++slot) {
        if (ItemStack *stack = craftMatrix->getStackInSlot(slot)) {
            player.dropPlayerItem(*stack);
        }
    }
}
