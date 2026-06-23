//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_PARTICLEENGINE_H
#define MCPORT_PARTICLEENGINE_H

#include <vector>

#include "Particle.h"

class Player;

class ParticleEngine {
protected:
    Level *level = nullptr;

private:
    std::vector<Particle *> particles;

public:
    explicit ParticleEngine(Level &level);

    ~ParticleEngine();

    void add(Particle *p);

    void tick();

    void render(Player &player, float a, int layer);
};

#endif //MCPORT_PARTICLEENGINE_H
