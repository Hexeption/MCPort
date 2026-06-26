//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ITEMHOE_H
#define MCPORT_ITEMHOE_H

#include "Item.h"

class ItemHoe : public Item {
public:
    ItemHoe(int_t id, int_t material);

    bool onItemUse(ItemStack *stack, EntityPlayer *player, World *world,
                   int_t x, int_t y, int_t z, int_t side) override;

    bool isFull3D() const override;
};

#endif //MCPORT_ITEMHOE_H
