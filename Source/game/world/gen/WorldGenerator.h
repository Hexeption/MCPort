//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_WORLDGENERATOR_H
#define MCPORT_WORLDGENERATOR_H
#include "game/world/World.h"
#include "java/Random.h"
#include "java/Type.h"


class WorldGenerator {
public:
    virtual ~WorldGenerator() = default;

    virtual bool generate(World &world, Random &random, int_t x, int_t y, int_t z) = 0;

    virtual void setScale(double x, double y, double z) {
    }
};


#endif //MCPORT_WORLDGENERATOR_H
