//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_BUSH_H
#define MCPORT_BUSH_H

#include "Tile.h"

class Bush : public Tile {
public:
    explicit Bush(int id);

    bool blocksLight() override;

    bool isSolid() override;
};


#endif //MCPORT_BUSH_H
