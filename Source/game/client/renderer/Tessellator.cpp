//
// Created by Keir Davis on 24/06/2026.
//

#include "Tessellator.h"

#include <stdexcept>

#include <glad/glad.h>

Tessellator Tessellator::instance;

void Tessellator::draw() {
    if (!isDrawing) {
        throw std::runtime_error("Not tesselating!");
    }

    isDrawing = false;
    glBegin(static_cast<GLenum>(drawMode));
    for (const Vertex &vertex: vertices) {
        if (vertex.hasColor) {
            glColor3ub(static_cast<GLubyte>(vertex.red), static_cast<GLubyte>(vertex.green),
                       static_cast<GLubyte>(vertex.blue));
        }
        if (vertex.hasTexture) {
            glTexCoord2d(vertex.u, vertex.v);
        }
        glVertex3d(vertex.x, vertex.y, vertex.z);
    }
    glEnd();

    vertices.clear();
    hasTexture = false;
    hasColor = false;
}

void Tessellator::startDrawingQuads() {
    startDrawing(GL_QUADS);
}

void Tessellator::startDrawing(const int_t mode) {
    if (isDrawing) {
        throw std::runtime_error("Already tesselating!");
    }

    vertices.clear();
    isDrawing = true;
    drawMode = mode;
    hasTexture = false;
    hasColor = false;
}

void Tessellator::setTextureUV(const double u, const double v) {
    hasTexture = true;
    textureU = u;
    textureV = v;
}

void Tessellator::setColorOpaque(const int_t red, const int_t green, const int_t blue) {
    hasColor = true;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void Tessellator::setColorOpaque_I(const int_t color) {
    setColorOpaque(color >> 16 & 255, color >> 8 & 255, color & 255);
}

void Tessellator::addVertexWithUV(const double x, const double y, const double z, const double u, const double v) {
    setTextureUV(u, v);
    addVertex(x, y, z);
}

void Tessellator::addVertex(const double x, const double y, const double z) {
    if (!isDrawing) {
        throw std::runtime_error("Not tesselating!");
    }

    vertices.push_back(Vertex{
        x,
        y,
        z,
        textureU,
        textureV,
        hasTexture,
        red,
        green,
        blue,
        hasColor
    });
}
