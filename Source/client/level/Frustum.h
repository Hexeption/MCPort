// 
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_FRUSTUM_H
#define MCPORT_FRUSTUM_H

#include <cstddef>

#include "client/phys/AABB.h"
#include "port/lwjgl/BufferUtils.h"

class Frustum {
public:
    float m_Frustum[6][4];

    static constexpr int RIGHT = 0;
    static constexpr int LEFT = 1;
    static constexpr int BOTTOM = 2;
    static constexpr int TOP = 3;
    static constexpr int BACK = 4;
    static constexpr int FRONT = 5;
    static constexpr int A = 0;
    static constexpr int B = 1;
    static constexpr int C = 2;
    static constexpr int D = 3;

private:
    static Frustum frustum;
    lwjgl::FloatBuffer _proj;
    lwjgl::FloatBuffer _modl;
    lwjgl::FloatBuffer _clip;
    float proj[16];
    float modl[16];
    float clip[16];

    Frustum();

    void normalizePlane(float frustum[6][4], int side);

    void calculateFrustum();

public:
    static Frustum &getFrustum();

    bool pointInFrustum(float x, float y, float z);

    bool sphereInFrustum(float x, float y, float z, float radius);

    bool cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);

    bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);

    bool isVisible(const AABB &aabb);
};

#endif // MCPORT_FRUSTUM_H
