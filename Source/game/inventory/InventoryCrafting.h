//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_INVENTORYCRAFTING_H
#define MCPORT_INVENTORYCRAFTING_H

#include <array>
#include <optional>

#include "IInventory.h"

class CraftingInventoryCB;

class InventoryCrafting final : public IInventory {
private:
    CraftingInventoryCB *craftingInventory = nullptr;
    std::array<std::optional<ItemStack>, 4> *stackList = nullptr;
    int_t gridSize = 0;

public:
    InventoryCrafting(CraftingInventoryCB &craftingInventory, std::array<std::optional<ItemStack>, 4> &stackList);

    int_t getSizeInventory() override;

    ItemStack *getStackInSlot(int_t slot) override;

    jstring getInvName() override;

    std::optional<ItemStack> decrStackSize(int_t slot, int_t amount) override;

    void setInventorySlotContents(int_t slot, const ItemStack &stack) override;

    int_t getInventoryStackLimit() override;

    void onInventoryChanged() override;
};

#endif //MCPORT_INVENTORYCRAFTING_H
