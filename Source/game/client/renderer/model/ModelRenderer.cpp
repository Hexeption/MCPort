//
// Created by Keir Davis on 26/06/2026.
//

#include "ModelRenderer.h"

#include <glad/glad.h>

static constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979f;

ModelRenderer::ModelRenderer(const int texOffsetX, const int texOffsetY)
    : textureOffsetX(texOffsetX), textureOffsetY(texOffsetY) {
}

ModelRenderer::~ModelRenderer() {
    if (compiled && displayList != 0) {
        glDeleteLists(static_cast<GLuint>(displayList), 1);
    }
}

void ModelRenderer::addBox(float x, float y, float z, int w, int h, int d, float extra) {
    corners.clear();
    faces.clear();
    compiled = false;
    if (displayList != 0) {
        glDeleteLists(static_cast<GLuint>(displayList), 1);
        displayList = 0;
    }

    float x1 = x + static_cast<float>(w);
    float y1 = y + static_cast<float>(h);
    float z1 = z + static_cast<float>(d);
    x -= extra; y -= extra; z -= extra;
    x1 += extra; y1 += extra; z1 += extra;

    if (mirror) { std::swap(x, x1); }

    PositionTexureVertex v0(x,  y,  z,  0, 0);
    PositionTexureVertex v1(x1, y,  z,  0, 8);
    PositionTexureVertex v2(x1, y1, z,  8, 8);
    PositionTexureVertex v3(x,  y1, z,  8, 0);
    PositionTexureVertex v4(x,  y,  z1, 0, 0);
    PositionTexureVertex v5(x1, y,  z1, 0, 8);
    PositionTexureVertex v6(x1, y1, z1, 8, 8);
    PositionTexureVertex v7(x,  y1, z1, 8, 0);

    const int tx = textureOffsetX;
    const int ty = textureOffsetY;

    faces.emplace_back(v5, v1, v2, v6, tx + d + w, ty + d, tx + d + w + d, ty + d + h);
    faces.emplace_back(v0, v4, v7, v3, tx + 0,     ty + d, tx + d,         ty + d + h);
    faces.emplace_back(v5, v4, v0, v1, tx + d,     ty + 0, tx + d + w,     ty + d    );
    faces.emplace_back(v2, v3, v7, v6, tx + d + w, ty + 0, tx + d + w + w, ty + d    );
    faces.emplace_back(v1, v0, v3, v2, tx + d,     ty + d, tx + d + w,     ty + d + h);
    faces.emplace_back(v4, v5, v6, v7, tx + d+w+d, ty + d, tx + d+w+d+w,   ty + d + h);

    if (mirror) {
        for (auto &face : faces) {
            face.flipFace();
        }
    }
}

void ModelRenderer::setRotationPoint(float px, float py, float pz) {
    rotationPointX = px;
    rotationPointY = py;
    rotationPointZ = pz;
}

void ModelRenderer::render(const float scale) {
    if (isHidden || !showModel) return;
    if (!compiled) compileDisplayList(scale);

    if (rotateAngleX == 0.0f && rotateAngleY == 0.0f && rotateAngleZ == 0.0f) {
        if (rotationPointX == 0.0f && rotationPointY == 0.0f && rotationPointZ == 0.0f) {
            glCallList(static_cast<GLuint>(displayList));
        } else {
            glTranslatef(rotationPointX * scale, rotationPointY * scale, rotationPointZ * scale);
            glCallList(static_cast<GLuint>(displayList));
            glTranslatef(-rotationPointX * scale, -rotationPointY * scale, -rotationPointZ * scale);
        }
    } else {
        glPushMatrix();
        glTranslatef(rotationPointX * scale, rotationPointY * scale, rotationPointZ * scale);
        if (rotateAngleZ != 0.0f) glRotatef(rotateAngleZ * RAD_TO_DEG, 0.0f, 0.0f, 1.0f);
        if (rotateAngleY != 0.0f) glRotatef(rotateAngleY * RAD_TO_DEG, 0.0f, 1.0f, 0.0f);
        if (rotateAngleX != 0.0f) glRotatef(rotateAngleX * RAD_TO_DEG, 1.0f, 0.0f, 0.0f);
        glCallList(static_cast<GLuint>(displayList));
        glPopMatrix();
    }
}

void ModelRenderer::renderWithRotation(const float scale) {
    if (isHidden || !showModel) return;
    if (!compiled) compileDisplayList(scale);

    if (rotateAngleX == 0.0f && rotateAngleY == 0.0f && rotateAngleZ == 0.0f) {
        if (rotationPointX != 0.0f || rotationPointY != 0.0f || rotationPointZ != 0.0f) {
            glTranslatef(rotationPointX * scale, rotationPointY * scale, rotationPointZ * scale);
        }
    } else {
        glTranslatef(rotationPointX * scale, rotationPointY * scale, rotationPointZ * scale);
        if (rotateAngleZ != 0.0f) glRotatef(rotateAngleZ * RAD_TO_DEG, 0.0f, 0.0f, 1.0f);
        if (rotateAngleY != 0.0f) glRotatef(rotateAngleY * RAD_TO_DEG, 0.0f, 1.0f, 0.0f);
        if (rotateAngleX != 0.0f) glRotatef(rotateAngleX * RAD_TO_DEG, 1.0f, 0.0f, 0.0f);
    }
}

void ModelRenderer::compileDisplayList(const float scale) {
    displayList = glGenLists(1);
    glNewList(static_cast<GLuint>(displayList), GL_COMPILE);
    for (const auto &face : faces) {
        face.draw(scale);
    }
    glEndList();
    compiled = true;
}
