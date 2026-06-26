//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_SLOTINVENTORY_H
#define MCPORT_SLOTINVENTORY_H

#include "Slot.h"

class GuiContainer;

class SlotInventory : public Slot {
protected:
    GuiContainer *guiContainer = nullptr;

public:
    int_t xDisplayPosition = 0;
    int_t yDisplayPosition = 0;

    SlotInventory(GuiContainer &guiContainer, IInventory &inventory, int_t slotIndex, int_t xDisplayPosition,
                  int_t yDisplayPosition);

    bool getIsMouseOverSlot(int_t mouseX, int_t mouseY) const;
};

#endif //MCPORT_SLOTINVENTORY_H
