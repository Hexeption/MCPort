//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_IINVENTORY_H
#define MCPORT_IINVENTORY_H

#include <optional>

#include "java/String.h"
#include "java/Type.h"
#include "game/item/ItemStack.h"

class IInventory {
public:
    virtual ~IInventory() = default;

    virtual int_t getSizeInventory() = 0;

    virtual ItemStack *getStackInSlot(int_t slot) = 0;

    virtual std::optional<ItemStack> decrStackSize(int_t slot, int_t amount) {
        (void) slot;
        (void) amount;
        return std::nullopt;
    }

    virtual void setInventorySlotContents(int_t slot, const ItemStack &stack) {
        (void) slot;
        (void) stack;
    }

    virtual bool canInteractWith() const {
        return true;
    }

    virtual jstring getInvName() = 0;

    virtual int_t getInventoryStackLimit() = 0;

    virtual void onInventoryChanged() {
    }
};

#endif //MCPORT_IINVENTORY_H
