//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_PLAYER_H
#define MCPORT_PLAYER_H

#include "Entity.h"

class Player : public Entity {
public:
    explicit Player(Level &level);

    void tick() override;
};

#endif //MCPORT_PLAYER_H
