//
// Created by Keir Davis on 26/06/2026.
//

#include "InventoryCrafting.h"

#include "CraftingInventoryCB.h"
#include "game/item/ItemStack.h"

InventoryCrafting::InventoryCrafting(CraftingInventoryCB &craftingInventory,
                                     std::array<std::optional<ItemStack>, 4> &stackList)
    : craftingInventory(&craftingInventory), stackList(&stackList), gridSize(static_cast<int_t>(stackList.size())) {
}

InventoryCrafting::InventoryCrafting(CraftingInventoryCB &cb, int_t width, int_t height)
    : craftingInventory(&cb), ownedSlots(static_cast<size_t>(width * height)), gridSize(width * height) {
}

int_t InventoryCrafting::getSizeInventory() {
    return gridSize;
}

ItemStack *InventoryCrafting::getStackInSlot(const int_t slot) {
    auto &storage = stackList ? (*stackList)[slot] : ownedSlots[slot];
    return slot >= 0 && slot < gridSize && storage.has_value() && storage->stackSize > 0
               ? &storage.value()
               : nullptr;
}

jstring InventoryCrafting::getInvName() {
    return u"Crafting";
}

std::optional<ItemStack> InventoryCrafting::decrStackSize(const int_t slot, const int_t amount) {
    if (slot < 0 || slot >= gridSize) {
        return std::nullopt;
    }

    auto &stack = stackList ? (*stackList)[slot] : ownedSlots[slot];
    if (!stack.has_value()) {
        return std::nullopt;
    }

    if (stack->stackSize <= amount) {
        std::optional<ItemStack> result = *stack;
        stack.reset();
        craftingInventory->onCraftMatrixChanged(*this);
        return result;
    }

    ItemStack result = stack->splitStack(amount);
    if (stack->stackSize == 0) {
        stack.reset();
    }
    craftingInventory->onCraftMatrixChanged(*this);
    return result;
}

void InventoryCrafting::setInventorySlotContents(const int_t slot, const ItemStack &stack) {
    if (slot < 0 || slot >= gridSize) {
        return;
    }

    auto &storage = stackList ? (*stackList)[slot] : ownedSlots[slot];
    storage = stack;
    craftingInventory->onCraftMatrixChanged(*this);
}

int_t InventoryCrafting::getInventoryStackLimit() {
    return 64;
}

void InventoryCrafting::onInventoryChanged() {
}
