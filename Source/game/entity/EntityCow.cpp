//
// Created by Keir Davis on 26/06/2026.
//

#include "EntityCow.h"

#include "game/entity/EntityPlayer.h"
#include "game/item/Item.h"
#include "game/item/ItemStack.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/world/World.h"

EntityCow::EntityCow(World &world) : EntityAnimal(world) {
    texture = u"/mob/cow.png";
    setSize(0.9f, 1.3f);
}

bool EntityCow::interact(EntityPlayer &player) {
    ItemStack *currentItem = player.inventory.getCurrentItem();
    if (currentItem != nullptr && currentItem->itemID == 325) {
        player.inventory.setInventorySlotContents(player.inventory.currentItem, ItemStack(335));
        return true;
    }
    return false;
}

void EntityCow::writeEntityToNBT(NBTTagCompound &nbt) {
    EntityAnimal::writeEntityToNBT(nbt);
}

void EntityCow::readEntityFromNBT(NBTTagCompound &nbt) {
    EntityAnimal::readEntityFromNBT(nbt);
}

jstring EntityCow::getLivingSound() const { return u"mob.cow"; }
jstring EntityCow::getHurtSound() const { return u"mob.cowhurt"; }
jstring EntityCow::getDeathSound() const { return u"mob.cowhurt"; }
float EntityCow::getSoundVolume() const { return 0.4f; }
int_t EntityCow::getDropItemId() const { return 78; }
