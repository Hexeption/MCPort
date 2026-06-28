//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_MOVEMENTINPUTFROMOPTIONS_H
#define MCPORT_MOVEMENTINPUTFROMOPTIONS_H

#include <array>

#include "MovementInput.h"

#include "game/client/options/GameSettings.h"

class MovementInputFromOptions : public MovementInput {
public:
    explicit MovementInputFromOptions(GameSettings &gameSettings);

    void updatePlayerMoveState(EntityPlayer &player) override;

    void resetKeyState() override;

    void checkKeyForMovementInput(int_t key, bool pressed) override;

private:
    std::array<bool, 10> movementKeyStates{};
    GameSettings &gameSettings;
};

#endif //MCPORT_MOVEMENTINPUTFROMOPTIONS_H
