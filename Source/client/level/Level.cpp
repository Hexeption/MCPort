//
// Created by Keir Davis on 23/06/2026.
//

#include "Level.h"

#include "PerlinNoiseFilter.h"
#include "tile/GrassTile.h"
#include "tile/DirtTile.h"
#include "tile/Tile.h"

#include <zlib.h>

Level::Level(int_t width, int_t height, int_t depth) {
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->blocks = std::vector<byte_t>(width * height * depth);
    this->lightDepths = std::vector<int_t>(width * height);
    bool mapLoaded = this->load();
    if (!mapLoaded) {
        this->generateMap();
    }

    this->calcLightDepths(0, 0, width, height);
}

bool Level::load() {
    gzFile file = gzopen("level.dat", "rb");
    if (file == nullptr) {
        return false;
    }

    int expected = static_cast<int>(this->blocks.size());
    int readBytes = gzread(file, this->blocks.data(), expected);
    gzclose(file);
    if (readBytes != expected) {
        return false;
    }

    this->calcLightDepths(0, 0, this->width, this->height);
    for (LevelListener *listener: this->levelListeners) {
        listener->allChanged();
    }
    return true;
}

bool Level::save() {
    gzFile file = gzopen("level.dat", "wb");
    if (file == nullptr) {
        return false;
    }

    int expected = static_cast<int>(this->blocks.size());
    int written = gzwrite(file, this->blocks.data(), expected);
    gzclose(file);
    return written == expected;
}

void Level::calcLightDepths(int_t x0, int_t y0, int_t x1, int_t y1) {
    for (int x = x0; x < x0 + x1; ++x) {
        for (int z = y0; z < y0 + y1; ++z) {
            int oldDepth = this->lightDepths[x + z * this->width];

            int y;
            for (y = this->depth - 1; y > 0 && !this->isLightBlocker(x, y, z); --y) {
            }

            this->lightDepths[x + z * this->width] = y;
            if (oldDepth != y) {
                int yl0 = oldDepth < y ? oldDepth : y;
                int yl1 = oldDepth > y ? oldDepth : y;

                for (LevelListener *listener: this->levelListeners) {
                    listener->lightColumnChanged(x, z, yl0, yl1);
                }
            }
        }
    }
}

void Level::addListener(LevelListener &listener) {
    this->levelListeners.push_back(&listener);
}

void Level::removeListener(LevelListener &listener) {
    auto it = std::remove(this->levelListeners.begin(), this->levelListeners.end(), &listener);
    this->levelListeners.erase(it, this->levelListeners.end());
}

bool Level::isLightBlocker(int_t x, int_t y, int_t z) {
    Tile *tile = Tile::tiles[this->getTile(x, y, z)];
    return tile != nullptr && tile->blocksLight();
}

std::vector<AABB *> &Level::getCubes(AABB aABB) {
    static std::vector<AABB *> cubes;
    cubes.clear();

#if MCPORT_ENABLE_EDGE_BARRIER
    constexpr float barrierThickness = 1.0F;
    constexpr float barrierBottom = -1024.0F;
    constexpr float barrierTop = 1024.0F;
    float levelWidth = static_cast<float>(this->width);
    float levelHeight = static_cast<float>(this->height);

    if (aABB.x0 < 0.0F) {
        cubes.push_back(new AABB(-barrierThickness, barrierBottom, 0.0F, 0.0F, barrierTop, levelHeight));
    }

    if (aABB.x1 > levelWidth) {
        cubes.push_back(new AABB(levelWidth, barrierBottom, 0.0F, levelWidth + barrierThickness, barrierTop, levelHeight));
    }

    if (aABB.z0 < 0.0F) {
        cubes.push_back(new AABB(0.0F, barrierBottom, -barrierThickness, levelWidth, barrierTop, 0.0F));
    }

    if (aABB.z1 > levelHeight) {
        cubes.push_back(new AABB(0.0F, barrierBottom, levelHeight, levelWidth, barrierTop, levelHeight + barrierThickness));
    }
#endif

    int_t x0 = static_cast<int_t>(aABB.x0);
    int_t x1 = static_cast<int_t>(aABB.x1 + 1.0F);
    int_t y0 = static_cast<int_t>(aABB.y0);
    int_t y1 = static_cast<int_t>(aABB.y1 + 1.0F);
    int_t z0 = static_cast<int_t>(aABB.z0);
    int_t z1 = static_cast<int_t>(aABB.z1 + 1.0F);

    if (x0 < 0) {
        x0 = 0;
    }

    if (y0 < 0) {
        y0 = 0;
    }

    if (z0 < 0) {
        z0 = 0;
    }

    if (x1 > this->width) {
        x1 = this->width;
    }

    if (y1 > this->depth) {
        y1 = this->depth;
    }

    if (z1 > this->height) {
        z1 = this->height;
    }

    for (int_t x = x0; x < x1; ++x) {
        for (int_t y = y0; y < y1; ++y) {
            for (int_t z = z0; z < z1; ++z) {
                Tile *tile = Tile::tiles[this->getTile(x, y, z)];
                if (tile != nullptr) {
                    AABB *aabb = tile->getAABB(x, y, z);
                    if (aabb != nullptr) {
                        cubes.push_back(aabb);
                    }
                }
            }
        }
    }

    return cubes;
}

bool Level::setTile(int_t x, int_t y, int_t z, int_t type) {
    if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
        int_t index = (y * this->height + z) * this->width + x;
        if (type == this->blocks[index]) {
            return false;
        }

        this->blocks[index] = static_cast<byte_t>(type);
        this->calcLightDepths(x, z, 1, 1);

        for (LevelListener *listener: this->levelListeners) {
            listener->tileChanged(x, y, z);
        }

        return true;
    }

    return false;
}

bool Level::isLit(int_t x, int_t y, int_t z) {
    if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
        return y >= this->lightDepths[x + z * this->width];
    }

    return true;
}

int Level::getTile(int_t x, int_t y, int_t z) {
    if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
        return this->blocks[(y * this->height + z) * this->width + x];
    }

    return 0;
}

bool Level::isSolidTile(int_t x, int_t y, int_t z) {
    Tile *tile = Tile::tiles[this->getTile(x, y, z)];
    return tile != nullptr && tile->isSolid();
}

void Level::tick() {
    this->unprocessed += this->width * this->height * this->depth;
    int_t ticks = this->unprocessed / TILE_UPDATE_INTERVAL;
    this->unprocessed -= ticks * TILE_UPDATE_INTERVAL;

    for (int_t i = 0; i < ticks; ++i) {
        int_t x = this->random.nextInt(this->width);
        int_t y = this->random.nextInt(this->depth);
        int_t z = this->random.nextInt(this->height);
        Tile *tile = Tile::tiles[this->getTile(x, y, z)];
        if (tile != nullptr) {
            tile->tick(this, x, y, z, this->random);
        }
    }
}

void Level::generateMap() {
    int_t w = this->width;
    int_t h = this->height;
    int_t d = this->depth;
    std::vector<int_t> heightmap1 = PerlinNoiseFilter(0).read(w, h);
    std::vector<int_t> heightmap2 = PerlinNoiseFilter(0).read(w, h);
    std::vector<int_t> cf = PerlinNoiseFilter(1).read(w, h);
    std::vector<int_t> rockMap = PerlinNoiseFilter(1).read(w, h);

    for (int_t x = 0; x < w; ++x) {
        for (int_t y = 0; y < d; ++y) {
            for (int_t z = 0; z < h; ++z) {
                int_t dh1 = heightmap1[x + z * this->width];
                int_t dh2 = heightmap2[x + z * this->width];
                int_t cfh = cf[x + z * this->width];
                if (cfh < 128) {
                    dh2 = dh1;
                }

                int_t dh = dh1;
                if (dh2 > dh1) {
                    dh = dh2;
                }

                dh = dh / 8 + d / 3;
                int_t rh = rockMap[x + z * this->width] / 8 + d / 3;
                if (rh > dh - 2) {
                    rh = dh - 2;
                }

                int_t i = (y * this->height + z) * this->width + x;
                int_t id = 0;
                if (y == dh) {
                    id = Tile::grass.id;
                }

                if (y < dh) {
                    id = Tile::dirt.id;
                }

                if (y <= rh) {
                    id = Tile::rock.id;
                }

                this->blocks[i] = (byte_t) id;
            }
        }
    }
}
