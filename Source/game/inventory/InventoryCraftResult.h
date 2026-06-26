//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_INVENTORYCRAFTRESULT_H
#define MCPORT_INVENTORYCRAFTRESULT_H

#include <array>
#include <optional>

#include "IInventory.h"

class InventoryCraftResult final : public IInventory {
private:
    std::array<std::optional<ItemStack>, 1> stackResult{};

public:
    int_t getSizeInventory() override;

    ItemStack *getStackInSlot(int_t slot) override;

    jstring getInvName() override;

    std::optional<ItemStack> decrStackSize(int_t slot, int_t amount) override;

    void setInventorySlotContents(int_t slot, const ItemStack &stack) override;

    int_t getInventoryStackLimit() override;

    void onInventoryChanged() override;
};

#endif //MCPORT_INVENTORYCRAFTRESULT_H
