//
// Created by Keir Davis on 25/06/2026.
//

#include "MovementInputFromOptions.h"

#include "EntityPlayer.h"
#include "game/client/options/GameSettings.h"

MovementInputFromOptions::MovementInputFromOptions(GameSettings &gameSettings) : gameSettings(gameSettings) {
}

void MovementInputFromOptions::checkKeyForMovementInput(const int_t key, const bool pressed) {
    int_t keyIndex = -1;
    if (key == gameSettings.keyBindForward.key) {
        keyIndex = 0;
    }
    if (key == gameSettings.keyBindBack.key) {
        keyIndex = 1;
    }
    if (key == gameSettings.keyBindLeft.key) {
        keyIndex = 2;
    }
    if (key == gameSettings.keyBindRight.key) {
        keyIndex = 3;
    }
    if (key == gameSettings.keyBindJump.key) {
        keyIndex = 4;
    }

    if (keyIndex >= 0) {
        movementKeyStates[keyIndex] = pressed;
    }
}

void MovementInputFromOptions::resetKeyState() {
    movementKeyStates.fill(false);
}

void MovementInputFromOptions::updatePlayerMoveState(EntityPlayer &) {
    moveStrafe = 0.0f;
    moveForward = 0.0f;
    if (movementKeyStates[0]) {
        ++moveForward;
    }
    if (movementKeyStates[1]) {
        --moveForward;
    }
    if (movementKeyStates[2]) {
        ++moveStrafe;
    }
    if (movementKeyStates[3]) {
        --moveStrafe;
    }

    jump = movementKeyStates[4];
}
