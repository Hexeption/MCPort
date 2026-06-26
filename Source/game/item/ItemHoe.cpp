//
// Created by Keir Davis on 26/06/2026.
//

#include "ItemHoe.h"

#include "Item.h"
#include "ItemStack.h"
#include "game/block/Block.h"
#include "game/block/Material.h"
#include "game/entity/EntityItem.h"
#include "game/entity/EntityPlayer.h"
#include "game/world/World.h"

ItemHoe::ItemHoe(int_t id, int_t material) : Item(id) {
    maxStackSize = 1;
    maxDamage = 32 << material;
}

bool ItemHoe::onItemUse(ItemStack *stack, EntityPlayer *, World *world, int_t x, int_t y, int_t z, int_t) {
    int_t blockId = world->getBlockId(x, y, z);
    Material *above = world->getBlockMaterial(x, y + 1, z);
    if ((above->isSolid() || blockId != Block::grass->blockID) && blockId != Block::dirt->blockID) {
        return false;
    }
    stack->damageItem(1);
    return true;
}

bool ItemHoe::isFull3D() const {
    return true;
}
