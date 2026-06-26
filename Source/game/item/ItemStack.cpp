//
// Created by Keir Davis on 25/06/2026.
//

#include "ItemStack.h"

#include "game/block/Block.h"
#include "game/entity/Entity.h"
#include "game/entity/EntityLiving.h"
#include "game/entity/EntityPlayer.h"
#include "game/item/Item.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/world/World.h"

ItemStack::ItemStack(Block &block) : ItemStack(block, 1) {
}

ItemStack::ItemStack(Block &block, int_t size) : ItemStack(block.blockID, size) {
}

ItemStack::ItemStack(Item &item) : ItemStack(item, 1) {
}

ItemStack::ItemStack(Item &item, int_t size) : ItemStack(item.shiftedIndex, size) {
}

ItemStack::ItemStack(int_t id) : ItemStack(id, 1) {
}

ItemStack::ItemStack(int_t id, int_t size) {
    stackSize = 0;
    itemID = id;
    stackSize = size;
}

ItemStack::ItemStack(int_t id, int_t size, int_t damage) {
    stackSize = 0;
    itemID = id;
    stackSize = size;
    itemDmg = damage;
}

ItemStack::ItemStack(NBTTagCompound &nbt) {
    stackSize = 0;
    readFromNBT(nbt);
}

ItemStack ItemStack::splitStack(int_t amount) {
    stackSize -= amount;
    return ItemStack(itemID, amount, itemDmg);
}

Item *ItemStack::getItem() const {
    return Item::itemsList[itemID];
}

int_t ItemStack::getIconIndex() const {
    return getItem()->getIconIndex(const_cast<ItemStack *>(this));
}

bool ItemStack::useItem(EntityPlayer &player, World &world, int_t x, int_t y, int_t z, int_t side) {
    return getItem()->onItemUse(this, &player, &world, x, y, z, side);
}

float ItemStack::getStrVsBlock(Block &block) {
    return getItem()->getStrVsBlock(this, &block);
}

ItemStack ItemStack::useItemRightClick(World &world, EntityPlayer &player) {
    return *getItem()->onItemRightClick(this, &world, &player);
}

NBTTagCompound &ItemStack::writeToNBT(NBTTagCompound &nbt) const {
    nbt.setShort(u"id", static_cast<short_t>(itemID));
    nbt.setByte(u"Count", static_cast<byte_t>(stackSize));
    nbt.setShort(u"Damage", static_cast<short_t>(itemDmg));
    return nbt;
}

void ItemStack::readFromNBT(NBTTagCompound &nbt) {
    itemID = nbt.getShort(u"id");
    stackSize = nbt.getByte(u"Count");
    itemDmg = nbt.getShort(u"Damage");
}

int_t ItemStack::getMaxStackSize() const {
    return getItem()->getItemStackLimit();
}

int_t ItemStack::getMaxDamage() const {
    return Item::itemsList[itemID]->getMaxDamage();
}

void ItemStack::damageItem(int_t amount) {
    itemDmg += amount;
    if (itemDmg > getMaxDamage()) {
        --stackSize;
        if (stackSize < 0) {
            stackSize = 0;
        }

        itemDmg = 0;
    }
}

void ItemStack::hitEntity(EntityLiving &entity) {
    Item::itemsList[itemID]->hitEntity(this, &entity);
}

void ItemStack::onDestroyBlock(int_t x, int_t y, int_t z, int_t side) {
    Item::itemsList[itemID]->onBlockDestroyed(this, x, y, z, side);
}

int_t ItemStack::getDamageVsEntity(Entity &entity) {
    return Item::itemsList[itemID]->getDamageVsEntity(&entity);
}

bool ItemStack::canHarvestBlock(Block &block) {
    return Item::itemsList[itemID]->canHarvestBlock(&block);
}

void ItemStack::onItemDestroyedByUse(EntityPlayer &) {
}

void ItemStack::useItemOnEntity(EntityLiving &entity) {
    Item::itemsList[itemID]->saddleEntity(this, &entity);
}

ItemStack ItemStack::copy() const {
    return ItemStack(itemID, stackSize, itemDmg);
}
