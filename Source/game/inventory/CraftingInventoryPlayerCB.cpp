//
// Created by Keir Davis on 26/06/2026.
//

#include "CraftingInventoryPlayerCB.h"

#include "InventoryCraftResult.h"
#include "InventoryCrafting.h"
#include "game/entity/EntityPlayer.h"
#include "game/item/ItemStack.h"

CraftingInventoryPlayerCB::CraftingInventoryPlayerCB(std::array<std::optional<ItemStack>, 4> &craftingInventory) {
    craftMatrix = new InventoryCrafting(*this, craftingInventory);
    craftResult = new InventoryCraftResult();
    onCraftMatrixChanged(*craftMatrix);
}

void CraftingInventoryPlayerCB::onCraftMatrixChanged(IInventory &) {
    if (craftResult != nullptr) {
        craftResult->setInventorySlotContents(0, ItemStack(0, 0));
    }
}

void CraftingInventoryPlayerCB::onCraftGuiClosed(EntityPlayer &player) {
    CraftingInventoryCB::onCraftGuiClosed(player);

    if (craftMatrix == nullptr) {
        return;
    }

    for (int_t slot = 0; slot < 9; ++slot) {
        if (ItemStack *stack = craftMatrix->getStackInSlot(slot)) {
            player.dropPlayerItem(*stack);
        }
    }
}
