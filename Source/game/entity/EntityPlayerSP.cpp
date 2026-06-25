//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityPlayerSP.h"

#include "MovementInputFromOptions.h"

EntityPlayerSP::EntityPlayerSP(World &world, GameSettings &gameSettings) : EntityPlayer(world),
                                                                           movementInput(
                                                                               std::make_unique<MovementInputFromOptions>(
                                                                                   gameSettings)) {
}

void EntityPlayerSP::onLivingUpdate() {
    movementInput->updatePlayerMoveState(*this);
    moveStrafing = movementInput->moveStrafe;
    moveForward = movementInput->moveForward;
    isJumping = movementInput->jump;
    EntityPlayer::onLivingUpdate();
}

void EntityPlayerSP::resetPlayerKeyState() {
    movementInput->resetKeyState();
}

void EntityPlayerSP::handleKeyPress(const int_t key, const bool pressed) {
    movementInput->checkKeyForMovementInput(key, pressed);
}
