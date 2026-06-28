//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_INVENTORYPLAYER_H
#define MCPORT_INVENTORYPLAYER_H

#include <array>
#include <optional>

#include "IInventory.h"
#include "game/item/ItemStack.h"

#include "game/entity/Entity.h"
#include "game/nbt/NBTTagList.h"

class Block;
class EntityPlayer;

class InventoryPlayer : public IInventory {
private:
    EntityPlayer *player = nullptr;

    int_t getInventorySlotContainItem(int_t itemId);

    int_t storeItemStack(int_t itemId);

    int_t getFirstEmptyStack();

    int_t storePartialItemStack(int_t itemId, int_t stackSize);

    bool getItemStacksEqual(const std::optional<ItemStack> &left, const std::optional<ItemStack> &right);

public:
    std::array<std::optional<ItemStack>, 36> mainInventory{};
    std::array<std::optional<ItemStack>, 4> armorInventory{};
    std::array<std::optional<ItemStack>, 4> craftingInventory{};
    int_t currentItem = 0;
    std::optional<ItemStack> draggedItemStack;
    bool inventoryChanged = false;

    explicit InventoryPlayer(EntityPlayer &player);

    ItemStack *getCurrentItem();

    void setCurrentItem(int_t itemId, bool unused);

    void changeCurrentItem(int_t delta);

    void decrementAnimations();

    bool consumeInventoryItem(int_t itemId);

    bool addItemStackToInventory(ItemStack &stack);

    float getStrVsBlock(Block &block);

    void writeToNBT(NBTTagList &list) const;

    void readFromNBT(NBTTagList &list);

    ItemStack *getStackInSlot(int_t slot) override;

    int_t getSizeInventory() override;

    jstring getInvName() override;

    int_t getInventoryStackLimit() override;

    std::optional<ItemStack> decrStackSize(int_t slot, int_t amount) override;

    void setInventorySlotContents(int_t slot, const ItemStack &stack) override;

    int_t getDamageVsEntity(Entity *entity);

    bool canHarvestBlock(Block &block);

    int_t getTotalArmorValue();

    void damageArmor(int_t amount);

    void dropAllItems();

    void onInventoryChanged() override;

    bool getInventoryEqual(InventoryPlayer &other);

    InventoryPlayer copyInventory();
};

#endif //MCPORT_INVENTORYPLAYER_H
