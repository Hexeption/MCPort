//
// Created by Keir Davis on 23/06/2026.
//

#include "Bush.h"

Bush::Bush(int id)
    : Tile(id) {
    this->tex = 15;
}

bool Bush::blocksLight() {
    return false;
}

bool Bush::isSolid() {
    return false;
}
