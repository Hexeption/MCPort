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
#include "level/tile/Tile.h"
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
    this->particleEngine = std::make_unique<ParticleEngine>(*this->level);
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

            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_1) {
                this->paintTexture = 1;
            }

            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_2) {
                this->paintTexture = 3;
            }

            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_3) {
                this->paintTexture = 4;
            }

            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_4) {
                this->paintTexture = 5;
            }

            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_6) {
                this->paintTexture = 6;
            }
        }
    }

    this->level->tick();
    this->particleEngine->tick();
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

void RubyDung::setupPickCamera(float a, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    this->viewportBuffer.clear();
    glGetIntegerv(GL_VIEWPORT, this->viewportBuffer.data());
    this->viewportBuffer.flip();
    this->viewportBuffer.limit(16);
    GLU::gluPickMatrix(static_cast<float>(x), static_cast<float>(y), 5.0F, 5.0F, this->viewportBuffer.data());
    GLU::gluPerspective(
        70.0F,
        static_cast<float>(this->width) / static_cast<float>(this->height),
        0.05F,
        1000.0F
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->moveCameraToPlayer(a);
}

void RubyDung::pick(float a) {
    this->selectBuffer.clear();
    glSelectBuffer(static_cast<GLsizei>(this->selectBuffer.capacity()),
                   reinterpret_cast<GLuint *>(this->selectBuffer.data()));
    glRenderMode(GL_SELECT);
    this->setupPickCamera(a, this->width / 2, this->height / 2);
    this->levelRenderer->pick(*this->player, Frustum::getFrustum());
    int_t hits = glRenderMode(GL_RENDER);
    this->selectBuffer.flip();
    this->selectBuffer.limit(this->selectBuffer.capacity());
    long_t closest = 0L;
    int_t names[10] = {};
    int_t hitNameCount = 0;

    for (int_t i = 0; i < hits; ++i) {
        int_t nameCount = this->selectBuffer.get();
        long_t minZ = this->selectBuffer.get();
        this->selectBuffer.get();
        if (minZ >= closest && i != 0) {
            for (int_t j = 0; j < nameCount; ++j) {
                this->selectBuffer.get();
            }
        } else {
            closest = minZ;
            hitNameCount = nameCount;

            for (int_t j = 0; j < nameCount; ++j) {
                names[j] = this->selectBuffer.get();
            }
        }
    }

    if (hitNameCount > 0) {
        this->hitResult = HitResult(names[0], names[1], names[2], names[3], names[4]);
    } else {
        this->hitResult.reset();
    }
}

void RubyDung::render(float a) {
    float xo = static_cast<float>(lwjgl::Mouse::getDX());
    float yo = static_cast<float>(lwjgl::Mouse::getDY());
    this->player->turn(xo, yo);
    this->pick(a);

    while (lwjgl::Mouse::next()) {
        if (lwjgl::Mouse::getEventButton() == 1 && lwjgl::Mouse::getEventButtonState() && this->hitResult.has_value()) {
            Tile *oldTile = Tile::tiles[this->level->getTile(this->hitResult->x, this->hitResult->y, this->hitResult->z)];
            bool changed = this->level->setTile(this->hitResult->x, this->hitResult->y, this->hitResult->z, 0);
            if (oldTile != nullptr && changed) {
                oldTile->destroy(*this->level, this->hitResult->x, this->hitResult->y, this->hitResult->z,
                                 *this->particleEngine);
            }
        }

        if (lwjgl::Mouse::getEventButton() == 0 && lwjgl::Mouse::getEventButtonState() && this->hitResult.has_value()) {
            int_t x = this->hitResult->x;
            int_t y = this->hitResult->y;
            int_t z = this->hitResult->z;
            if (this->hitResult->f == 0) {
                --y;
            }

            if (this->hitResult->f == 1) {
                ++y;
            }

            if (this->hitResult->f == 2) {
                --z;
            }

            if (this->hitResult->f == 3) {
                ++z;
            }

            if (this->hitResult->f == 4) {
                --x;
            }

            if (this->hitResult->f == 5) {
                ++x;
            }

            this->level->setTile(x, y, z, this->paintTexture);
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->setupCamera(a);

    glEnable(GL_CULL_FACE);

    this->levelRenderer->updateDirtyChunks(*this->player);
    this->levelRenderer->render(*this->player, 0);
    this->particleEngine->render(*this->player, a, 0);
    this->levelRenderer->render(*this->player, 1);
    this->particleEngine->render(*this->player, a, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    if (this->hitResult.has_value()) {
        glDisable(GL_ALPHA_TEST);
        this->levelRenderer->renderHit(*this->hitResult);
        glEnable(GL_ALPHA_TEST);
    }

    lwjgl::Display::update();
}

void RubyDung::destory() {
    if (this->level != nullptr) {
        this->level->save();
    }
    lwjgl::Display::Destory();
}
