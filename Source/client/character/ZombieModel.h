//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_ZOMBIEMODEL_H
#define MCPORT_ZOMBIEMODEL_H

#include "Cube.h"

class ZombieModel {
public:
    Cube head;
    Cube body;
    Cube arm0;
    Cube arm1;
    Cube leg0;
    Cube leg1;

    ZombieModel();

    void render(float time);
};

#endif //MCPORT_ZOMBIEMODEL_H
