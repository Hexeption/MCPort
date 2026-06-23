//
// Created by Keir Davis on 23/06/2026.
//

#include "LevelRenderer.h"

#include <algorithm>
#include <cmath>

#include <glad/glad.h>

#include "client/Player.h"
#include "client/HitResult.h"
#include "client/Textures.h"
#include "client/level/DirtyChunkSorter.h"
#include "client/level/Frustum.h"
#include "client/level/Level.h"
#include "client/level/Tesselator.h"
#include "client/level/tile/Tile.h"
#include "java/System.h"

LevelRenderer::LevelRenderer(Level &level)
    : level(&level) {
    level.addListener(*this);
    this->xChunks = level.width / CHUNK_SIZE;
    this->yChunks = level.depth / CHUNK_SIZE;
    this->zChunks = level.height / CHUNK_SIZE;
    this->chunks.reserve(this->xChunks * this->yChunks * this->zChunks);

    for (int_t x = 0; x < this->xChunks; ++x) {
        for (int_t y = 0; y < this->yChunks; ++y) {
            for (int_t z = 0; z < this->zChunks; ++z) {
                int_t x0 = x * CHUNK_SIZE;
                int_t y0 = y * CHUNK_SIZE;
                int_t z0 = z * CHUNK_SIZE;
                int_t x1 = std::min((x + 1) * CHUNK_SIZE, level.width);
                int_t y1 = std::min((y + 1) * CHUNK_SIZE, level.depth);
                int_t z1 = std::min((z + 1) * CHUNK_SIZE, level.height);
                this->chunks.push_back(std::make_unique<Chunk>(level, x0, y0, z0, x1, y1, z1));
            }
        }
    }
}

std::vector<Chunk *> LevelRenderer::getAllDirtyChunks() {
    std::vector<Chunk *> dirty;
    for (const auto &chunk: this->chunks) {
        if (chunk->isDirty()) {
            dirty.push_back(chunk.get());
        }
    }

    return dirty;
}

void LevelRenderer::render(Player &player, int_t layer) {
    glEnable(GL_TEXTURE_2D);
    int id = Textures::loadTexture("/terrain.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(id));
    Frustum &frustum = Frustum::getFrustum();

    for (const auto &chunk: this->chunks) {
        if (frustum.isVisible(chunk->aabb)) {
            chunk->render(layer);
        }
    }

    glDisable(GL_TEXTURE_2D);
}

void LevelRenderer::updateDirtyChunks(Player &player) {
    std::vector<Chunk *> dirty = this->getAllDirtyChunks();
    if (dirty.empty()) {
        return;
    }

    Frustum &frustum = Frustum::getFrustum();
    std::sort(dirty.begin(), dirty.end(), DirtyChunkSorter(player, frustum));

    for (int_t i = 0; i < MAX_REBUILDS_PER_FRAME && i < static_cast<int_t>(dirty.size()); ++i) {
        dirty[i]->rebuild();
    }
}

void LevelRenderer::pick(Player &player, Frustum &frustum) {
    Tesselator &t = Tesselator::instance;
    float r = 3.0F;
    AABB box = player.bb.grow(r, r, r);
    int_t x0 = static_cast<int_t>(box.x0);
    int_t x1 = static_cast<int_t>(box.x1 + 1.0F);
    int_t y0 = static_cast<int_t>(box.y0);
    int_t y1 = static_cast<int_t>(box.y1 + 1.0F);
    int_t z0 = static_cast<int_t>(box.z0);
    int_t z1 = static_cast<int_t>(box.z1 + 1.0F);
    glInitNames();
    glPushName(0);
    glPushName(0);

    for (int_t x = x0; x < x1; ++x) {
        glLoadName(static_cast<GLuint>(x));
        glPushName(0);

        for (int_t y = y0; y < y1; ++y) {
            glLoadName(static_cast<GLuint>(y));
            glPushName(0);

            for (int_t z = z0; z < z1; ++z) {
                Tile *tile = Tile::tiles[this->level->getTile(x, y, z)];
                if (tile != nullptr && frustum.isVisible(tile->getTileAABB(x, y, z))) {
                    glLoadName(static_cast<GLuint>(z));
                    glPushName(0);

                    for (int_t i = 0; i < 6; ++i) {
                        glLoadName(static_cast<GLuint>(i));
                        t.init();
                        tile->renderFaceNoTexture(t, x, y, z, i);
                        t.flush();
                    }

                    glPopName();
                }
            }

            glPopName();
        }

        glPopName();
    }

    glPopName();
    glPopName();
}

void LevelRenderer::renderHit(HitResult &h) {
    Tesselator &t = Tesselator::instance;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(
        1.0F,
        1.0F,
        1.0F,
        (std::sin(static_cast<double>(System::currentTimeMillis()) / 100.0) * 0.2F + 0.4F) * 0.5F
    );
    t.init();
    Tile::rock.renderFaceNoTexture(t, h.x, h.y, h.z, h.f);
    t.flush();
    glDisable(GL_BLEND);
}

void LevelRenderer::setDirty(int_t x0, int_t y0, int_t z0, int_t x1, int_t y1, int_t z1) {
    x0 /= CHUNK_SIZE;
    x1 /= CHUNK_SIZE;
    y0 /= CHUNK_SIZE;
    y1 /= CHUNK_SIZE;
    z0 /= CHUNK_SIZE;
    z1 /= CHUNK_SIZE;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 >= this->xChunks) x1 = this->xChunks - 1;
    if (y1 >= this->yChunks) y1 = this->yChunks - 1;
    if (z1 >= this->zChunks) z1 = this->zChunks - 1;

    for (int_t x = x0; x <= x1; ++x) {
        for (int_t y = y0; y <= y1; ++y) {
            for (int_t z = z0; z <= z1; ++z) {
                this->chunks[(x + y * this->xChunks) * this->zChunks + z]->setDirty();
            }
        }
    }
}

void LevelRenderer::tileChanged(int x, int y, int z) {
    this->setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void LevelRenderer::lightColumnChanged(int x, int z, int y0, int y1) {
    this->setDirty(x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
}

void LevelRenderer::allChanged() {
    this->setDirty(0, 0, 0, this->level->width, this->level->depth, this->level->height);
}
