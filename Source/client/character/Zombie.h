//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_ZOMBIE_H
#define MCPORT_ZOMBIE_H

#include "ZombieModel.h"
#include "client/Entity.h"

class Zombie : public Entity {
private:
    static ZombieModel zombieModel;

public:
    using Entity::resetPos;

    float rot = 0.0F;
    float timeOffs = 0.0F;
    float speed = 0.0F;
    float rotA = 0.0F;

    Zombie(Level &level, float x, float y, float z);

    void tick() override;

    void render(float a);
};

#endif //MCPORT_ZOMBIE_H
