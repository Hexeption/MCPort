//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_BUSH_H
#define MCPORT_BUSH_H

#include "Tile.h"

class Bush : public Tile {
public:
    explicit Bush(int id);

    void render(Tesselator &t, Level &level, int_t layer, int_t x, int_t y, int_t z) override;

    void tick(Level *level, int_t x, int_t y, int_t z, Random &random) override;

    AABB *getAABB(int_t x, int_t y, int_t z) override;

    bool blocksLight() override;

    bool isSolid() override;
};


#endif //MCPORT_BUSH_H
