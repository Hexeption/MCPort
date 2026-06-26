//
// Created by Keir Davis on 26/06/2026.
//

#include "ItemTool.h"

#include "ItemStack.h"
#include "game/block/Block.h"

ItemTool::ItemTool(int_t id, int_t dvse, int_t material, std::vector<Block *> blocks)
    : Item(id), toolMaterial(material), blocksEffectiveAgainst(std::move(blocks)) {
    maxStackSize = 1;
    maxDamage = 32 << material;
    if (material == 3) {
        maxDamage *= 4;
    }
    efficiencyOnProperMaterial = static_cast<float>((material + 1) * 2);
    damageVsEntity = dvse + material;
}

float ItemTool::getStrVsBlock(ItemStack *, Block *block) {
    for (Block *b : blocksEffectiveAgainst) {
        if (b != nullptr && b == block) {
            return efficiencyOnProperMaterial;
        }
    }
    return 1.0f;
}

void ItemTool::hitEntity(ItemStack *stack, EntityLiving *) {
    stack->damageItem(2);
}

void ItemTool::onBlockDestroyed(ItemStack *stack, int_t, int_t, int_t, int_t) {
    stack->damageItem(1);
}

int_t ItemTool::getDamageVsEntity(Entity *) {
    return damageVsEntity;
}

bool ItemTool::isFull3D() const {
    return true;
}
