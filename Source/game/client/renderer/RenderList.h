//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_RENDERLIST_H
#define MCPORT_RENDERLIST_H

#include <vector>

#include "java/Type.h"

class RenderList {
private:
    int_t posX = 0;
    int_t posY = 0;
    int_t posZ = 0;
    float playerPosX = 0.0f;
    float playerPosY = 0.0f;
    float playerPosZ = 0.0f;
    std::vector<int_t> buffers;
    bool active = false;
    bool isCached = false;

public:
    void setLocation(int_t x, int_t y, int_t z, double playerX, double playerY, double playerZ);

    bool isRenderedAt(int_t x, int_t y, int_t z) const;

    void render(int_t buffer);

    void render();

    void reset();
};

#endif //MCPORT_RENDERLIST_H
