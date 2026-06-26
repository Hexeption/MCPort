//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ITEMTOOL_H
#define MCPORT_ITEMTOOL_H

#include <vector>
#include "Item.h"

class Block;

class ItemTool : public Item {
private:
    std::vector<Block *> blocksEffectiveAgainst;
    float efficiencyOnProperMaterial = 4.0f;
    int_t damageVsEntity;

protected:
    int_t toolMaterial;

public:
    ItemTool(int_t id, int_t damageVsEntity, int_t toolMaterial, std::vector<Block *> blocks);

    float getStrVsBlock(ItemStack *stack, Block *block) override;

    void hitEntity(ItemStack *stack, EntityLiving *entity) override;

    void onBlockDestroyed(ItemStack *stack, int_t x, int_t y, int_t z, int_t side) override;

    int_t getDamageVsEntity(Entity *entity) override;

    bool isFull3D() const override;
};

#endif //MCPORT_ITEMTOOL_H
