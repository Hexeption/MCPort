//
// Created by Keir Davis on 24/06/2026.
//

#include "Tessellator.h"

#include <stdexcept>

#include <glad/glad.h>

Tessellator Tessellator::instance;

int_t Tessellator::clampColor(int_t value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

Tessellator::Vertex Tessellator::makeCurrentVertex(double x, double y, double z) const {
    return Vertex{
        x + xOffset,
        y + yOffset,
        z + zOffset,
        textureU,
        textureV,
        hasTexture,
        red,
        green,
        blue,
        alpha,
        hasColor,
        normalX,
        normalY,
        normalZ,
        hasNormals
    };
}

void Tessellator::draw() {
    if (!isDrawing) {
        throw std::runtime_error("Not tesselating!");
    }

    isDrawing = false;

    if (!vertices.empty()) {
        glBegin(static_cast<GLenum>(drawMode));

        for (const Vertex &v: vertices) {
            if (v.hasColor) {
                glColor4ub(
                    static_cast<GLubyte>(v.red),
                    static_cast<GLubyte>(v.green),
                    static_cast<GLubyte>(v.blue),
                    static_cast<GLubyte>(v.alpha)
                );
            }

            if (v.hasNormal) {
                glNormal3f(v.normalX, v.normalY, v.normalZ);
            }

            if (v.hasTexture) {
                glTexCoord2d(v.u, v.v);
            }

            glVertex3d(v.x, v.y, v.z);
        }

        glEnd();
    }

    vertices.clear();
    addedVertices = 0;
}

void Tessellator::startDrawingQuads() {
    startDrawing(GL_QUADS);
}

void Tessellator::startDrawing(int_t mode) {
    if (isDrawing) {
        throw std::runtime_error("Already tesselating!");
    }

    vertices.clear();
    addedVertices = 0;
    isDrawing = true;
    drawMode = mode;

    hasTexture = false;
    hasColor = false;
    hasNormals = false;
    colorDisabled = false;
}

void Tessellator::setTextureUV(double u, double v) {
    hasTexture = true;
    textureU = u;
    textureV = v;
}

void Tessellator::setColorOpaque(int_t r, int_t g, int_t b) {
    setColorRGBA(r, g, b, 255);
}

void Tessellator::setColorRGBA(int_t r, int_t g, int_t b, int_t a) {
    if (colorDisabled) {
        return;
    }

    red = clampColor(r);
    green = clampColor(g);
    blue = clampColor(b);
    alpha = clampColor(a);
    hasColor = true;
}

void Tessellator::setColorOpaque_I(int_t color) {
    setColorOpaque(
        color >> 16 & 255,
        color >> 8 & 255,
        color & 255
    );
}

void Tessellator::setColorOpaque_F(float r, float g, float b) {
    setColorOpaque(
        static_cast<int_t>(r * 255.0f),
        static_cast<int_t>(g * 255.0f),
        static_cast<int_t>(b * 255.0f)
    );
}

void Tessellator::setColorRGBA_F(float r, float g, float b, float a) {
    setColorRGBA(
        static_cast<int_t>(r * 255.0f),
        static_cast<int_t>(g * 255.0f),
        static_cast<int_t>(b * 255.0f),
        static_cast<int_t>(a * 255.0f)
    );
}

void Tessellator::disableColor() {
    colorDisabled = true;
}

void Tessellator::setNormal(float x, float y, float z) {
    hasNormals = true;
    normalX = x;
    normalY = y;
    normalZ = z;
}

void Tessellator::setTranslationD(double x, double y, double z) {
    xOffset = x;
    yOffset = y;
    zOffset = z;
}

void Tessellator::setTranslationF(float x, float y, float z) {
    xOffset += static_cast<double>(x);
    yOffset += static_cast<double>(y);
    zOffset += static_cast<double>(z);
}

void Tessellator::addVertexWithUV(double x, double y, double z, double u, double v) {
    setTextureUV(u, v);
    addVertex(x, y, z);
}

void Tessellator::addVertex(double x, double y, double z) {
    if (!isDrawing) {
        throw std::runtime_error("Not tesselating!");
    }

    vertices.push_back(makeCurrentVertex(x, y, z));
}
