//
// Created by Keir Davis on 26/06/2026.
//

#include "InventoryCraftResult.h"

int_t InventoryCraftResult::getSizeInventory() {
    return 1;
}

ItemStack *InventoryCraftResult::getStackInSlot(const int_t slot) {
    return slot == 0 && stackResult[0].has_value() && stackResult[0]->stackSize > 0 ? &stackResult[0].value() : nullptr;
}

jstring InventoryCraftResult::getInvName() {
    return u"Result";
}

std::optional<ItemStack> InventoryCraftResult::decrStackSize(const int_t slot, const int_t amount) {
    (void) amount;
    if (slot != 0 || !stackResult[0].has_value()) {
        return std::nullopt;
    }

    std::optional<ItemStack> result = stackResult[0];
    stackResult[0].reset();
    return result;
}

void InventoryCraftResult::setInventorySlotContents(const int_t slot, const ItemStack &stack) {
    if (slot == 0) {
        if (stack.stackSize <= 0) {
            stackResult[0].reset();
        } else {
            stackResult[0] = stack;
        }
    }
}

int_t InventoryCraftResult::getInventoryStackLimit() {
    return 64;
}

void InventoryCraftResult::onInventoryChanged() {
}
