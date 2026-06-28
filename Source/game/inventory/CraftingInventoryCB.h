//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CRAFTINGINVENTORYCB_H
#define MCPORT_CRAFTINGINVENTORYCB_H

#include "game/entity/EntityPlayer.h"
#include "IInventory.h"

class CraftingInventoryCB {
public:
    virtual ~CraftingInventoryCB() = default;

    virtual void onCraftMatrixChanged(IInventory &inventory);

    virtual void onCraftGuiClosed(EntityPlayer &player);
};

#endif //MCPORT_CRAFTINGINVENTORYCB_H
