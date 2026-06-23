//
// Created by Keir Davis on 23/06/2026.
//

#include "Polygon.h"

#include <glad/glad.h>
#include <utility>

Polygon::Polygon(std::vector<Vertex> vertices)
    : vertices(std::move(vertices)),
      vertexCount(static_cast<int>(this->vertices.size())) {
}

Polygon::Polygon(std::vector<Vertex> vertices, int u0, int v0, int u1, int v1)
    : Polygon(std::move(vertices)) {
    this->vertices[0] = this->vertices[0].remap(static_cast<float>(u1), static_cast<float>(v0));
    this->vertices[1] = this->vertices[1].remap(static_cast<float>(u0), static_cast<float>(v0));
    this->vertices[2] = this->vertices[2].remap(static_cast<float>(u0), static_cast<float>(v1));
    this->vertices[3] = this->vertices[3].remap(static_cast<float>(u1), static_cast<float>(v1));
}

void Polygon::render() {
    glColor3f(1.0F, 1.0F, 1.0F);

    for (int i = 3; i >= 0; --i) {
        Vertex &v = this->vertices[i];
        glTexCoord2f(v.u / 63.999F, v.v / 31.999F);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
}
