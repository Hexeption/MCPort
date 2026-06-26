//
// Created by Keir Davis on 25/06/2026.
//

#include "Item.h"

#include <iostream>

std::array<Item *, 32000> Item::itemsList{};

Item::Item(int_t id) : shiftedIndex(256 + id) {
    if (itemsList[shiftedIndex] != nullptr) {
        std::cout << "CONFLICT @ " << id << '\n';
    }

    itemsList[shiftedIndex] = this;
}

Item &Item::setIconIndex(int_t index) {
    iconIndex = index;
    return *this;
}

int_t Item::getIconIndex(ItemStack *) {
    return iconIndex;
}

bool Item::onItemUse(ItemStack *, EntityPlayer *, World *, int_t, int_t, int_t, int_t) {
    return false;
}

float Item::getStrVsBlock(ItemStack *, Block *) {
    return 1.0f;
}

ItemStack *Item::onItemRightClick(ItemStack *stack, World *, EntityPlayer *) {
    return stack;
}

int_t Item::getItemStackLimit() const {
    return maxStackSize;
}

int_t Item::getMaxDamage() const {
    return maxDamage;
}

void Item::hitEntity(ItemStack *, EntityLiving *) {
}

void Item::onBlockDestroyed(ItemStack *, int_t, int_t, int_t, int_t) {
}

int_t Item::getDamageVsEntity(Entity *) {
    return 1;
}

bool Item::canHarvestBlock(Block *) {
    return false;
}

void Item::saddleEntity(ItemStack *, EntityLiving *) {
}

Item &Item::setFull3D() {
    bFull3D = true;
    return *this;
}

bool Item::isFull3D() const {
    return bFull3D;
}
