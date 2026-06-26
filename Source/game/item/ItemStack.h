//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ITEMSTACK_H
#define MCPORT_ITEMSTACK_H


#include "java/Type.h"

class Block;
class Entity;
class EntityLiving;
class EntityPlayer;
class Item;
class NBTTagCompound;
class World;

class ItemStack final {
public:
    int_t stackSize = 0;
    int_t animationsToGo = 0;
    int_t itemID = 0;
    int_t itemDmg = 0;

    explicit ItemStack(Block &block);

    ItemStack(Block &block, int_t size);

    explicit ItemStack(Item &item);

    ItemStack(Item &item, int_t size);

    explicit ItemStack(int_t id);

    ItemStack(int_t id, int_t size);

    ItemStack(int_t id, int_t size, int_t damage);

    explicit ItemStack(NBTTagCompound &nbt);

    ItemStack splitStack(int_t amount);

    Item *getItem() const;

    int_t getIconIndex() const;

    bool useItem(EntityPlayer &player, World &world, int_t x, int_t y, int_t z, int_t side);

    float getStrVsBlock(Block &block);

    ItemStack useItemRightClick(World &world, EntityPlayer &player);

    NBTTagCompound &writeToNBT(NBTTagCompound &nbt) const;

    void readFromNBT(NBTTagCompound &nbt);

    int_t getMaxStackSize() const;

    int_t getMaxDamage() const;

    void damageItem(int_t amount);

    void hitEntity(EntityLiving &entity);

    void onDestroyBlock(int_t x, int_t y, int_t z, int_t side);

    int_t getDamageVsEntity(Entity &entity);

    bool canHarvestBlock(Block &block);

    void onItemDestroyedByUse(EntityPlayer &player);

    void useItemOnEntity(EntityLiving &entity);

    ItemStack copy() const;
};


#endif //MCPORT_ITEMSTACK_H
