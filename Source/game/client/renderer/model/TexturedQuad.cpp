//
// Created by Keir Davis on 26/06/2026.
//

#include "TexturedQuad.h"

#include <cmath>

#include <glad/glad.h>

TexturedQuad::TexturedQuad(PositionTexureVertex v0, PositionTexureVertex v1,
                           PositionTexureVertex v2, PositionTexureVertex v3) {
    vertexPositions[0] = v0;
    vertexPositions[1] = v1;
    vertexPositions[2] = v2;
    vertexPositions[3] = v3;
}

TexturedQuad::TexturedQuad(PositionTexureVertex v0, PositionTexureVertex v1,
                           PositionTexureVertex v2, PositionTexureVertex v3,
                           int u0, int v0i, int u1, int v1i) : TexturedQuad(v0, v1, v2, v3) {
    constexpr float uEps = 0.0015625f;
    constexpr float vEps = 0.003125f;
    vertexPositions[0] = vertexPositions[0].setTexturePosition(
        static_cast<float>(u1) / 64.0f - uEps,
        static_cast<float>(v0i) / 32.0f + vEps);
    vertexPositions[1] = vertexPositions[1].setTexturePosition(
        static_cast<float>(u0) / 64.0f + uEps,
        static_cast<float>(v0i) / 32.0f + vEps);
    vertexPositions[2] = vertexPositions[2].setTexturePosition(
        static_cast<float>(u0) / 64.0f + uEps,
        static_cast<float>(v1i) / 32.0f - vEps);
    vertexPositions[3] = vertexPositions[3].setTexturePosition(
        static_cast<float>(u1) / 64.0f - uEps,
        static_cast<float>(v1i) / 32.0f - vEps);
}

void TexturedQuad::flipFace() {
    PositionTexureVertex tmp0 = vertexPositions[0];
    PositionTexureVertex tmp1 = vertexPositions[1];
    vertexPositions[0] = vertexPositions[3];
    vertexPositions[1] = vertexPositions[2];
    vertexPositions[2] = tmp1;
    vertexPositions[3] = tmp0;
}

void TexturedQuad::draw(const float scale) const {
    const float dx = vertexPositions[1].x - vertexPositions[0].x;
    const float dy = vertexPositions[1].y - vertexPositions[0].y;
    const float dz = vertexPositions[1].z - vertexPositions[0].z;
    const float ex = vertexPositions[1].x - vertexPositions[2].x;
    const float ey = vertexPositions[1].y - vertexPositions[2].y;
    const float ez = vertexPositions[1].z - vertexPositions[2].z;
    float nx = ey * dz - ez * dy;
    float ny = ez * dx - ex * dz;
    float nz = ex * dy - ey * dx;
    const float len = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (len > 0.0f) { nx /= len; ny /= len; nz /= len; }

    glBegin(GL_QUADS);
    glNormal3f(nx, ny, nz);
    for (int i = 0; i < 4; ++i) {
        const PositionTexureVertex &v = vertexPositions[i];
        glTexCoord2f(v.u, v.v);
        glVertex3f(v.x * scale, v.y * scale, v.z * scale);
    }
    glEnd();
}
