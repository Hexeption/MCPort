//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_CHUNK_H
#define MCPORT_CHUNK_H

#include "client/phys/AABB.h"
#include "java/Type.h"

class Level;
class Player;
class Tesselator;

class Chunk {
public:
    AABB aabb;
    Level *level;
    int_t x0;
    int_t y0;
    int_t z0;
    int_t x1;
    int_t y1;
    int_t z1;
    float x;
    float y;
    float z;

private:
    bool dirty = true;
    int_t lists = -1;

public:
    long_t dirtiedTime = 0L;
    static Tesselator *t;
    static int_t updates;

private:
    static long_t totalTime;
    static int_t totalUpdates;

    void rebuild(int_t layer);

public:
    Chunk(Level &level, int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1);

    void rebuild();

    void render(int_t layer);

    void setDirty();

    bool isDirty() const;

    float distanceToSqr(const Player &player) const;
};


#endif //MCPORT_CHUNK_H
