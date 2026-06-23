//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_GRASSTILE_H
#define MCPORT_GRASSTILE_H

#include "Tile.h"

class GrassTile : public Tile {
public:
    explicit GrassTile(int id);

    void tick(Level *level, int_t x, int_t y, int_t z, Random &random) override;

protected:
    int_t getTexture(int_t face) override;
};

#endif //MCPORT_GRASSTILE_H
