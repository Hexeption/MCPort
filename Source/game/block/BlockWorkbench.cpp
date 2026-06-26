//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockWorkbench.h"

#include "game/entity/EntityPlayer.h"

BlockWorkbench::BlockWorkbench(int_t id) : Block(id, Material::wood) {
    blockIndexInTexture = 59;
}

int_t BlockWorkbench::getBlockTextureFromSide(int_t side) const {
    if (side == 1) {
        return blockIndexInTexture - 16;
    }
    if (side == 0) {
        return Block::planks->getBlockTextureFromSide(0);
    }
    if (side == 2 || side == 4) {
        return blockIndexInTexture;
    }
    return blockIndexInTexture + 1;
}

bool BlockWorkbench::blockActivated(World &, int_t, int_t, int_t, EntityPlayer &player) {
    player.displayWorkbenchGUI();
    return true;
}
