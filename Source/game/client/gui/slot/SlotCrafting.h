//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_SLOTCRAFTING_H
#define MCPORT_SLOTCRAFTING_H

#include "SlotInventory.h"

class SlotCrafting final : public SlotInventory {
private:
    IInventory *craftMatrix = nullptr;

public:
    SlotCrafting(GuiContainer &guiContainer, IInventory &craftMatrix, IInventory &craftResult, int_t slotIndex,
                 int_t xDisplayPosition, int_t yDisplayPosition);

    bool isItemValid(const ItemStack &stack) override;

    void onPickupFromSlot() override;
};

#endif //MCPORT_SLOTCRAFTING_H
