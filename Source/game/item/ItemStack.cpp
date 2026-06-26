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
    if (Item *item = getItem()) {
        return item->getIconIndex(const_cast<ItemStack *>(this));
    }

    if (itemID >= 0 && itemID < static_cast<int_t>(Block::blocksList.size()) && Block::blocksList[itemID] != nullptr) {
        return Block::blocksList[itemID]->getBlockTextureFromSide(0);
    }

    return 0;
}

bool ItemStack::useItem(EntityPlayer &player, World &world, int_t x, int_t y, int_t z, int_t side) {
    return getItem()->onItemUse(this, &player, &world, x, y, z, side);
}

float ItemStack::getStrVsBlock(Block &block) {
    if (Item *item = getItem()) {
        return item->getStrVsBlock(this, &block);
    }

    if (itemID >= 0 && itemID < static_cast<int_t>(Block::blocksList.size()) && Block::blocksList[itemID] != nullptr) {
        return 1.0f;
    }

    return 1.0f;
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
    stackSize = static_cast<int_t>(static_cast<ubyte_t>(nbt.getByte(u"Count")));
    itemDmg = nbt.getShort(u"Damage");
    if (stackSize < 0) {
        stackSize = 0;
    }
    if (stackSize > getMaxStackSize()) {
        stackSize = getMaxStackSize();
    }
}

int_t ItemStack::getMaxStackSize() const {
    if (Item *item = getItem()) {
        return item->getItemStackLimit();
    }

    if (itemID >= 0 && itemID < static_cast<int_t>(Block::blocksList.size()) && Block::blocksList[itemID] != nullptr) {
        return 64;
    }

    return 64;
}

int_t ItemStack::getMaxDamage() const {
    if (Item *item = Item::itemsList[itemID]) {
        return item->getMaxDamage();
    }

    return 0;
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
    if (Item *item = Item::itemsList[itemID]) {
        item->hitEntity(this, &entity);
    }
}

void ItemStack::onDestroyBlock(int_t x, int_t y, int_t z, int_t side) {
    if (Item *item = Item::itemsList[itemID]) {
        item->onBlockDestroyed(this, x, y, z, side);
    }
}

int_t ItemStack::getDamageVsEntity(Entity &entity) {
    if (Item *item = Item::itemsList[itemID]) {
        return item->getDamageVsEntity(&entity);
    }
    return 1;
}

bool ItemStack::canHarvestBlock(Block &block) {
    if (Item *item = Item::itemsList[itemID]) {
        return item->canHarvestBlock(&block);
    }
    return true;
}

void ItemStack::onItemDestroyedByUse(EntityPlayer &) {
}

void ItemStack::useItemOnEntity(EntityLiving &entity) {
    if (Item *item = Item::itemsList[itemID]) {
        item->saddleEntity(this, &entity);
    }
}

ItemStack ItemStack::copy() const {
    return ItemStack(itemID, stackSize, itemDmg);
}
