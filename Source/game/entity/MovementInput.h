//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_MOVEMENTINPUT_H
#define MCPORT_MOVEMENTINPUT_H

#include "java/Type.h"

class EntityPlayer;

class MovementInput {
public:
    float moveStrafe = 0.0f;
    float moveForward = 0.0f;
    bool unused = false;
    bool jump = false;

    virtual ~MovementInput() = default;

    virtual void updatePlayerMoveState(EntityPlayer &player);

    virtual void resetKeyState();

    virtual void checkKeyForMovementInput(int_t key, bool pressed);
};

#endif //MCPORT_MOVEMENTINPUT_H
