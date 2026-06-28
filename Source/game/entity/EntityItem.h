//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYITEM_H
#define MCPORT_ENTITYITEM_H


#include "game/entity/Entity.h"
#include "game/entity/EntityLiving.h"
#include "game/item/ItemStack.h"
#include "game/nbt/NBTTagCompound.h"

class EntityItem : public Entity {
private:
    int_t age2 = 0;
    int_t health = 5;

    bool pushOutOfBlocks(double x, double y, double z);

protected:
    void dealFireDamage(int_t amount);

public:
    ItemStack item;
    int_t age = 0;
    int_t delayBeforeCanPickup = 0;
    float hoverStart = 0.0f;

    EntityItem(World &world, double x, double y, double z, const ItemStack &stack);

    explicit EntityItem(World &world);

    void onUpdate() override;

    bool handleWaterMovement() override;

    bool attackEntityFrom(Entity *entity, int_t amount) override;

    void writeEntityToNBT(NBTTagCompound &nbt) override;

    void readEntityFromNBT(NBTTagCompound &nbt) override;

    void onCollideWithPlayer(EntityPlayer &player) override;
};


#endif //MCPORT_ENTITYITEM_H
