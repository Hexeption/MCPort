//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityPlayerSP.h"

#include "MovementInputFromOptions.h"
#include "lwjgl/Keyboard.h"

EntityPlayerSP::EntityPlayerSP(World &world, GameSettings &gameSettings) : EntityPlayer(world),
                                                                           movementInput(
                                                                               std::make_unique<
                                                                                   MovementInputFromOptions>(
                                                                                   gameSettings)) {
}

void EntityPlayerSP::onLivingUpdate() {
    if (noclip) {
        const float speed = 0.3f;

        movementInput->updatePlayerMoveState(*this);

        motionX = 0.0;
        motionY = 0.0;
        motionZ = 0.0;

        moveFlying(movementInput->moveStrafe,
                   movementInput->moveForward,
                   speed);

        if (lwjgl::Keyboard::isKeyDown(lwjgl::Keyboard::KEY_SPACE))
            motionY += speed;

        if (lwjgl::Keyboard::isKeyDown(lwjgl::Keyboard::KEY_LSHIFT))
            motionY -= speed;

        posX += motionX;
        posY += motionY;
        posZ += motionZ;

        return;
    }
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
