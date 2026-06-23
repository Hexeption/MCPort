//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_TILE_H
#define MCPORT_TILE_H

#include "java/Type.h"
#include "client/phys/AABB.h"

class Level;
class Random;
class GrassTile;
class DirtTile;
class Bush;

class Tile {
public:
    static Tile *tiles[256];
    static Tile *empty;
    static Tile rock;
    static GrassTile grass;
    static DirtTile dirt;
    static Tile stoneBrick;
    static Tile wood;
    static Bush bush;

    int tex = 0;
    const int id;

protected:
    explicit Tile(int id);

    Tile(int id, int tex);

public:
    virtual ~Tile() = default;

    virtual AABB getAABB(int_t x, int_t y, int_t z);

    virtual void tick(Level *level, int_t x, int_t y, int_t z, Random &random);

    virtual bool blocksLight();

    virtual bool isSolid();
};

#endif //MCPORT_TILE_H
