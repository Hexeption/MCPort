//
// Created by Keir Davis on 23/06/2026.
//

#include "Frustum.h"

#include <cmath>

#include <glad/glad.h>

Frustum Frustum::frustum = Frustum();

Frustum::Frustum()
    : _proj(lwjgl::BufferUtils::createFloatBuffer(16)),
      _modl(lwjgl::BufferUtils::createFloatBuffer(16)),
      _clip(lwjgl::BufferUtils::createFloatBuffer(16)) {
}

Frustum &Frustum::getFrustum() {
    frustum.calculateFrustum();
    return frustum;
}

void Frustum::normalizePlane(float frustumPlanes[6][4], int side) {
    float magnitude = std::sqrt(
        frustumPlanes[side][0] * frustumPlanes[side][0] +
        frustumPlanes[side][1] * frustumPlanes[side][1] +
        frustumPlanes[side][2] * frustumPlanes[side][2]
    );
    frustumPlanes[side][0] /= magnitude;
    frustumPlanes[side][1] /= magnitude;
    frustumPlanes[side][2] /= magnitude;
    frustumPlanes[side][3] /= magnitude;
}

void Frustum::calculateFrustum() {
    this->_proj.clear();
    this->_modl.clear();
    this->_clip.clear();
    glGetFloatv(GL_PROJECTION_MATRIX, this->_proj.data());
    glGetFloatv(GL_MODELVIEW_MATRIX, this->_modl.data());
    this->_proj.flip();
    this->_proj.limit(16);
    this->_proj.get(this->proj);
    this->_modl.flip();
    this->_modl.limit(16);
    this->_modl.get(this->modl);

    this->clip[0] = this->modl[0] * this->proj[0] + this->modl[1] * this->proj[4] + this->modl[2] * this->proj[8] + this->modl[3] * this->proj[12];
    this->clip[1] = this->modl[0] * this->proj[1] + this->modl[1] * this->proj[5] + this->modl[2] * this->proj[9] + this->modl[3] * this->proj[13];
    this->clip[2] = this->modl[0] * this->proj[2] + this->modl[1] * this->proj[6] + this->modl[2] * this->proj[10] + this->modl[3] * this->proj[14];
    this->clip[3] = this->modl[0] * this->proj[3] + this->modl[1] * this->proj[7] + this->modl[2] * this->proj[11] + this->modl[3] * this->proj[15];
    this->clip[4] = this->modl[4] * this->proj[0] + this->modl[5] * this->proj[4] + this->modl[6] * this->proj[8] + this->modl[7] * this->proj[12];
    this->clip[5] = this->modl[4] * this->proj[1] + this->modl[5] * this->proj[5] + this->modl[6] * this->proj[9] + this->modl[7] * this->proj[13];
    this->clip[6] = this->modl[4] * this->proj[2] + this->modl[5] * this->proj[6] + this->modl[6] * this->proj[10] + this->modl[7] * this->proj[14];
    this->clip[7] = this->modl[4] * this->proj[3] + this->modl[5] * this->proj[7] + this->modl[6] * this->proj[11] + this->modl[7] * this->proj[15];
    this->clip[8] = this->modl[8] * this->proj[0] + this->modl[9] * this->proj[4] + this->modl[10] * this->proj[8] + this->modl[11] * this->proj[12];
    this->clip[9] = this->modl[8] * this->proj[1] + this->modl[9] * this->proj[5] + this->modl[10] * this->proj[9] + this->modl[11] * this->proj[13];
    this->clip[10] = this->modl[8] * this->proj[2] + this->modl[9] * this->proj[6] + this->modl[10] * this->proj[10] + this->modl[11] * this->proj[14];
    this->clip[11] = this->modl[8] * this->proj[3] + this->modl[9] * this->proj[7] + this->modl[10] * this->proj[11] + this->modl[11] * this->proj[15];
    this->clip[12] = this->modl[12] * this->proj[0] + this->modl[13] * this->proj[4] + this->modl[14] * this->proj[8] + this->modl[15] * this->proj[12];
    this->clip[13] = this->modl[12] * this->proj[1] + this->modl[13] * this->proj[5] + this->modl[14] * this->proj[9] + this->modl[15] * this->proj[13];
    this->clip[14] = this->modl[12] * this->proj[2] + this->modl[13] * this->proj[6] + this->modl[14] * this->proj[10] + this->modl[15] * this->proj[14];
    this->clip[15] = this->modl[12] * this->proj[3] + this->modl[13] * this->proj[7] + this->modl[14] * this->proj[11] + this->modl[15] * this->proj[15];

    this->m_Frustum[0][0] = this->clip[3] - this->clip[0];
    this->m_Frustum[0][1] = this->clip[7] - this->clip[4];
    this->m_Frustum[0][2] = this->clip[11] - this->clip[8];
    this->m_Frustum[0][3] = this->clip[15] - this->clip[12];
    this->normalizePlane(this->m_Frustum, 0);
    this->m_Frustum[1][0] = this->clip[3] + this->clip[0];
    this->m_Frustum[1][1] = this->clip[7] + this->clip[4];
    this->m_Frustum[1][2] = this->clip[11] + this->clip[8];
    this->m_Frustum[1][3] = this->clip[15] + this->clip[12];
    this->normalizePlane(this->m_Frustum, 1);
    this->m_Frustum[2][0] = this->clip[3] + this->clip[1];
    this->m_Frustum[2][1] = this->clip[7] + this->clip[5];
    this->m_Frustum[2][2] = this->clip[11] + this->clip[9];
    this->m_Frustum[2][3] = this->clip[15] + this->clip[13];
    this->normalizePlane(this->m_Frustum, 2);
    this->m_Frustum[3][0] = this->clip[3] - this->clip[1];
    this->m_Frustum[3][1] = this->clip[7] - this->clip[5];
    this->m_Frustum[3][2] = this->clip[11] - this->clip[9];
    this->m_Frustum[3][3] = this->clip[15] - this->clip[13];
    this->normalizePlane(this->m_Frustum, 3);
    this->m_Frustum[4][0] = this->clip[3] - this->clip[2];
    this->m_Frustum[4][1] = this->clip[7] - this->clip[6];
    this->m_Frustum[4][2] = this->clip[11] - this->clip[10];
    this->m_Frustum[4][3] = this->clip[15] - this->clip[14];
    this->normalizePlane(this->m_Frustum, 4);
    this->m_Frustum[5][0] = this->clip[3] + this->clip[2];
    this->m_Frustum[5][1] = this->clip[7] + this->clip[6];
    this->m_Frustum[5][2] = this->clip[11] + this->clip[10];
    this->m_Frustum[5][3] = this->clip[15] + this->clip[14];
    this->normalizePlane(this->m_Frustum, 5);
}

bool Frustum::pointInFrustum(float x, float y, float z) {
    for (int i = 0; i < 6; ++i) {
        if (this->m_Frustum[i][0] * x + this->m_Frustum[i][1] * y + this->m_Frustum[i][2] * z + this->m_Frustum[i][3] <= 0.0F) {
            return false;
        }
    }

    return true;
}

bool Frustum::sphereInFrustum(float x, float y, float z, float radius) {
    for (int i = 0; i < 6; ++i) {
        if (this->m_Frustum[i][0] * x + this->m_Frustum[i][1] * y + this->m_Frustum[i][2] * z + this->m_Frustum[i][3] <= -radius) {
            return false;
        }
    }

    return true;
}

bool Frustum::cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
    for (int i = 0; i < 6; ++i) {
        if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }

        if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }
    }

    return true;
}

bool Frustum::cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
    for (int i = 0; i < 6; ++i) {
        if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F) &&
            !(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0F)) {
            return false;
        }
    }

    return true;
}

bool Frustum::isVisible(const AABB &aabb) {
    return this->cubeInFrustum(aabb.x0, aabb.y0, aabb.z0, aabb.x1, aabb.y1, aabb.z1);
}
