//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityPlayer.h"

#include <algorithm>
#include <cmath>
#include <memory>

#include "game/block/Block.h"
#include "game/block/Material.h"
#include "game/entity/EntityItem.h"
#include "game/item/ItemStack.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/nbt/NBTTagList.h"
#include "game/world/World.h"

EntityPlayer::EntityPlayer(World &world) : EntityLiving(world), inventory(*this) {
    yOffset = 1.62f;
    isAIEnabled = true;
    setLocationAndAngles(static_cast<double>(world.spawnX) + 0.5, static_cast<double>(world.spawnY + 1),
                         static_cast<double>(world.spawnZ) + 0.5, 0.0f, 0.0f);
    health = 20;
}

void EntityPlayer::preparePlayerToSpawn() {
    yOffset = 1.62f;
    setSize(0.6f, 1.8f);
    EntityLiving::preparePlayerToSpawn();
    health = 20;
}

void EntityPlayer::onLivingUpdate() {
    inventory.decrementAnimations();
    prevCameraYaw = cameraYaw;
    prevCameraPitch = cameraPitch;
    EntityLiving::onLivingUpdate();

    float horizontalMotion = static_cast<float>(std::sqrt(motionX * motionX + motionZ * motionZ));
    float pitchMotion = static_cast<float>(std::atan(-motionY * 0.2) * 15.0);
    horizontalMotion = std::min(horizontalMotion, 0.1f);
    if (!onGround || health <= 0) {
        horizontalMotion = 0.0f;
    }
    if (onGround || health <= 0) {
        pitchMotion = 0.0f;
    }
    cameraYaw += (horizontalMotion - cameraYaw) * 0.4f;
    cameraPitch += (pitchMotion - cameraPitch) * 0.8f;

    if (health > 0) {
        const double expand = 1.0;
        const auto entities = worldObj.getEntitiesWithinAABBExcludingEntity(
            *this, boundingBox.expand(expand, 0.0, expand));
        for (Entity *entity: entities) {
            if (entity != nullptr) {
                entity->onCollideWithPlayer(*this);
            }
        }
    }
}

float EntityPlayer::getEyeHeight() const {
    return 0.12f;
}

float EntityPlayer::getCurrentPlayerStrVsBlock(const Block &block) {
    float strength = inventory.getStrVsBlock(const_cast<Block &>(block));
    if (isInsideOfMaterial(Material::water)) {
        strength /= 5.0f;
    }
    if (!onGround) {
        strength /= 5.0f;
    }
    return strength;
}

bool EntityPlayer::canHarvestBlock(const Block &block) {
    return inventory.canHarvestBlock(const_cast<Block &>(block));
}

void EntityPlayer::swingItem() {
    swingProgressInt = -1;
    isSwinging = true;
}

ItemStack *EntityPlayer::getCurrentEquippedItem() {
    return inventory.getCurrentItem();
}

void EntityPlayer::destroyCurrentEquippedItem() {
    inventory.mainInventory[inventory.currentItem].reset();
}

void EntityPlayer::dropPlayerItem(ItemStack &stack) {
    dropPlayerItemWithRandomChoice(stack, false);
}

void EntityPlayer::dropPlayerItemWithRandomChoice(ItemStack &stack, bool randomize) {
    if (stack.stackSize <= 0) {
        return;
    }

    auto item = std::make_unique<EntityItem>(worldObj, posX, posY - 0.3 + getEyeHeight(), posZ, stack);
    item->delayBeforeCanPickup = 40;

    float spread = 0.1f;
    if (randomize) {
        const float size = rand.nextFloat() * 0.5f;
        const float angle = rand.nextFloat() * static_cast<float>(std::acos(-1.0)) * 2.0f;
        item->motionX = -std::sin(angle) * size;
        item->motionZ = std::cos(angle) * size;
        item->motionY = 0.2f;
    } else {
        spread = 0.3f;
        const float degreesToRadians = static_cast<float>(std::acos(-1.0) / 180.0);
        item->motionX = -std::sin(rotationYaw * degreesToRadians) * std::cos(rotationPitch * degreesToRadians) * spread;
        item->motionZ = std::cos(rotationYaw * degreesToRadians) * std::cos(rotationPitch * degreesToRadians) * spread;
        item->motionY = -std::sin(rotationPitch * degreesToRadians) * spread + 0.1f;
        float jitter = rand.nextFloat() * static_cast<float>(std::acos(-1.0)) * 2.0f;
        spread = 0.02f * rand.nextFloat();
        item->motionX += std::cos(jitter) * spread;
        item->motionY += (rand.nextFloat() - rand.nextFloat()) * 0.1f;
        item->motionZ += std::sin(jitter) * spread;
    }

    worldObj.spawnEntityInWorld(std::move(item));
}

void EntityPlayer::joinEntityItemWithWorld(EntityItem &item) {
    (void) item;
}

void EntityPlayer::onItemPickup(Entity &, int_t) {
}

void EntityPlayer::writeEntityToNBT(NBTTagCompound &nbt) {
    EntityLiving::writeEntityToNBT(nbt);
    auto *inventoryTag = new NBTTagList();
    inventory.writeToNBT(*inventoryTag);
    nbt.setTag(u"Inventory", inventoryTag);
}

void EntityPlayer::readEntityFromNBT(NBTTagCompound &nbt) {
    EntityLiving::readEntityFromNBT(nbt);
    NBTTagList *inventoryTag = nbt.getTagList(u"Inventory");
    inventory.readFromNBT(*inventoryTag);
}

void EntityPlayer::displayWorkbenchGUI() {
}
