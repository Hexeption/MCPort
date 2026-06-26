//
// Created by Keir Davis on 26/06/2026.
//

#include "SlotArmor.h"

#include "game/item/ItemStack.h"

SlotArmor::SlotArmor(GuiInventory &guiInventory, GuiContainer &guiContainer, IInventory &inventory,
                     const int_t slotIndex, const int_t xDisplayPosition, const int_t yDisplayPosition,
                     const int_t armorType)
    : SlotInventory(guiContainer, inventory, slotIndex, xDisplayPosition, yDisplayPosition), armorType(armorType),
      guiInventory(&guiInventory) {
}

bool SlotArmor::isItemValid(const ItemStack &stack) {
    return false;
}

int_t SlotArmor::getBackgroundIconIndex() {
    return 15 + armorType * 16;
}
