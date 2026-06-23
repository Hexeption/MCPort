//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_LEVEL_H
#define MCPORT_LEVEL_H
#include <algorithm>
#include <vector>

#include "java/Type.h"

#include "LevelListener.h"
#include "client/phys/AABB.h"
#include "java/Random.h"

class Level {
private:
    static constexpr int_t TILE_UPDATE_INTERVAL = 400;

public:
    int_t width;
    int_t height;
    int_t depth;

private:
    std::vector<byte_t> blocks;
    std::vector<int_t> lightDepths;
    std::vector<LevelListener *> levelListeners;
    Random random = Random();
    int_t unprocessed = 0;

public:
    Level(int_t width, int_t height, int_t depth);

    bool load();

    bool save();

    void calcLightDepths(int_t x0, int_t y0, int_t x1, int_t y1);

    void addListener(LevelListener &listener);

    void removeListener(LevelListener &listener);

    bool isLightBlocker(int_t x, int_t y, int_t z);

    std::vector<AABB *> &getCubes(AABB aABB);

    bool setTile(int_t x, int_t y, int_t z, int_t type);

    bool isLit(int_t x, int_t y, int_t z);

    int getTile(int_t x, int_t y, int_t z);

    bool isSolidTile(int_t x, int_t y, int_t z);

    void tick();

private:
    void generateMap();
};


#endif //MCPORT_LEVEL_H
