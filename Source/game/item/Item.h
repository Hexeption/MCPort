//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ITEM_H
#define MCPORT_ITEM_H
#include "java/Random.h"

class Block;
class World;
class Entity;
class EntityLiving;
class EntityPlayer;
class ItemStack;

class Item {
protected:
    static Random random;

public:
    static std::array<Item *, 32000> itemsList;

    static Item *shovel;
    static Item *pickaxeSteel;
    static Item *axeSteel;
    static Item *bow;
    static Item *arrow;
    static Item *diamond;
    static Item *ingotIron;
    static Item *ingotGold;
    static Item *swordSteel;
    static Item *swordWood;
    static Item *shovelWood;
    static Item *pickaxeWood;
    static Item *axeWood;
    static Item *swordStone;
    static Item *shovelStone;
    static Item *pickaxeStone;
    static Item *axeStone;
    static Item *swordDiamond;
    static Item *shovelDiamond;
    static Item *pickaxeDiamond;
    static Item *axeDiamond;
    static Item *stick;
    static Item *swordGold;
    static Item *shovelGold;
    static Item *pickaxeGold;
    static Item *axeGold;
    static Item *silk;
    static Item *feather;
    static Item *hoeWood;
    static Item *hoeStone;
    static Item *hoeSteel;
    static Item *hoeDiamond;
    static Item *hoeGold;
    static Item *seeds;
    static Item *flint;

    const int_t shiftedIndex;
    int_t maxStackSize = 64;
    int_t maxDamage = 32;
    int_t iconIndex = 0;
    bool bFull3D = false;

protected:
    explicit Item(int_t id);

public:
    virtual ~Item() = default;

    virtual Item *setIconIndex(int_t index);

    virtual int_t getIconIndex(ItemStack *stack);

    virtual bool onItemUse(ItemStack *stack, EntityPlayer *player, World *world,
                           int_t x, int_t y, int_t z, int_t side);

    virtual float getStrVsBlock(ItemStack *stack, Block *block);

    virtual ItemStack *onItemRightClick(ItemStack *stack, World *world, EntityPlayer *player);

    virtual int_t getItemStackLimit() const;

    virtual int_t getMaxDamage() const;

    virtual void hitEntity(ItemStack *stack, EntityLiving *entity);

    virtual void onBlockDestroyed(ItemStack *stack, int_t x, int_t y, int_t z, int_t side);

    virtual int_t getDamageVsEntity(Entity *entity);

    virtual bool canHarvestBlock(Block *block);

    virtual void saddleEntity(ItemStack *stack, EntityLiving *entity);

    virtual Item *setFull3D();

    virtual bool isFull3D() const;
};


#endif //MCPORT_ITEM_H
