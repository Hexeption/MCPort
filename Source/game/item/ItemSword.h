//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ITEMSWORD_H
#define MCPORT_ITEMSWORD_H

#include "Item.h"

class ItemSword : public Item {
private:
    int_t weaponDamage;

public:
    ItemSword(int_t id, int_t material);

    float getStrVsBlock(ItemStack *stack, Block *block) override;

    void hitEntity(ItemStack *stack, EntityLiving *entity) override;

    void onBlockDestroyed(ItemStack *stack, int_t x, int_t y, int_t z, int_t side) override;

    int_t getDamageVsEntity(Entity *entity) override;

    bool isFull3D() const override;
};

#endif //MCPORT_ITEMSWORD_H
