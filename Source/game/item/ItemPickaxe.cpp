//
// Created by Keir Davis on 26/06/2026.
//

#include "ItemPickaxe.h"

#include "game/block/Block.h"
#include "game/block/Material.h"

ItemPickaxe::ItemPickaxe(int_t id, int_t material)
    : ItemTool(id, 2, material, {
        Block::cobblestone, Block::stairDouble, Block::stairSingle,
        Block::stone, Block::cobblestoneMossy, Block::oreIron,
        Block::blockSteel, Block::oreCoal, Block::blockGold,
        Block::oreGold, Block::oreDiamond, Block::blockDiamond, Block::ice
    }), harvestLevel(material) {}

bool ItemPickaxe::canHarvestBlock(Block *block) {
    if (block == Block::obsidian) {
        return harvestLevel == 3;
    }
    if (block == Block::blockDiamond || block == Block::oreDiamond) {
        return harvestLevel >= 2;
    }
    if (block == Block::blockGold || block == Block::oreGold) {
        return harvestLevel >= 2;
    }
    if (block == Block::blockSteel || block == Block::oreIron) {
        return harvestLevel >= 1;
    }
    if (block == Block::oreRedstone || block == Block::oreRedstoneGlowing) {
        return harvestLevel >= 2;
    }
    if (block != nullptr) {
        return block->material == Material::rock || block->material == Material::iron;
    }
    return false;
}
