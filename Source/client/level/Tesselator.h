//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_TESSELATOR_H
#define MCPORT_TESSELATOR_H

#include <vector>

#include "java/Type.h"
#include "lwjgl/BufferUtils.h"

class Tesselator {
private:
    static constexpr int_t MAX_FLOATS = 524288;

    lwjgl::FloatBuffer buffer = lwjgl::BufferUtils::createFloatBuffer(MAX_FLOATS);
    std::vector<float> array = std::vector<float>(MAX_FLOATS);
    int_t vertices = 0;
    float u = 0.0F;
    float v = 0.0F;
    float r = 0.0F;
    float g = 0.0F;
    float b = 0.0F;
    bool hasColor = false;
    bool hasTexture = false;
    int_t len = 3;
    int_t p = 0;

    Tesselator();

    void clear();

public:
    static Tesselator instance;

    void flush();

    void init();

    void tex(float u, float v);

    void color(float r, float g, float b);

    void vertexUV(float x, float y, float z, float u, float v);

    void vertex(float x, float y, float z);
};


#endif //MCPORT_TESSELATOR_H
