//
// Created by Keir Davis on 23/06/2026.
//

#include "Cube.h"

#include <glad/glad.h>

Cube::Cube(int xTexOffs, int yTexOffs)
    : xTexOffs(xTexOffs),
      yTexOffs(yTexOffs) {
}

void Cube::setTexOffs(int xTexOffs, int yTexOffs) {
    this->xTexOffs = xTexOffs;
    this->yTexOffs = yTexOffs;
}

void Cube::addBox(float x0, float y0, float z0, int w, int h, int d) {
    this->vertices.clear();
    this->polygons.clear();

    float x1 = x0 + static_cast<float>(w);
    float y1 = y0 + static_cast<float>(h);
    float z1 = z0 + static_cast<float>(d);
    Vertex u0(x0, y0, z0, 0.0F, 0.0F);
    Vertex u1(x1, y0, z0, 0.0F, 8.0F);
    Vertex u2(x1, y1, z0, 8.0F, 8.0F);
    Vertex u3(x0, y1, z0, 8.0F, 0.0F);
    Vertex l0(x0, y0, z1, 0.0F, 0.0F);
    Vertex l1(x1, y0, z1, 0.0F, 8.0F);
    Vertex l2(x1, y1, z1, 8.0F, 8.0F);
    Vertex l3(x0, y1, z1, 8.0F, 0.0F);

    this->vertices = {u0, u1, u2, u3, l0, l1, l2, l3};
    this->polygons.emplace_back(std::vector<Vertex>{l1, u1, u2, l2}, this->xTexOffs + d + w, this->yTexOffs + d, this->xTexOffs + d + w + d, this->yTexOffs + d + h);
    this->polygons.emplace_back(std::vector<Vertex>{u0, l0, l3, u3}, this->xTexOffs + 0, this->yTexOffs + d, this->xTexOffs + d, this->yTexOffs + d + h);
    this->polygons.emplace_back(std::vector<Vertex>{l1, l0, u0, u1}, this->xTexOffs + d, this->yTexOffs + 0, this->xTexOffs + d + w, this->yTexOffs + d);
    this->polygons.emplace_back(std::vector<Vertex>{u2, u3, l3, l2}, this->xTexOffs + d + w, this->yTexOffs + 0, this->xTexOffs + d + w + w, this->yTexOffs + d);
    this->polygons.emplace_back(std::vector<Vertex>{u1, u0, u3, u2}, this->xTexOffs + d, this->yTexOffs + d, this->xTexOffs + d + w, this->yTexOffs + d + h);
    this->polygons.emplace_back(std::vector<Vertex>{l0, l1, l2, l3}, this->xTexOffs + d + w + d, this->yTexOffs + d, this->xTexOffs + d + w + d + w, this->yTexOffs + d + h);
    this->compiled = false;
}

void Cube::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Cube::render() {
    if (!this->compiled) {
        this->compile();
    }

    float c = 57.29578F;
    glPushMatrix();
    glTranslatef(this->x, this->y, this->z);
    glRotatef(this->zRot * c, 0.0F, 0.0F, 1.0F);
    glRotatef(this->yRot * c, 0.0F, 1.0F, 0.0F);
    glRotatef(this->xRot * c, 1.0F, 0.0F, 0.0F);
    glCallList(this->list);
    glPopMatrix();
}

void Cube::compile() {
    this->list = glGenLists(1);
    glNewList(this->list, GL_COMPILE);
    glBegin(GL_QUADS);

    for (Polygon &polygon : this->polygons) {
        polygon.render();
    }

    glEnd();
    glEndList();
    this->compiled = true;
}
