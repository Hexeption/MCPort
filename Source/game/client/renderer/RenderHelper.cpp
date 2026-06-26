//
// Created by Keir Davis on 26/06/2026.
//

#include "RenderHelper.h"

#include <cmath>

#include <glad/glad.h>

void RenderHelper::disableStandardItemLighting() {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_COLOR_MATERIAL);
}

void RenderHelper::enableStandardItemLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    constexpr float ambient = 0.4f;
    constexpr float diffuse = 0.6f;
    constexpr float specular = 0.0f;

    float x = 0.2f;
    float y = 1.0f;
    float z = -0.7f;
    float length = std::sqrt(x * x + y * y + z * z);
    glLightfv(GL_LIGHT0, GL_POSITION, setColorBuffer(x / length, y / length, z / length, 0.0f));
    glLightfv(GL_LIGHT0, GL_DIFFUSE, setColorBuffer(diffuse, diffuse, diffuse, 1.0f));
    glLightfv(GL_LIGHT0, GL_AMBIENT, setColorBuffer(0.0f, 0.0f, 0.0f, 1.0f));
    glLightfv(GL_LIGHT0, GL_SPECULAR, setColorBuffer(specular, specular, specular, 1.0f));

    x = -0.2f;
    y = 1.0f;
    z = 0.7f;
    length = std::sqrt(x * x + y * y + z * z);
    glLightfv(GL_LIGHT1, GL_POSITION, setColorBuffer(x / length, y / length, z / length, 0.0f));
    glLightfv(GL_LIGHT1, GL_DIFFUSE, setColorBuffer(diffuse, diffuse, diffuse, 1.0f));
    glLightfv(GL_LIGHT1, GL_AMBIENT, setColorBuffer(0.0f, 0.0f, 0.0f, 1.0f));
    glLightfv(GL_LIGHT1, GL_SPECULAR, setColorBuffer(specular, specular, specular, 1.0f));

    glShadeModel(GL_FLAT);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, setColorBuffer(ambient, ambient, ambient, 1.0f));
}

const float *RenderHelper::setColorBuffer(const float red, const float green, const float blue, const float alpha) {
    static float colorBuffer[4];
    colorBuffer[0] = red;
    colorBuffer[1] = green;
    colorBuffer[2] = blue;
    colorBuffer[3] = alpha;
    return colorBuffer;
}
