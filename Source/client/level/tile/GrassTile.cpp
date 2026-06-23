//
// Created by Keir Davis on 23/06/2026.
//

#include "GrassTile.h"

#include "DirtTile.h"
#include "client/level/Level.h"

void GrassTile::tick(Level *level, int_t x, int_t y, int_t z, Random &random) {
    if (!level->isLit(x, y, z)) {
        level->setTile(x, y, z, Tile::dirt.id);
    } else {
        for (int i = 0; i < 4; ++i) {
            int xt = x + random.nextInt(3) - 1;
            int yt = y + random.nextInt(5) - 3;
            int zt = z + random.nextInt(3) - 1;
            if (level->getTile(xt, yt, zt) == Tile::dirt.id && level->isLit(xt, yt, zt)) {
                level->setTile(xt, yt, zt, Tile::grass.id);
            }
        }
    }
}

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
