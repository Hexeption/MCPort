//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_SLOTARMOR_H
#define MCPORT_SLOTARMOR_H

#include "SlotInventory.h"

class GuiInventory;

class SlotArmor final : public SlotInventory {
private:
    int_t armorType = 0;
    GuiInventory *guiInventory = nullptr;

public:
    SlotArmor(GuiInventory &guiInventory, GuiContainer &guiContainer, IInventory &inventory, int_t slotIndex,
              int_t xDisplayPosition, int_t yDisplayPosition, int_t armorType);

    bool isItemValid(const ItemStack &stack) override;

    int_t getBackgroundIconIndex() override;
};

#endif //MCPORT_SLOTARMOR_H
