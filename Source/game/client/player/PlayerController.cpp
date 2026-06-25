//
// Created by Keir Davis on 25/06/2026.
//

#include "PlayerController.h"

#include "game/block/Block.h"
#include "game/client/Minecraft.h"
#include "game/entity/EntityPlayer.h"
#include "game/world/World.h"

PlayerController::PlayerController(Minecraft &minecraft) : mc(minecraft) {
}

void PlayerController::onWorldChange(World &) {
}

void PlayerController::clickBlock(const int_t x, const int_t y, const int_t z, const int_t side) {
    sendBlockRemoved(x, y, z, side);
}

bool PlayerController::sendBlockRemoved(const int_t x, const int_t y, const int_t z, int_t) {
    if (mc.theWorld == nullptr) {
        return false;
    }

    World &world = *mc.theWorld;
    const int_t blockId = world.getBlockId(x, y, z);
    Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    const int_t metadata = world.getBlockMetadata(x, y, z);
    const bool removed = world.setBlockWithNotify(x, y, z, 0);
    if (block != nullptr && removed) {
        block->onBlockDestroyedByPlayer(world, x, y, z, metadata);
    }

    return removed;
}

void PlayerController::sendBlockRemoving(int_t, int_t, int_t, int_t) {
}

void PlayerController::resetBlockRemoving() {
}

void PlayerController::setPartialTime(float) {
}

float PlayerController::getBlockReachDistance() {
    return 5.0f;
}

void PlayerController::flipPlayer(EntityPlayer &) {
}

void PlayerController::onUpdate() {
}

bool PlayerController::shouldDrawHUD() {
    return true;
}

void PlayerController::onRespawn(EntityPlayer &) {
}
