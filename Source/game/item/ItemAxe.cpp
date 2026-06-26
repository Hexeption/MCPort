//
// Created by Keir Davis on 26/06/2026.
//

#include "ItemAxe.h"

#include "game/block/Block.h"

ItemAxe::ItemAxe(int_t id, int_t material)
    : ItemTool(id, 3, material, {
        Block::planks, Block::bookshelf, Block::wood, Block::chest
    }) {}
