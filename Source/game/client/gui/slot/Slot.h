//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_SLOT_H
#define MCPORT_SLOT_H

#include <optional>

#include "java/Type.h"

class IInventory;
class ItemStack;

class Slot {
public:
    IInventory *inventory = nullptr;
    int_t slotIndex = 0;

    Slot(IInventory &inventory, int_t slotIndex);

    virtual ~Slot() = default;

    virtual void onPickupFromSlot();

    virtual bool isItemValid(const ItemStack &stack);

    virtual ItemStack *getStack();

    virtual void putStack(const std::optional<ItemStack> &stack);

    virtual int_t getBackgroundIconIndex();

    virtual void onSlotChanged();
};

#endif //MCPORT_SLOT_H
