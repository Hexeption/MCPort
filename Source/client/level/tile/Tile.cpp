//
// Created by Keir Davis on 23/06/2026.
//

#include "Tile.h"

#include "GrassTile.h"
#include "DirtTile.h"
#include "Bush.h"
#include "client/level/Level.h"
#include "java/Random.h"

Tile *Tile::tiles[256] = {};
Tile *Tile::empty = nullptr;
Tile Tile::rock(1, 1);
GrassTile Tile::grass(2);
DirtTile Tile::dirt(3, 2);
Tile Tile::stoneBrick(4, 16);
Tile Tile::wood(5, 4);
Bush Tile::bush(6);

Tile::Tile(int id)
    : id(id) {
    Tile::tiles[id] = this;
}

Tile::Tile(int id, int tex)
    : Tile(id) {
    this->tex = tex;
}

bool Tile::blocksLight() {
    return true;
}

bool Tile::isSolid() {
    return true;
}

AABB Tile::getAABB(int_t x, int_t y, int_t z) {
    return AABB(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z),
                static_cast<float>(x + 1), static_cast<float>(y + 1), static_cast<float>(z + 1));
}

void Tile::tick(Level *level, int_t x, int_t y, int_t z, Random &random) {
}
