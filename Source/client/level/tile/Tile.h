//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_TILE_H
#define MCPORT_TILE_H

#include "java/Type.h"
#include "client/phys/AABB.h"

class Level;
class Random;
class Tesselator;
class GrassTile;
class DirtTile;
class Bush;
class ParticleEngine;

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

    virtual void render(Tesselator &t, Level &level, int_t layer, int_t x, int_t y, int_t z);

    virtual void renderFace(Tesselator &t, int_t x, int_t y, int_t z, int_t face);

    virtual void renderFaceNoTexture(Tesselator &t, int_t x, int_t y, int_t z, int_t face);

    virtual AABB getTileAABB(int x, int y, int z);

    virtual AABB *getAABB(int_t x, int_t y, int_t z);

    virtual bool blocksLight();

    virtual bool isSolid();

    virtual void tick(Level *level, int_t x, int_t y, int_t z, Random &random);

    virtual void destroy(Level &level, int x, int y, int z, ParticleEngine &particleEngine);

private:
    virtual bool shouldRenderFace(Level &level, int x, int y, int z, int layer);

protected:
    virtual int_t getTexture(int_t face);
};

#endif //MCPORT_TILE_H
