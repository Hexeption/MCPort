//
// Created by Keir Davis on 22/06/2026.
//

#include "RubyDung.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "java/String.h"
#include "java/System.h"
#include "lwjgl/Display.h"
#include "lwjgl/GLContext.h"
#include "lwjgl/Keyboard.h"
#include "Player.h"
#include "level/Frustum.h"
#include "lwjgl/Mouse.h"
#include "utils/GLU.h"

RubyDung::RubyDung(int_t width, int_t height)
    : width(width),
      height(height) {
}

void RubyDung::init() {
    int col0 = 16710650;
    int col1 = 920330;
    float fr = 0.5F;
    float fg = 0.8F;
    float fb = 1.0F;

    this->fogColor0.put({
        (float) (col0 >> 16 & 255) / 255.0F,
        (float) (col0 >> 8 & 255) / 255.0F,
        (float) (col0 & 255) / 255.0F,
        1.0F
    });
    this->fogColor0.flip();
    this->fogColor1.put({
        (float) (col1 >> 16 & 255) / 255.0F,
        (float) (col1 >> 8 & 255) / 255.0F,
        (float) (col1 & 255) / 255.0F,
        1.0F
    });
    this->fogColor1.flip();

    lwjgl::Display::setDisplayMode(lwjgl::DisplayMode(width, height));
    lwjgl::Display::setTitle(u"RubyDung");
    lwjgl::Display::create();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(fr, fg, fb, 0.0F);
    glClearDepth(1.0F);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1F);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    this->level = std::make_unique<Level>(256, 256, 64);
    this->player = std::make_unique<Player>(*this->level);
    this->player->setSpawnPosition(
        static_cast<float>(this->level->width) * 0.5F,
        static_cast<float>(this->level->depth + 10),
        static_cast<float>(this->level->height) * 0.5F
    );
    this->levelRenderer = std::make_unique<LevelRenderer>(*this->level);
    lwjgl::Mouse::setGrabbed(true);
}

void RubyDung::run() {
    try {
        init();

        long_t lastTime = System::currentTimeMillis();
        int_t frames = 0;
        while (!lwjgl::Keyboard::isKeyDown(1) && !lwjgl::Display::isCloseRequested()) {
            this->timer.advanceTime();

            for (int i = 0; i < this->timer.ticks; ++i) {
                this->tick();
            }

            this->render(this->timer.a);
            frames++;

            while (System::currentTimeMillis() >= lastTime + 1000L) {
                lwjgl::Display::setTitle(
                    u"RubyDung - FPS: " + String::toString(frames) + u", Updates: " + String::toString(Chunk::updates));
                Chunk::updates = 0;
                lastTime += 1000L;
                frames = 0;
            }
        }
    } catch (std::exception &e) {
        printf("Exception caught: %s\n", e.what());
    }

    this->destory();
}

void RubyDung::tick() {
    while (lwjgl::Keyboard::next()) {
        if (lwjgl::Keyboard::getEventKeyState()) {
            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_RETURN) {
                if (this->level != nullptr) {
                    this->level->save();
                }
            }
        }
        const std::string keyName = String::toUtf8(
            lwjgl::Keyboard::getKeyName(lwjgl::Keyboard::getEventKey())
        );
        printf("Key pressed: %s\n", keyName.c_str());
    }

    this->level->tick();
    this->player->tick();
}

void RubyDung::moveCameraToPlayer(float a) {
    glTranslatef(0.0F, 0.0F, -0.3F);
    glRotatef(this->player->xRot, 1.0F, 0.0F, 0.0F);
    glRotatef(this->player->yRot, 0.0F, 1.0F, 0.0F);
    float x = this->player->xo + (this->player->x - this->player->xo) * a;
    float y = this->player->yo + (this->player->y - this->player->yo) * a;
    float z = this->player->zo + (this->player->z - this->player->zo) * a;
    glTranslatef(-x, -y, -z);
}

void RubyDung::setupCamera(float a) {
    glViewport(0, 0, lwjgl::Display::getWidth(), lwjgl::Display::getHeight());
    glMatrixMode(5889);
    glLoadIdentity();
    GLU::gluPerspective(
        70.0F,
        (float) lwjgl::Display::getWidth() / (float) lwjgl::Display::getHeight(),
        0.05F,
        1000.0F
    );
    glMatrixMode(5888);
    glLoadIdentity();
    this->moveCameraToPlayer(a);
}

void RubyDung::render(float a) {
    float xo = static_cast<float>(lwjgl::Mouse::getDX());
    float yo = static_cast<float>(lwjgl::Mouse::getDY());
    this->player->turn(xo, yo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->setupCamera(a);

    glEnable(GL_CULL_FACE);

    this->levelRenderer->updateDirtyChunks(*this->player);
    this->levelRenderer->render(*this->player, 0);
    this->levelRenderer->render(*this->player, 1);

    lwjgl::Display::update();
}

void RubyDung::destory() {
    if (this->level != nullptr) {
        this->level->save();
    }
    lwjgl::Display::Destory();
}
