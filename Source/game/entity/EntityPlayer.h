//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYPLAYER_H
#define MCPORT_ENTITYPLAYER_H

#include "EntityLiving.h"
#include "java/Type.h"
#include "game/inventory/InventoryPlayer.h"

class Block;
class Entity;
class EntityItem;
class ItemStack;

class EntityPlayer : public EntityLiving {
public:
    InventoryPlayer inventory;
    int_t score = 0;
    float prevCameraYaw = 0.0f;
    float cameraYaw = 0.0f;
    bool isSwinging = false;
    int_t swingProgressInt = 0;

    explicit EntityPlayer(World &world);

    void preparePlayerToSpawn() override;

    void onLivingUpdate() override;

    float getEyeHeight() const override;

    float getCurrentPlayerStrVsBlock(const Block &block);

    bool canHarvestBlock(const Block &block);

    void swingItem();

    ItemStack *getCurrentEquippedItem();

    void destroyCurrentEquippedItem();

    void dropPlayerItem(ItemStack &stack);

    void dropPlayerItemWithRandomChoice(ItemStack &stack, bool randomize);

    void joinEntityItemWithWorld(EntityItem &item);

    void onItemPickup(Entity &entity, int_t amount);
};

#endif //MCPORT_ENTITYPLAYER_H
