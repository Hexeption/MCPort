//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYPLAYERSP_H
#define MCPORT_ENTITYPLAYERSP_H

#include <memory>

#include "EntityPlayer.h"
#include "MovementInput.h"

class GameSettings;

class EntityPlayerSP : public EntityPlayer {
public:
    std::unique_ptr<MovementInput> movementInput;

    EntityPlayerSP(World &world, GameSettings &gameSettings);

    void onLivingUpdate() override;

    void resetPlayerKeyState();

    void handleKeyPress(int_t key, bool pressed);
};

#endif //MCPORT_ENTITYPLAYERSP_H
