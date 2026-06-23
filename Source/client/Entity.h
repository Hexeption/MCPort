//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_ENTITY_H
#define MCPORT_ENTITY_H

#include "client/level/Level.h"
#include "client/phys/AABB.h"

class Entity {
protected:
    Level *level = nullptr;

public:
    float xo = 0.0F;
    float yo = 0.0F;
    float zo = 0.0F;
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
    float xd = 0.0F;
    float yd = 0.0F;
    float zd = 0.0F;
    float yRot = 0.0F;
    float xRot = 0.0F;
    AABB bb = AABB(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);
    bool onGround = false;
    bool removed = false;

protected:
    float heightOffset = 0.0F;
    float bbWidth = 0.6F;
    float bbHeight = 1.8F;

public:
    explicit Entity(Level &level);

    virtual ~Entity() = default;

    void remove();

    void turn(float xo, float yo);

    virtual void tick();

    void move(float xa, float ya, float za);

    void moveRelative(float xa, float za, float speed);

    bool isLit();

protected:
    void resetPos();

    void setSize(float w, float h);

    void setPos(float x, float y, float z);
};

#endif //MCPORT_ENTITY_H
