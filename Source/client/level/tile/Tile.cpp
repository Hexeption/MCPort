//
// Created by Keir Davis on 23/06/2026.
//

#include "Tile.h"

#include "GrassTile.h"
#include "DirtTile.h"
#include "Bush.h"
#include "client/level/Level.h"
#include "client/level/Tesselator.h"
#include "client/particle/Particle.h"
#include "client/particle/ParticleEngine.h"

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

void Tile::render(Tesselator &t, Level &level, int_t layer, int_t x, int_t y, int_t z) {
    float c1 = 1.0F;
    float c2 = 0.8F;
    float c3 = 0.6F;
    if (this->shouldRenderFace(level, x, y - 1, z, layer)) {
        t.color(c1, c1, c1);
        this->renderFace(t, x, y, z, 0);
    }

    if (this->shouldRenderFace(level, x, y + 1, z, layer)) {
        t.color(c1, c1, c1);
        this->renderFace(t, x, y, z, 1);
    }

    if (this->shouldRenderFace(level, x, y, z - 1, layer)) {
        t.color(c2, c2, c2);
        this->renderFace(t, x, y, z, 2);
    }

    if (this->shouldRenderFace(level, x, y, z + 1, layer)) {
        t.color(c2, c2, c2);
        this->renderFace(t, x, y, z, 3);
    }

    if (this->shouldRenderFace(level, x - 1, y, z, layer)) {
        t.color(c3, c3, c3);
        this->renderFace(t, x, y, z, 4);
    }

    if (this->shouldRenderFace(level, x + 1, y, z, layer)) {
        t.color(c3, c3, c3);
        this->renderFace(t, x, y, z, 5);
    }
}

bool Tile::shouldRenderFace(Level &level, int x, int y, int z, int layer) {
    return !level.isSolidTile(x, y, z) && level.isLit(x, y, z) ^ (layer == 1);
}

int_t Tile::getTexture(int_t face) {
    return this->tex;
}

void Tile::renderFace(Tesselator &t, int_t x, int_t y, int_t z, int_t face) {
    int_t tex = this->getTexture(face);
    float u0 = static_cast<float>(tex % 16) / 16.0F;
    float u1 = u0 + 0.0624375F;
    float v0 = static_cast<float>(tex / 16) / 16.0F;
    float v1 = v0 + 0.0624375F;
    float x0 = static_cast<float>(x) + 0.0F;
    float x1 = static_cast<float>(x) + 1.0F;
    float y0 = static_cast<float>(y) + 0.0F;
    float y1 = static_cast<float>(y) + 1.0F;
    float z0 = static_cast<float>(z) + 0.0F;
    float z1 = static_cast<float>(z) + 1.0F;
    if (face == 0) {
        t.vertexUV(x0, y0, z1, u0, v1);
        t.vertexUV(x0, y0, z0, u0, v0);
        t.vertexUV(x1, y0, z0, u1, v0);
        t.vertexUV(x1, y0, z1, u1, v1);
    }

    if (face == 1) {
        t.vertexUV(x1, y1, z1, u1, v1);
        t.vertexUV(x1, y1, z0, u1, v0);
        t.vertexUV(x0, y1, z0, u0, v0);
        t.vertexUV(x0, y1, z1, u0, v1);
    }

    if (face == 2) {
        t.vertexUV(x0, y1, z0, u1, v0);
        t.vertexUV(x1, y1, z0, u0, v0);
        t.vertexUV(x1, y0, z0, u0, v1);
        t.vertexUV(x0, y0, z0, u1, v1);
    }

    if (face == 3) {
        t.vertexUV(x0, y1, z1, u0, v0);
        t.vertexUV(x0, y0, z1, u0, v1);
        t.vertexUV(x1, y0, z1, u1, v1);
        t.vertexUV(x1, y1, z1, u1, v0);
    }

    if (face == 4) {
        t.vertexUV(x0, y1, z1, u1, v0);
        t.vertexUV(x0, y1, z0, u0, v0);
        t.vertexUV(x0, y0, z0, u0, v1);
        t.vertexUV(x0, y0, z1, u1, v1);
    }

    if (face == 5) {
        t.vertexUV(x1, y0, z1, u0, v1);
        t.vertexUV(x1, y0, z0, u1, v1);
        t.vertexUV(x1, y1, z0, u1, v0);
        t.vertexUV(x1, y1, z1, u0, v0);
    }
}

void Tile::renderFaceNoTexture(Tesselator &t, int_t x, int_t y, int_t z, int_t face) {
    float x0 = static_cast<float>(x) + 0.0F;
    float x1 = static_cast<float>(x) + 1.0F;
    float y0 = static_cast<float>(y) + 0.0F;
    float y1 = static_cast<float>(y) + 1.0F;
    float z0 = static_cast<float>(z) + 0.0F;
    float z1 = static_cast<float>(z) + 1.0F;
    if (face == 0) {
        t.vertex(x0, y0, z1);
        t.vertex(x0, y0, z0);
        t.vertex(x1, y0, z0);
        t.vertex(x1, y0, z1);
    }

    if (face == 1) {
        t.vertex(x1, y1, z1);
        t.vertex(x1, y1, z0);
        t.vertex(x0, y1, z0);
        t.vertex(x0, y1, z1);
    }

    if (face == 2) {
        t.vertex(x0, y1, z0);
        t.vertex(x1, y1, z0);
        t.vertex(x1, y0, z0);
        t.vertex(x0, y0, z0);
    }

    if (face == 3) {
        t.vertex(x0, y1, z1);
        t.vertex(x0, y0, z1);
        t.vertex(x1, y0, z1);
        t.vertex(x1, y1, z1);
    }

    if (face == 4) {
        t.vertex(x0, y1, z1);
        t.vertex(x0, y1, z0);
        t.vertex(x0, y0, z0);
        t.vertex(x0, y0, z1);
    }

    if (face == 5) {
        t.vertex(x1, y0, z1);
        t.vertex(x1, y0, z0);
        t.vertex(x1, y1, z0);
        t.vertex(x1, y1, z1);
    }
}

AABB Tile::getTileAABB(int x, int y, int z) {
    return AABB(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(x + 1),
                static_cast<float>(y + 1), static_cast<float>(z + 1));
}

AABB *Tile::getAABB(int_t x, int_t y, int_t z) {
    return new AABB(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z),
                    static_cast<float>(x + 1), static_cast<float>(y + 1), static_cast<float>(z + 1));
}

bool Tile::blocksLight() {
    return true;
}

bool Tile::isSolid() {
    return true;
}

void Tile::tick(Level *level, int_t x, int_t y, int_t z, Random &random) {
}

void Tile::destroy(Level &level, int x, int y, int z, ParticleEngine &particleEngine) {
    int_t SD = 4;

    for (int_t xx = 0; xx < SD; ++xx) {
        for (int_t yy = 0; yy < SD; ++yy) {
            for (int_t zz = 0; zz < SD; ++zz) {
                float xp = static_cast<float>(x) + (static_cast<float>(xx) + 0.5F) / static_cast<float>(SD);
                float yp = static_cast<float>(y) + (static_cast<float>(yy) + 0.5F) / static_cast<float>(SD);
                float zp = static_cast<float>(z) + (static_cast<float>(zz) + 0.5F) / static_cast<float>(SD);
                particleEngine.add(new Particle(
                    level,
                    xp,
                    yp,
                    zp,
                    xp - static_cast<float>(x) - 0.5F,
                    yp - static_cast<float>(y) - 0.5F,
                    zp - static_cast<float>(z) - 0.5F,
                    this->tex
                ));
            }
        }
    }
}
