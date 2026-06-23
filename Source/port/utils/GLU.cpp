//
// Created by Keir Davis on 23/06/2026.
//

#include "GLU.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "glad/glad.h"

void GLU::gluPerspective(float fovy, float aspect, float zNear, float zFar) {
    double const height = zNear * tanf(fovy * M_PI / 360.0);
    double const width = height * aspect;
    glFrustum(-width, width, -height, height, zNear, zFar);
}
