//
// Created by Keir Davis on 26/06/2026.
//

#include "ItemSword.h"

#include "ItemStack.h"
#include "game/block/Block.h"

ItemSword::ItemSword(int_t id, int_t material) : Item(id) {
    maxStackSize = 1;
    maxDamage = 32 << material;
    if (material == 3) {
        maxDamage *= 4;
    }
    weaponDamage = 4 + material * 2;
}

float ItemSword::getStrVsBlock(ItemStack *, Block *) {
    return 1.5f;
}

void ItemSword::hitEntity(ItemStack *stack, EntityLiving *) {
    stack->damageItem(1);
}

void ItemSword::onBlockDestroyed(ItemStack *stack, int_t, int_t, int_t, int_t) {
    stack->damageItem(2);
}

int_t ItemSword::getDamageVsEntity(Entity *) {
    return weaponDamage;
}

bool ItemSword::isFull3D() const {
    return true;
}
