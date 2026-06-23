//
// Created by Keir Davis on 23/06/2026.
//

#include "GLU.h"

#include <cmath>

#include "glad/glad.h"
#include "java/Math.h"

void GLU::gluPerspective(float fovy, float aspect, float zNear, float zFar) {
    double const height = zNear * std::tan(static_cast<double>(fovy) * Math::PI / 360.0);
    double const width = height * aspect;
    glFrustum(-width, width, -height, height, zNear, zFar);
}

void GLU::gluPickMatrix(float x, float y, float width, float height, const int_t *viewport) {
    if (width <= 0.0F || height <= 0.0F) {
        return;
    }

    glTranslatef(
        (static_cast<float>(viewport[2]) - 2.0F * (x - static_cast<float>(viewport[0]))) / width,
        (static_cast<float>(viewport[3]) - 2.0F * (y - static_cast<float>(viewport[1]))) / height,
        0.0F
    );
    glScalef(static_cast<float>(viewport[2]) / width, static_cast<float>(viewport[3]) / height, 1.0F);
}
