//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CRAFTINGINVENTORYPLAYERCB_H
#define MCPORT_CRAFTINGINVENTORYPLAYERCB_H

#include <array>
#include <optional>

#include "CraftingInventoryCB.h"

class InventoryCraftResult;
class InventoryCrafting;
class ItemStack;
class EntityPlayer;

class CraftingInventoryPlayerCB final : public CraftingInventoryCB {
public:
    InventoryCrafting *craftMatrix = nullptr;
    InventoryCraftResult *craftResult = nullptr;

    explicit CraftingInventoryPlayerCB(std::array<std::optional<ItemStack>, 4> &craftingInventory);

    void onCraftMatrixChanged(IInventory &inventory) override;

    void onCraftGuiClosed(EntityPlayer &player) override;
};

#endif //MCPORT_CRAFTINGINVENTORYPLAYERCB_H
