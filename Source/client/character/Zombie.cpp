//
// Created by Keir Davis on 23/06/2026.
//

#include "Zombie.h"

#include <cmath>
#include <cstdlib>

#include <glad/glad.h>

#include "client/Textures.h"
#include "java/Math.h"
#include "java/System.h"

ZombieModel Zombie::zombieModel = ZombieModel();

Zombie::Zombie(Level &level, float x, float y, float z)
    : Entity(level),
      rotA((Math::random() + 1.0F) * 0.01F) {
    this->setPos(x, y, z);
    this->timeOffs = Math::random() * 1239813.0F;
    this->rot = static_cast<float>(Math::random() * Math::PI * 2.0);
    this->speed = 1.0F;
}

void Zombie::tick() {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    float xa = 0.0F;
    float ya = 0.0F;
    if (this->y < -100.0F) {
        this->remove();
    }

    this->rot += this->rotA;
    this->rotA = static_cast<float>(static_cast<double>(this->rotA) * 0.99);
    this->rotA = static_cast<float>(
        static_cast<double>(this->rotA) +
        static_cast<double>(Math::random() - Math::random()) *
        static_cast<double>(Math::random()) *
        static_cast<double>(Math::random()) *
        0.08
    );
    xa = static_cast<float>(std::sin(static_cast<double>(this->rot)));
    ya = static_cast<float>(std::cos(static_cast<double>(this->rot)));
    if (this->onGround && Math::random() < 0.08F) {
        this->yd = 0.5F;
    }

    this->moveRelative(xa, ya, this->onGround ? 0.1F : 0.02F);
    this->yd = static_cast<float>(static_cast<double>(this->yd) - 0.08);
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.91F;
    this->yd *= 0.98F;
    this->zd *= 0.91F;
    if (this->onGround) {
        this->xd *= 0.7F;
        this->zd *= 0.7F;
    }
}

void Zombie::render(float a) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(Textures::loadTexture("/char.png", GL_NEAREST)));
    glPushMatrix();
    double time = static_cast<double>(System::nanoTime()) / 1.0E9 * 10.0 * static_cast<double>(this->speed) +
                  static_cast<double>(this->timeOffs);
    float size = 0.058333334F;
    float yy = static_cast<float>(-std::abs(std::sin(time * 0.6662)) * 5.0 - 23.0);
    glTranslatef(
        this->xo + (this->x - this->xo) * a,
        this->yo + (this->y - this->yo) * a,
        this->zo + (this->z - this->zo) * a
    );
    glScalef(1.0F, -1.0F, 1.0F);
    glScalef(size, size, size);
    glTranslatef(0.0F, yy, 0.0F);
    float c = 57.29578F;
    glRotatef(this->rot * c + 180.0F, 0.0F, 1.0F, 0.0F);
    zombieModel.render(static_cast<float>(time));
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
