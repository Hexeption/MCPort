//
// Created by Keir Davis on 26/06/2026.
//

#include "SlotInventory.h"

#include "game/client/gui/GuiContainer.h"

SlotInventory::SlotInventory(GuiContainer &guiContainer, IInventory &inventory, const int_t slotIndex,
                             const int_t xDisplayPosition, const int_t yDisplayPosition)
    : Slot(inventory, slotIndex), guiContainer(&guiContainer), xDisplayPosition(xDisplayPosition),
      yDisplayPosition(yDisplayPosition) {
}

bool SlotInventory::getIsMouseOverSlot(const int_t mouseX, const int_t mouseY) const {
    if (guiContainer == nullptr) {
        return false;
    }

    int_t x = mouseX - (guiContainer->width - guiContainer->xSize) / 2;
    int_t y = mouseY - (guiContainer->height - guiContainer->ySize) / 2;
    return x >= xDisplayPosition - 1 && x < xDisplayPosition + 17 && y >= yDisplayPosition - 1 &&
           y < yDisplayPosition + 17;
}
