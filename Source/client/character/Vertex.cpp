//
// Created by Keir Davis on 23/06/2026.
//

#include "Vertex.h"

Vertex::Vertex(float x, float y, float z, float u, float v)
    : Vertex(Vec3(x, y, z), u, v) {
}

Vertex::Vertex(const Vertex &vertex, float u, float v)
    : pos(vertex.pos),
      u(u),
      v(v) {
}

Vertex::Vertex(const Vec3 &pos, float u, float v)
    : pos(pos),
      u(u),
      v(v) {
}

Vertex Vertex::remap(float u, float v) {
    return Vertex(*this, u, v);
}
