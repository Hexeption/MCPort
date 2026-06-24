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
            glColor4ub(static_cast<GLubyte>(vertex.red), static_cast<GLubyte>(vertex.green),
                       static_cast<GLubyte>(vertex.blue), static_cast<GLubyte>(vertex.alpha));
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
    this->alpha = 255;
}

void Tessellator::setColorOpaque_I(const int_t color) {
    setColorOpaque(color >> 16 & 255, color >> 8 & 255, color & 255);
}

void Tessellator::setColorOpaque_F(const float red, const float green, const float blue) {
    setColorOpaque(static_cast<int_t>(red * 255.0f), static_cast<int_t>(green * 255.0f),
                   static_cast<int_t>(blue * 255.0f));
}

void Tessellator::setColorRGBA_F(const float red, const float green, const float blue, const float alpha) {
    hasColor = true;
    this->red = static_cast<int_t>(red * 255.0f);
    this->green = static_cast<int_t>(green * 255.0f);
    this->blue = static_cast<int_t>(blue * 255.0f);
    this->alpha = static_cast<int_t>(alpha * 255.0f);
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
        alpha,
        hasColor
    });
}
