//
// Created by Keir Davis on 26/06/2026.
//

#include "InventoryPlayer.h"

#include "game/block/Block.h"
#include "game/block/Material.h"
#include "game/entity/Entity.h"
#include "game/entity/EntityPlayer.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/nbt/NBTTagList.h"

InventoryPlayer::InventoryPlayer(EntityPlayer &player) : player(&player) {
}

int_t InventoryPlayer::getInventorySlotContainItem(const int_t itemId) {
    for (int_t slot = 0; slot < static_cast<int_t>(mainInventory.size()); ++slot) {
        if (mainInventory[slot].has_value() && mainInventory[slot]->itemID == itemId) {
            return slot;
        }
    }

    return -1;
}

int_t InventoryPlayer::storeItemStack(const int_t itemId) {
    for (int_t slot = 0; slot < static_cast<int_t>(mainInventory.size()); ++slot) {
        if (mainInventory[slot].has_value() && mainInventory[slot]->itemID == itemId &&
            mainInventory[slot]->stackSize<mainInventory[slot]->getMaxStackSize() &&
                                           mainInventory[slot]->stackSize < getInventoryStackLimit()) {
            return slot;
        }
    }

    return -1;
}

int_t InventoryPlayer::getFirstEmptyStack() {
    for (int_t slot = 0; slot < static_cast<int_t>(mainInventory.size()); ++slot) {
        if (!mainInventory[slot].has_value()) {
            return slot;
        }
    }

    return -1;
}

int_t InventoryPlayer::storePartialItemStack(const int_t itemId, int_t stackSize) {
    int_t slot = storeItemStack(itemId);
    if (slot < 0) {
        slot = getFirstEmptyStack();
    }

    if (slot < 0) {
        return stackSize;
    }

    if (!mainInventory[slot].has_value()) {
        mainInventory[slot] = ItemStack(itemId, 0);
    }

    int_t transfer = stackSize;
    if (transfer > mainInventory[slot]->getMaxStackSize() - mainInventory[slot]->stackSize) {
        transfer = mainInventory[slot]->getMaxStackSize() - mainInventory[slot]->stackSize;
    }
    if (transfer > getInventoryStackLimit() - mainInventory[slot]->stackSize) {
        transfer = getInventoryStackLimit() - mainInventory[slot]->stackSize;
    }

    if (transfer == 0) {
        return stackSize;
    }

    stackSize -= transfer;
    mainInventory[slot]->stackSize += transfer;
    mainInventory[slot]->animationsToGo = 5;
    return stackSize;
}

bool InventoryPlayer::getItemStacksEqual(const std::optional<ItemStack> &left, const std::optional<ItemStack> &right) {
    if (!left.has_value() && !right.has_value()) {
        return true;
    }
    if (left.has_value() && right.has_value()) {
        return left->itemID == right->itemID && left->stackSize == right->stackSize && left->itemDmg == right->itemDmg;
    }
    return false;
}

ItemStack *InventoryPlayer::getCurrentItem() {
    return mainInventory[currentItem].has_value() ? &*mainInventory[currentItem] : nullptr;
}

void InventoryPlayer::setCurrentItem(const int_t itemId, bool) {
    const int_t slot = getInventorySlotContainItem(itemId);
    if (slot >= 0 && slot < 9) {
        currentItem = slot;
    }
}

void InventoryPlayer::changeCurrentItem(const int_t delta) {
    int_t step = delta > 0 ? 1 : (delta < 0 ? -1 : 0);
    currentItem -= step;
    while (currentItem < 0) {
        currentItem += 9;
    }
    while (currentItem >= 9) {
        currentItem -= 9;
    }
}

void InventoryPlayer::decrementAnimations() {
    for (auto &stack: mainInventory) {
        if (stack.has_value() && stack->animationsToGo > 0) {
            --stack->animationsToGo;
        }
    }
}

bool InventoryPlayer::consumeInventoryItem(const int_t itemId) {
    const int_t slot = getInventorySlotContainItem(itemId);
    if (slot < 0 || !mainInventory[slot].has_value()) {
        return false;
    }

    if (--mainInventory[slot]->stackSize <= 0) {
        mainInventory[slot].reset();
    }
    return true;
}

bool InventoryPlayer::addItemStackToInventory(ItemStack &stack) {
    if (stack.itemDmg == 0) {
        stack.stackSize = storePartialItemStack(stack.itemID, stack.stackSize);
        if (stack.stackSize == 0) {
            return true;
        }
    }

    const int_t slot = getFirstEmptyStack();
    if (slot < 0) {
        return false;
    }

    mainInventory[slot] = stack;
    mainInventory[slot]->animationsToGo = 5;
    return true;
}

std::optional<ItemStack> InventoryPlayer::decrStackSize(const int_t slot, const int_t amount) {
    if (slot < static_cast<int_t>(mainInventory.size())) {
        auto &stack = mainInventory[slot];
        if (!stack.has_value()) {
            return std::nullopt;
        }
        if (stack->stackSize <= amount) {
            std::optional<ItemStack> result = *stack;
            stack.reset();
            return result;
        }
        ItemStack result = stack->splitStack(amount);
        if (stack->stackSize == 0) {
            stack.reset();
        }
        return result;
    }

    int_t index = slot - static_cast<int_t>(mainInventory.size());
    if (index < static_cast<int_t>(armorInventory.size())) {
        auto &stack = armorInventory[index];
        if (!stack.has_value()) {
            return std::nullopt;
        }
        if (stack->stackSize <= amount) {
            std::optional<ItemStack> result = *stack;
            stack.reset();
            return result;
        }
        ItemStack result = stack->splitStack(amount);
        if (stack->stackSize == 0) {
            stack.reset();
        }
        return result;
    }

    index -= static_cast<int_t>(armorInventory.size());
    if (index < static_cast<int_t>(craftingInventory.size())) {
        auto &stack = craftingInventory[index];
        if (!stack.has_value()) {
            return std::nullopt;
        }
        if (stack->stackSize <= amount) {
            std::optional<ItemStack> result = *stack;
            stack.reset();
            return result;
        }
        ItemStack result = stack->splitStack(amount);
        if (stack->stackSize == 0) {
            stack.reset();
        }
        return result;
    }

    return std::nullopt;
}

void InventoryPlayer::setInventorySlotContents(const int_t slot, const ItemStack &stack) {
    if (slot < static_cast<int_t>(mainInventory.size())) {
        mainInventory[slot] = stack;
        return;
    }

    int_t index = slot - static_cast<int_t>(mainInventory.size());
    if (index < static_cast<int_t>(armorInventory.size())) {
        armorInventory[index] = stack;
        return;
    }

    index -= static_cast<int_t>(armorInventory.size());
    if (index < static_cast<int_t>(craftingInventory.size())) {
        craftingInventory[index] = stack;
    }
}

float InventoryPlayer::getStrVsBlock(Block &block) {
    if (ItemStack *stack = getCurrentItem()) {
        return stack->getStrVsBlock(block);
    }
    return 1.0f;
}

void InventoryPlayer::writeToNBT(NBTTagList &list) const {
    for (int_t slot = 0; slot < static_cast<int_t>(mainInventory.size()); ++slot) {
        if (mainInventory[slot].has_value()) {
            auto *tag = new NBTTagCompound();
            tag->setByte(u"Slot", static_cast<byte_t>(slot));
            mainInventory[slot]->writeToNBT(*tag);
            list.setTag(tag);
        }
    }

    for (int_t slot = 0; slot < static_cast<int_t>(armorInventory.size()); ++slot) {
        if (armorInventory[slot].has_value()) {
            auto *tag = new NBTTagCompound();
            tag->setByte(u"Slot", static_cast<byte_t>(slot + 100));
            armorInventory[slot]->writeToNBT(*tag);
            list.setTag(tag);
        }
    }

    for (int_t slot = 0; slot < static_cast<int_t>(craftingInventory.size()); ++slot) {
        if (craftingInventory[slot].has_value()) {
            auto *tag = new NBTTagCompound();
            tag->setByte(u"Slot", static_cast<byte_t>(slot + 80));
            craftingInventory[slot]->writeToNBT(*tag);
            list.setTag(tag);
        }
    }
}

void InventoryPlayer::readFromNBT(NBTTagList &list) {
    mainInventory.fill(std::nullopt);
    armorInventory.fill(std::nullopt);
    craftingInventory.fill(std::nullopt);

    for (int_t index = 0; index < list.tagCount(); ++index) {
        auto *tag = dynamic_cast<NBTTagCompound *>(list.tagAt(index));
        if (tag == nullptr) {
            continue;
        }

        const int_t slot = tag->getByte(u"Slot") & 255;
        if (slot >= 0 && slot < static_cast<int_t>(mainInventory.size())) {
            mainInventory[slot] = ItemStack(*tag);
        } else if (slot >= 80 && slot < 80 + static_cast<int_t>(craftingInventory.size())) {
            craftingInventory[slot - 80] = ItemStack(*tag);
        } else if (slot >= 100 && slot < 100 + static_cast<int_t>(armorInventory.size())) {
            armorInventory[slot - 100] = ItemStack(*tag);
        }
    }
}

ItemStack *InventoryPlayer::getStackInSlot(const int_t slot) {
    if (slot < static_cast<int_t>(mainInventory.size())) {
        return mainInventory[slot].has_value() ? &*mainInventory[slot] : nullptr;
    }

    int_t index = slot - static_cast<int_t>(mainInventory.size());
    if (index < static_cast<int_t>(armorInventory.size())) {
        return armorInventory[index].has_value() ? &*armorInventory[index] : nullptr;
    }

    index -= static_cast<int_t>(armorInventory.size());
    if (index < static_cast<int_t>(craftingInventory.size())) {
        return craftingInventory[index].has_value() ? &*craftingInventory[index] : nullptr;
    }

    return nullptr;
}

int_t InventoryPlayer::getSizeInventory() {
    return static_cast<int_t>(mainInventory.size()) + static_cast<int_t>(armorInventory.size());
}

jstring InventoryPlayer::getInvName() {
    return u"Inventory";
}

int_t InventoryPlayer::getInventoryStackLimit() {
    return 64;
}

int_t InventoryPlayer::getDamageVsEntity(Entity *entity) {
    if (entity != nullptr) {
        if (ItemStack *stack = getCurrentItem()) {
            return stack->getDamageVsEntity(*entity);
        }
    }
    return 1;
}

bool InventoryPlayer::canHarvestBlock(Block &block) {
    if (block.material != Material::rock && block.material != Material::iron &&
        block.material != Material::craftedSnow && block.material != Material::snow) {
        return true;
    }

    if (ItemStack *stack = getCurrentItem()) {
        return stack->canHarvestBlock(block);
    }
    return false;
}

int_t InventoryPlayer::getTotalArmorValue() {
    return 0;
}

void InventoryPlayer::damageArmor(const int_t) {
}

void InventoryPlayer::dropAllItems() {
    if (player == nullptr) {
        return;
    }

    for (auto &stack: mainInventory) {
        if (stack.has_value()) {
            player->dropPlayerItemWithRandomChoice(*stack, true);
            stack.reset();
        }
    }

    for (auto &stack: armorInventory) {
        if (stack.has_value()) {
            player->dropPlayerItemWithRandomChoice(*stack, true);
            stack.reset();
        }
    }
}

void InventoryPlayer::onInventoryChanged() {
    inventoryChanged = true;
}

bool InventoryPlayer::getInventoryEqual(InventoryPlayer &other) {
    for (int_t slot = 0; slot < static_cast<int_t>(mainInventory.size()); ++slot) {
        if (!getItemStacksEqual(other.mainInventory[slot], mainInventory[slot])) {
            return false;
        }
    }

    for (int_t slot = 0; slot < static_cast<int_t>(armorInventory.size()); ++slot) {
        if (!getItemStacksEqual(other.armorInventory[slot], armorInventory[slot])) {
            return false;
        }
    }

    for (int_t slot = 0; slot < static_cast<int_t>(craftingInventory.size()); ++slot) {
        if (!getItemStacksEqual(other.craftingInventory[slot], craftingInventory[slot])) {
            return false;
        }
    }

    return true;
}

InventoryPlayer InventoryPlayer::copyInventory() {
    InventoryPlayer copy = *this;
    copy.player = nullptr;
    return copy;
}
