//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_CUBE_H
#define MCPORT_CUBE_H

#include <vector>

#include "Polygon.h"
#include "Vertex.h"
#include "java/Type.h"

class Cube {
private:
    std::vector<Vertex> vertices;
    std::vector<Polygon> polygons;
    int xTexOffs;
    int yTexOffs;
    bool compiled = false;
    int_t list = 0;

    void compile();

public:
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
    float xRot = 0.0F;
    float yRot = 0.0F;
    float zRot = 0.0F;

    Cube(int xTexOffs, int yTexOffs);

    void setTexOffs(int xTexOffs, int yTexOffs);

    void addBox(float x0, float y0, float z0, int w, int h, int d);

    void setPos(float x, float y, float z);

    void render();
};

#endif //MCPORT_CUBE_H
