//
// Created by Keir Davis on 25/06/2026.
//

#include "PlayerControllerSP.h"

#include <memory>

#include "game/block/Block.h"
#include "game/client/Minecraft.h"
#include "game/entity/EntityAnimal.h"
#include "game/entity/EntityCow.h"
#include "game/entity/EntityPlayer.h"
#include "game/world/World.h"

namespace {
    bool isAnimalEntity(const Entity &entity) {
        return dynamic_cast<const EntityAnimal *>(&entity) != nullptr;
    }

    std::unique_ptr<EntityLiving> createCow(World &world) {
        return std::make_unique<EntityCow>(world);
    }
}

PlayerControllerSP::PlayerControllerSP(Minecraft &minecraft)
    : PlayerController(minecraft),
      animalSpawner(15, isAnimalEntity, std::vector<SpawnerAnimals::EntityFactory>{createCow}) {
}

void PlayerControllerSP::flipPlayer(EntityPlayer &player) {
    player.rotationYaw = -180.0f;
}

bool PlayerControllerSP::sendBlockRemoved(const int_t x, const int_t y, const int_t z, const int_t side) {
    if (mc.theWorld == nullptr) {
        return false;
    }

    const int_t blockId = mc.theWorld->getBlockId(x, y, z);
    const int_t blockMetadata = mc.theWorld->getBlockMetadata(x, y, z);
    const bool removed = PlayerController::sendBlockRemoved(x, y, z, side);
    Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    if (removed && block != nullptr && mc.thePlayer != nullptr && mc.thePlayer->canHarvestBlock(*block)) {
        Block::blocksList[blockId]->harvestBlock(*mc.theWorld, x, y, z, blockMetadata);
    }

    return removed;
}

void PlayerControllerSP::clickBlock(const int_t x, const int_t y, const int_t z, const int_t side) {
    if (mc.theWorld == nullptr || mc.thePlayer == nullptr) {
        return;
    }

    const int_t blockId = mc.theWorld->getBlockId(x, y, z);
    Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    if (block != nullptr && curBlockDamage == 0.0f) {
        block->onBlockClicked(*mc.theWorld, x, y, z, *mc.thePlayer);
    }

    if (block != nullptr && block->blockStrength(*mc.thePlayer) >= 1.0f) {
        sendBlockRemoved(x, y, z, side);
    }
}

void PlayerControllerSP::resetBlockRemoving() {
    curBlockDamage = 0.0f;
    blockHitWait = 0;
}

void PlayerControllerSP::sendBlockRemoving(const int_t x, const int_t y, const int_t z, const int_t side) {
    if (mc.theWorld == nullptr || mc.thePlayer == nullptr) {
        return;
    }

    if (blockHitWait > 0) {
        --blockHitWait;
        return;
    }

    if (x == curBlockX && y == curBlockY && z == curBlockZ) {
        const int_t blockId = mc.theWorld->getBlockId(x, y, z);
        if (blockId == 0) {
            return;
        }

        Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                           ? Block::blocksList[blockId]
                           : nullptr;
        if (block == nullptr) {
            return;
        }

        curBlockDamage += block->blockStrength(*mc.thePlayer);
        ++blockDestroySoundCounter;
        if (curBlockDamage >= 1.0f) {
            sendBlockRemoved(x, y, z, side);
            curBlockDamage = 0.0f;
            prevBlockDamage = 0.0f;
            blockDestroySoundCounter = 0.0f;
            blockHitWait = 5;
        }
    } else {
        curBlockDamage = 0.0f;
        prevBlockDamage = 0.0f;
        blockDestroySoundCounter = 0.0f;
        curBlockX = x;
        curBlockY = y;
        curBlockZ = z;
    }
}

void PlayerControllerSP::setPartialTime(const float partialTicks) {
    if (mc.renderGlobal == nullptr) {
        return;
    }

    mc.renderGlobal->damagePartialTime = curBlockDamage <= 0.0f
                                             ? 0.0f
                                             : prevBlockDamage + (curBlockDamage - prevBlockDamage) * partialTicks;
}

float PlayerControllerSP::getBlockReachDistance() {
    return 4.0f;
}

void PlayerControllerSP::onWorldChange(World &world) {
    PlayerController::onWorldChange(world);
}

void PlayerControllerSP::onUpdate() {
    prevBlockDamage = curBlockDamage;
    if (mc.theWorld != nullptr) {
        animalSpawner.onUpdate(*mc.theWorld);
    }
}
