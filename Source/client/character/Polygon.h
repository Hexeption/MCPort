//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_POLYGON_H
#define MCPORT_POLYGON_H

#include <vector>

#include "Vertex.h"

class Polygon {
public:
    std::vector<Vertex> vertices;
    int vertexCount = 0;

    explicit Polygon(std::vector<Vertex> vertices);

    Polygon(std::vector<Vertex> vertices, int u0, int v0, int u1, int v1);

    void render();
};

#endif //MCPORT_POLYGON_H
