//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_VERTEX_H
#define MCPORT_VERTEX_H

#include "Vec3.h"


class Vertex {
public:
    Vec3 pos;
    float u;
    float v;

    Vertex(float x, float y, float z, float u, float v);

    Vertex(const Vertex &vertex, float u, float v);

    Vertex(const Vec3 &pos, float u, float v);

    Vertex remap(float u, float v);
};


#endif //MCPORT_VERTEX_H
