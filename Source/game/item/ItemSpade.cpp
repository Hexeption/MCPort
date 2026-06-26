//
// Created by Keir Davis on 26/06/2026.
//

#include "ItemSpade.h"

#include "game/block/Block.h"

ItemSpade::ItemSpade(int_t id, int_t material)
    : ItemTool(id, 1, material, {
        Block::grass, Block::dirt, Block::sand, Block::gravel,
        Block::snow, Block::blockSnow, Block::blockClay
    }) {}

bool ItemSpade::canHarvestBlock(Block *block) {
    return block == Block::snow || block == Block::blockSnow;
}
