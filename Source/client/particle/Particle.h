//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_PARTICLE_H
#define MCPORT_PARTICLE_H

#include "client/Entity.h"

class Tesselator;

class Particle : public Entity {
private:
    float xd = 0.0F;
    float yd = 0.0F;
    float zd = 0.0F;

public:
    int tex;

private:
    float uo = 0.0F;
    float vo = 0.0F;
    int age = 0;
    int lifetime = 0;
    float size = 0.0F;

public:
    Particle(Level &level, float x, float y, float z, float xa, float ya, float za, int tex);

    void tick() override;

    void render(Tesselator &t, float a, float xa, float ya, float za, float xa2, float za2);
};

#endif //MCPORT_PARTICLE_H
