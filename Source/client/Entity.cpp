//
// Created by Keir Davis on 23/06/2026.
//

#include "Entity.h"

#include <cmath>
#include <cstdlib>

namespace {
    constexpr float PI = 3.14159265358979323846F;
}

Entity::Entity(Level &level)
    : level(&level) {
    this->resetPos();
}

void Entity::resetPos() {
    float x = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * static_cast<float>(this->level->width);
    float y = static_cast<float>(this->level->depth + 10);
    float z = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * static_cast<float>(this->level->height);
    this->setPos(x, y, z);
}

void Entity::remove() {
    this->removed = true;
}

void Entity::setSize(float w, float h) {
    this->bbWidth = w;
    this->bbHeight = h;
}

void Entity::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    float w = this->bbWidth / 2.0F;
    float h = this->bbHeight / 2.0F;
    this->bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void Entity::turn(float xo, float yo) {
    this->yRot = static_cast<float>(static_cast<double>(this->yRot) + static_cast<double>(xo) * 0.15);
    this->xRot = static_cast<float>(static_cast<double>(this->xRot) - static_cast<double>(yo) * 0.15);
    if (this->xRot < -90.0F) {
        this->xRot = -90.0F;
    }

    if (this->xRot > 90.0F) {
        this->xRot = 90.0F;
    }
}

void Entity::tick() {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
}

void Entity::move(float xa, float ya, float za) {
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;
    std::vector<AABB *> &aABBs = this->level->getCubes(this->bb.expand(xa, ya, za));

    for (AABB *aabb : aABBs) {
        ya = aabb->clipYCollide(this->bb, ya);
    }

    this->bb.move(0.0F, ya, 0.0F);

    for (AABB *aabb : aABBs) {
        xa = aabb->clipXCollide(this->bb, xa);
    }

    this->bb.move(xa, 0.0F, 0.0F);

    for (AABB *aabb : aABBs) {
        za = aabb->clipZCollide(this->bb, za);
    }

    this->bb.move(0.0F, 0.0F, za);
    this->onGround = yaOrg != ya && yaOrg < 0.0F;
    if (xaOrg != xa) {
        this->xd = 0.0F;
    }

    if (yaOrg != ya) {
        this->yd = 0.0F;
    }

    if (zaOrg != za) {
        this->zd = 0.0F;
    }

    this->x = (this->bb.x0 + this->bb.x1) / 2.0F;
    this->y = this->bb.y0 + this->heightOffset;
    this->z = (this->bb.z0 + this->bb.z1) / 2.0F;

    for (AABB *aabb : aABBs) {
        delete aabb;
    }
}

void Entity::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (!(dist < 0.01F)) {
        dist = speed / static_cast<float>(std::sqrt(static_cast<double>(dist)));
        xa *= dist;
        za *= dist;
        float sin = static_cast<float>(std::sin(static_cast<double>(this->yRot) * static_cast<double>(PI) / 180.0));
        float cos = static_cast<float>(std::cos(static_cast<double>(this->yRot) * static_cast<double>(PI) / 180.0));
        this->xd += xa * cos - za * sin;
        this->zd += za * cos + xa * sin;
    }
}

bool Entity::isLit() {
    int xTile = static_cast<int>(this->x);
    int yTile = static_cast<int>(this->y);
    int zTile = static_cast<int>(this->z);
    return this->level->isLit(xTile, yTile, zTile);
}
