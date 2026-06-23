//
// Created by Keir Davis on 23/06/2026.
//

#include "Particle.h"

#include <cmath>
#include <cstdlib>

#include "client/level/Tesselator.h"

namespace {
    double randomDouble() {
        return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    }
}

Particle::Particle(Level &level, float x, float y, float z, float xa, float ya, float za, int tex)
    : Entity(level), tex(tex) {
    this->setSize(0.2F, 0.2F);
    this->heightOffset = this->bbHeight / 2.0F;
    this->setPos(x, y, z);
    this->xd = xa + static_cast<float>(randomDouble() * static_cast<double>(2.0F) - static_cast<double>(1.0F)) * 0.4F;
    this->yd = ya + static_cast<float>(randomDouble() * static_cast<double>(2.0F) - static_cast<double>(1.0F)) * 0.4F;
    this->zd = za + static_cast<float>(randomDouble() * static_cast<double>(2.0F) - static_cast<double>(1.0F)) * 0.4F;
    float speed = static_cast<float>(randomDouble() + randomDouble() + static_cast<double>(1.0F)) * 0.15F;
    float dd = static_cast<float>(std::sqrt(static_cast<double>(this->xd * this->xd + this->yd * this->yd + this->zd * this->zd)));
    this->xd = this->xd / dd * speed * 0.4F;
    this->yd = this->yd / dd * speed * 0.4F + 0.1F;
    this->zd = this->zd / dd * speed * 0.4F;
    this->uo = static_cast<float>(randomDouble()) * 3.0F;
    this->vo = static_cast<float>(randomDouble()) * 3.0F;
    this->size = static_cast<float>(randomDouble() * static_cast<double>(0.5F) + static_cast<double>(0.5F));
    this->lifetime = static_cast<int>(static_cast<double>(4.0F) / (randomDouble() * 0.9 + 0.1));
    this->age = 0;
}

void Particle::tick() {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    if (this->age++ >= this->lifetime) {
        this->remove();
    }

    this->yd = static_cast<float>(static_cast<double>(this->yd) - 0.04);
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.98F;
    this->yd *= 0.98F;
    this->zd *= 0.98F;
    if (this->onGround) {
        this->xd *= 0.7F;
        this->zd *= 0.7F;
    }
}

void Particle::render(Tesselator &t, float a, float xa, float ya, float za, float xa2, float za2) {
    float u0 = (static_cast<float>(this->tex % 16) + this->uo / 4.0F) / 16.0F;
    float u1 = u0 + 0.015609375F;
    float v0 = (static_cast<float>(this->tex / 16) + this->vo / 4.0F) / 16.0F;
    float v1 = v0 + 0.015609375F;
    float r = 0.1F * this->size;
    float x = this->xo + (this->x - this->xo) * a;
    float y = this->yo + (this->y - this->yo) * a;
    float z = this->zo + (this->z - this->zo) * a;
    t.vertexUV(x - xa * r - xa2 * r, y - ya * r, z - za * r - za2 * r, u0, v1);
    t.vertexUV(x - xa * r + xa2 * r, y + ya * r, z - za * r + za2 * r, u0, v0);
    t.vertexUV(x + xa * r + xa2 * r, y + ya * r, z + za * r + za2 * r, u1, v0);
    t.vertexUV(x + xa * r - xa2 * r, y - ya * r, z + za * r - za2 * r, u1, v1);
}
