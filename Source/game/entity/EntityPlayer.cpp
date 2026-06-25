//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityPlayer.h"

#include <algorithm>
#include <cmath>

#include "game/world/World.h"

EntityPlayer::EntityPlayer(World &world) : EntityLiving(world) {
    yOffset = 1.62f;
    setLocationAndAngles(static_cast<double>(world.spawnX) + 0.5, static_cast<double>(world.spawnY + 1),
                         static_cast<double>(world.spawnZ) + 0.5, 0.0f, 0.0f);
    health = 20;
}

void EntityPlayer::preparePlayerToSpawn() {
    yOffset = 1.62f;
    setSize(0.6f, 1.8f);
    EntityLiving::preparePlayerToSpawn();
    health = 20;
}

void EntityPlayer::onLivingUpdate() {
    prevCameraYaw = cameraYaw;
    EntityLiving::onLivingUpdate();

    float horizontalMotion = static_cast<float>(std::sqrt(motionX * motionX + motionZ * motionZ));
    float pitchMotion = static_cast<float>(std::atan(-motionY * 0.2) * 15.0);
    horizontalMotion = std::min(horizontalMotion, 0.1f);
    if (!onGround || health <= 0) {
        horizontalMotion = 0.0f;
    }
    if (onGround || health <= 0) {
        pitchMotion = 0.0f;
    }
    cameraYaw += (horizontalMotion - cameraYaw) * 0.4f;
    cameraPitch += (pitchMotion - cameraPitch) * 0.8f;
}

float EntityPlayer::getCurrentPlayerStrVsBlock(const Block &) {
    return 1.0f;
}

bool EntityPlayer::canHarvestBlock(const Block &) {
    return true;
}

void EntityPlayer::swingItem() {
    swingProgressInt = -1;
    isSwinging = true;
}
