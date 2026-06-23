//
// Created by Keir Davis on 23/06/2026.
//

#include "ParticleEngine.h"

#include <cmath>

#include <glad/glad.h>

#include "client/Player.h"
#include "client/Textures.h"
#include "client/level/Tesselator.h"
#include "java/Math.h"

ParticleEngine::ParticleEngine(Level &level)
    : level(&level) {
}

ParticleEngine::~ParticleEngine() {
    for (Particle *particle: this->particles) {
        delete particle;
    }
}

void ParticleEngine::add(Particle *p) {
    this->particles.push_back(p);
}

void ParticleEngine::tick() {
    for (int i = 0; i < static_cast<int>(this->particles.size()); ++i) {
        Particle *p = this->particles[i];
        p->tick();
        if (p->removed) {
            delete p;
            this->particles.erase(this->particles.begin() + i--);
        }
    }
}

void ParticleEngine::render(Player &player, float a, int layer) {
    glEnable(GL_TEXTURE_2D);
    int id = Textures::loadTexture("/terrain.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, id);
    float xa = -static_cast<float>(std::cos(
        static_cast<double>(player.yRot) * static_cast<double>(Math::PI) / static_cast<double>(180.0F)));
    float za = -static_cast<float>(std::sin(
        static_cast<double>(player.yRot) * static_cast<double>(Math::PI) / static_cast<double>(180.0F)));
    float xa2 = -za * static_cast<float>(std::sin(
                    static_cast<double>(player.xRot) * static_cast<double>(Math::PI) / static_cast<double>(180.0F)));
    float za2 = xa * static_cast<float>(std::sin(
                    static_cast<double>(player.xRot) * static_cast<double>(Math::PI) / static_cast<double>(180.0F)));
    float ya = static_cast<float>(std::cos(
        static_cast<double>(player.xRot) * static_cast<double>(Math::PI) / static_cast<double>(180.0F)));
    Tesselator &t = Tesselator::instance;
    glColor4f(0.8F, 0.8F, 0.8F, 1.0F);
    t.init();

    for (int i = 0; i < static_cast<int>(this->particles.size()); ++i) {
        Particle *p = this->particles[i];
        if (p->isLit() ^ (layer == 1)) {
            p->render(t, a, xa, ya, za, xa2, za2);
        }
    }

    t.flush();
    glDisable(GL_TEXTURE_2D);
}
