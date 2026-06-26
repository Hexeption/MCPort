//
// Created by Keir Davis on 26/06/2026.
//

#include "CraftingInventoryCB.h"

#include "game/entity/EntityPlayer.h"

void CraftingInventoryCB::onCraftMatrixChanged(IInventory &) {
}

void CraftingInventoryCB::onCraftGuiClosed(EntityPlayer &player) {
    InventoryPlayer &inventory = player.inventory;
    if (inventory.draggedItemStack.has_value()) {
        player.dropPlayerItem(inventory.draggedItemStack.value());
    }
}
