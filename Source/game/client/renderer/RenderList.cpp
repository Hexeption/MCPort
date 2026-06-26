//
// Created by Keir Davis on 26/06/2026.
//

#include "RenderList.h"

#include <glad/glad.h>

void RenderList::setLocation(const int_t x, const int_t y, const int_t z, const double playerX, const double playerY,
                             const double playerZ) {
    active = true;
    posX = x;
    posY = y;
    posZ = z;
    playerPosX = static_cast<float>(playerX);
    playerPosY = static_cast<float>(playerY);
    playerPosZ = static_cast<float>(playerZ);
}

bool RenderList::isRenderedAt(const int_t x, const int_t y, const int_t z) const {
    return active && x == posX && y == posY && z == posZ;
}

void RenderList::render(const int_t buffer) {
    buffers.push_back(buffer);
    if (buffers.size() == 65536) {
        render();
    }
}

void RenderList::render() {
    if (!active || buffers.empty()) {
        return;
    }

    if (!isCached) {
        isCached = true;
    }

    glPushMatrix();
    glTranslatef(static_cast<float>(posX) - playerPosX, static_cast<float>(posY) - playerPosY,
                 static_cast<float>(posZ) - playerPosZ);
    glCallLists(static_cast<GLsizei>(buffers.size()), GL_UNSIGNED_INT, buffers.data());
    glPopMatrix();
}

void RenderList::reset() {
    active = false;
    isCached = false;
    buffers.clear();
}
