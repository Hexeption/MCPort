//
// Created by Keir Davis on 23/06/2026.
//

#include "Chunk.h"

#include <glad/glad.h>

#include "client/Player.h"
#include "client/level/Level.h"
#include "client/level/Tesselator.h"
#include "client/level/tile/Tile.h"
#include "java/System.h"

Tesselator *Chunk::t = &Tesselator::instance;
int_t Chunk::updates = 0;
long_t Chunk::totalTime = 0L;
int_t Chunk::totalUpdates = 0;

Chunk::Chunk(Level &level, int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1)
    : aabb(static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(z0),
           static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(z1)),
      level(&level),
      x0(x0),
      y0(y0),
      z0(z0),
      x1(x1),
      y1(y1),
      z1(z1),
      x(static_cast<float>(x0 + x1) / 2.0F),
      y(static_cast<float>(y0 + y1) / 2.0F),
      z(static_cast<float>(z0 + z1) / 2.0F) {
    this->lists = glGenLists(2);
}

void Chunk::rebuild(int_t layer) {
    this->dirty = false;
    ++updates;
    long_t before = System::nanoTime();
    glNewList(this->lists + layer, GL_COMPILE);
    t->init();
    int_t tiles = 0;

    for (int_t x = this->x0; x < this->x1; ++x) {
        for (int_t y = this->y0; y < this->y1; ++y) {
            for (int_t z = this->z0; z < this->z1; ++z) {
                int_t tileId = this->level->getTile(x, y, z);
                Tile *tile = tileId > 0 ? Tile::tiles[tileId] : nullptr;
                if (tile != nullptr) {
                    tile->render(*t, *this->level, layer, x, y, z);
                    ++tiles;
                }
            }
        }
    }

    t->flush();
    glEndList();
    long_t after = System::nanoTime();
    if (tiles > 0) {
        totalTime += after - before;
        ++totalUpdates;
    }
}

void Chunk::rebuild() {
    this->rebuild(0);
    this->rebuild(1);
}

void Chunk::render(int_t layer) {
    glCallList(this->lists + layer);
}

void Chunk::setDirty() {
    if (!this->dirty) {
        this->dirtiedTime = System::currentTimeMillis();
    }

    this->dirty = true;
}

bool Chunk::isDirty() const {
    return this->dirty;
}

float Chunk::distanceToSqr(const Player &player) const {
    float xd = player.x - this->x;
    float yd = player.y - this->y;
    float zd = player.z - this->z;
    return xd * xd + yd * yd + zd * zd;
}
