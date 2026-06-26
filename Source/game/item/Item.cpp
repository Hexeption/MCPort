//
// Created by Keir Davis on 25/06/2026.
//

#include "Item.h"

#include <iostream>

#include "ItemAxe.h"
#include "ItemHoe.h"
#include "ItemPickaxe.h"
#include "ItemSpade.h"
#include "ItemSword.h"
#include "ItemTool.h"

std::array<Item *, 32000> Item::itemsList{};

Item *Item::shovel         = (new ItemSpade(0, 2))->setIconIndex(82);
Item *Item::pickaxeSteel   = (new ItemPickaxe(1, 2))->setIconIndex(98);
Item *Item::axeSteel       = (new ItemAxe(2, 2))->setIconIndex(114);
Item *Item::bow            = (new Item(5))->setIconIndex(21);
Item *Item::arrow          = (new Item(6))->setIconIndex(37);
Item *Item::diamond        = (new Item(8))->setIconIndex(55);
Item *Item::ingotIron      = (new Item(9))->setIconIndex(23);
Item *Item::ingotGold      = (new Item(10))->setIconIndex(39);
Item *Item::swordSteel     = (new ItemSword(11, 2))->setIconIndex(66);
Item *Item::swordWood      = (new ItemSword(12, 0))->setIconIndex(64);
Item *Item::shovelWood     = (new ItemSpade(13, 0))->setIconIndex(80);
Item *Item::pickaxeWood    = (new ItemPickaxe(14, 0))->setIconIndex(96);
Item *Item::axeWood        = (new ItemAxe(15, 0))->setIconIndex(112);
Item *Item::swordStone     = (new ItemSword(16, 1))->setIconIndex(65);
Item *Item::shovelStone    = (new ItemSpade(17, 1))->setIconIndex(81);
Item *Item::pickaxeStone   = (new ItemPickaxe(18, 1))->setIconIndex(97);
Item *Item::axeStone       = (new ItemAxe(19, 1))->setIconIndex(113);
Item *Item::swordDiamond   = (new ItemSword(20, 3))->setIconIndex(67);
Item *Item::shovelDiamond  = (new ItemSpade(21, 3))->setIconIndex(83);
Item *Item::pickaxeDiamond = (new ItemPickaxe(22, 3))->setIconIndex(99);
Item *Item::axeDiamond     = (new ItemAxe(23, 3))->setIconIndex(115);
Item *Item::stick          = (new Item(24))->setIconIndex(53)->setFull3D();
Item *Item::swordGold      = (new ItemSword(27, 0))->setIconIndex(68);
Item *Item::shovelGold     = (new ItemSpade(28, 0))->setIconIndex(84);
Item *Item::pickaxeGold    = (new ItemPickaxe(29, 0))->setIconIndex(100);
Item *Item::axeGold        = (new ItemAxe(30, 0))->setIconIndex(116);
Item *Item::silk           = (new Item(31))->setIconIndex(8);
Item *Item::feather        = (new Item(32))->setIconIndex(24);
Item *Item::hoeWood        = (new ItemHoe(34, 0))->setIconIndex(128);
Item *Item::hoeStone       = (new ItemHoe(35, 1))->setIconIndex(129);
Item *Item::hoeSteel       = (new ItemHoe(36, 2))->setIconIndex(130);
Item *Item::hoeDiamond     = (new ItemHoe(37, 3))->setIconIndex(131);
Item *Item::hoeGold        = (new ItemHoe(38, 1))->setIconIndex(132);
Item *Item::seeds          = (new Item(39))->setIconIndex(9);
Item *Item::flint          = (new Item(62))->setIconIndex(6);

Item::Item(int_t id) : shiftedIndex(256 + id) {
    if (itemsList[shiftedIndex] != nullptr) {
        std::cout << "CONFLICT @ " << id << '\n';
    }

    itemsList[shiftedIndex] = this;
}

Item *Item::setIconIndex(int_t index) {
    iconIndex = index;
    return this;
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

Item *Item::setFull3D() {
    bFull3D = true;
    return this;
}

bool Item::isFull3D() const {
    return bFull3D;
}
