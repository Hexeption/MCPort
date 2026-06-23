//
// Created by Keir Davis on 23/06/2026.
//

#include "GrassTile.h"

GrassTile::GrassTile(int id)
    : Tile(id) {
    this->tex = 3;
}

int_t GrassTile::getTexture(int_t face) {
    if (face == 1) {
        return 0;
    }

    return face == 0 ? 2 : 3;
}
