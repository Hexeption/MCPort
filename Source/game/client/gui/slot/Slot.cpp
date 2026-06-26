//
// Created by Keir Davis on 26/06/2026.
//

#include "Slot.h"

#include <optional>

#include "game/inventory/IInventory.h"
#include "game/item/ItemStack.h"

Slot::Slot(IInventory &inventory, const int_t slotIndex) : inventory(&inventory), slotIndex(slotIndex) {
}

void Slot::onPickupFromSlot() {
    onSlotChanged();
}

bool Slot::isItemValid(const ItemStack &) {
    return true;
}

ItemStack *Slot::getStack() {
    return inventory != nullptr ? inventory->getStackInSlot(slotIndex) : nullptr;
}

void Slot::putStack(const std::optional<ItemStack> &stack) {
    if (inventory == nullptr) {
        return;
    }

    if (stack.has_value()) {
        inventory->setInventorySlotContents(slotIndex, *stack);
    } else {
        inventory->decrStackSize(slotIndex, getStack() != nullptr ? getStack()->stackSize : 0);
    }
    onSlotChanged();
}

int_t Slot::getBackgroundIconIndex() {
    return -1;
}

void Slot::onSlotChanged() {
    if (inventory != nullptr) {
        inventory->onInventoryChanged();
    }
}
