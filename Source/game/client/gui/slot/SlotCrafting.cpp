//
// Created by Keir Davis on 26/06/2026.
//

#include "SlotCrafting.h"

#include "game/inventory/IInventory.h"

SlotCrafting::SlotCrafting(GuiContainer &guiContainer, IInventory &craftMatrix, IInventory &craftResult,
                           const int_t slotIndex, const int_t xDisplayPosition, const int_t yDisplayPosition)
    : SlotInventory(guiContainer, craftResult, slotIndex, xDisplayPosition, yDisplayPosition),
      craftMatrix(&craftMatrix) {
}

bool SlotCrafting::isItemValid(const ItemStack &) {
    return false;
}

void SlotCrafting::onPickupFromSlot() {
    if (craftMatrix == nullptr) {
        return;
    }

    for (int_t slot = 0; slot < craftMatrix->getSizeInventory(); ++slot) {
        if (ItemStack *stack = craftMatrix->getStackInSlot(slot)) {
            craftMatrix->decrStackSize(slot, 1);
        }
    }
}
