//
// Created by Keir Davis on 23/06/2026.
//

#include "Tesselator.h"

#include <glad/glad.h>

Tesselator Tesselator::instance = Tesselator();

Tesselator::Tesselator() {
}

void Tesselator::clear() {
    this->vertices = 0;
    this->p = 0;
}

void Tesselator::flush() {
    this->buffer.put(this->array.data(), this->p);
    this->buffer.flip();
    if (this->hasTexture && this->hasColor) {
        glInterleavedArrays(GL_T2F_C3F_V3F, 0, this->buffer.data());
    } else if (this->hasTexture) {
        glInterleavedArrays(GL_T2F_V3F, 0, this->buffer.data());
    } else if (this->hasColor) {
        glInterleavedArrays(GL_C3F_V3F, 0, this->buffer.data());
    } else {
        glInterleavedArrays(GL_V3F, 0, this->buffer.data());
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (this->hasColor) {
        glEnableClientState(GL_COLOR_ARRAY);
    }

    glDrawArrays(GL_QUADS, 0, this->vertices);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (this->hasColor) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    this->clear();
}

void Tesselator::init() {
    this->clear();
    this->hasColor = false;
    this->hasTexture = false;
    this->len = 3;
}

void Tesselator::tex(float u, float v) {
    if (!this->hasTexture) {
        this->len += 2;
    }

    this->hasTexture = true;
    this->u = u;
    this->v = v;
}

void Tesselator::color(float r, float g, float b) {
    if (!this->hasColor) {
        this->len += 3;
    }

    this->hasColor = true;
    this->r = r;
    this->g = g;
    this->b = b;
}

void Tesselator::vertexUV(float x, float y, float z, float u, float v) {
    this->tex(u, v);
    this->vertex(x, y, z);
}

void Tesselator::vertex(float x, float y, float z) {
    if (this->hasTexture) {
        this->array[this->p++] = this->u;
        this->array[this->p++] = this->v;
    }

    if (this->hasColor) {
        this->array[this->p++] = this->r;
        this->array[this->p++] = this->g;
        this->array[this->p++] = this->b;
    }

    this->array[this->p++] = x;
    this->array[this->p++] = y;
    this->array[this->p++] = z;
    ++this->vertices;

    if (this->vertices % 4 == 0 && this->p >= MAX_FLOATS - this->len * 4) {
        this->flush();
    }
}
