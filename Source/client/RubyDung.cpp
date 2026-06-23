//
// Created by Keir Davis on 22/06/2026.
//

#include "RubyDung.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <utility>

#include "java/String.h"
#include "java/System.h"
#include "level/Tesselator.h"
#include "lwjgl/Display.h"
#include "lwjgl/GLContext.h"
#include "lwjgl/Keyboard.h"
#include "Player.h"
#include "level/Frustum.h"
#include "level/tile/Tile.h"
#include "lwjgl/Mouse.h"
#include "Textures.h"
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

    for (int_t i = 0; i < 10; ++i) {
        std::unique_ptr<Zombie> zombie = std::make_unique<Zombie>(*this->level, 128.0F, 0.0F, 128.0F);
        zombie->resetPos();
        this->zombies.push_back(std::move(zombie));
    }
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

            if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_G) {
                this->zombies.push_back(
                    std::make_unique<Zombie>(*this->level, this->player->x, this->player->y, this->player->z)
                );
            }
        }
    }

    this->level->tick();
    this->particleEngine->tick();

    for (int_t i = 0; i < static_cast<int_t>(this->zombies.size()); ++i) {
        this->zombies[i]->tick();
        if (this->zombies[i]->removed) {
            this->zombies.erase(this->zombies.begin() + i--);
        }
    }

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
    this->setupFog(0);
    glEnable(GL_FOG);
    this->levelRenderer->render(*this->player, 0);

    Frustum &frustum = Frustum::getFrustum();
    for (const auto &zombie : this->zombies) {
        if (zombie->isLit() && frustum.isVisible(zombie->bb)) {
            zombie->render(a);
        }
    }

    this->particleEngine->render(*this->player, a, 0);
    this->setupFog(1);
    this->levelRenderer->render(*this->player, 1);

    for (const auto &zombie : this->zombies) {
        if (!zombie->isLit() && frustum.isVisible(zombie->bb)) {
            zombie->render(a);
        }
    }

    this->particleEngine->render(*this->player, a, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    if (this->hitResult.has_value()) {
        glDisable(GL_ALPHA_TEST);
        this->levelRenderer->renderHit(*this->hitResult);
        glEnable(GL_ALPHA_TEST);
    }

    this->drawGui(a);
    lwjgl::Display::update();
}

void RubyDung::drawGui(float a) {
    int_t screenWidth = this->width * 240 / this->height;
    int_t screenHeight = this->height * 240 / this->height;

    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0F, screenWidth, screenHeight, 0.0F, 100.0F, 300.0F);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -200.0F);

    glPushMatrix();
    glTranslatef(static_cast<float>(screenWidth - 16), 16.0F, 0.0F);
    Tesselator &t = Tesselator::instance;
    glScalef(16.0F, 16.0F, 16.0F);
    glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
    glTranslatef(-1.5F, 0.5F, -0.5F);
    glScalef(-1.0F, -1.0F, 1.0F);

    int_t id = Textures::loadTexture("/terrain.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(id));
    glEnable(GL_TEXTURE_2D);
    t.init();
    Tile::tiles[this->paintTexture]->render(t, *this->level, 0, -2, 0, 0);
    t.flush();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    int_t wc = screenWidth / 2;
    int_t hc = screenHeight / 2;
    glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
    t.init();
    t.vertex(static_cast<float>(wc + 1), static_cast<float>(hc - 4), 0.0F);
    t.vertex(static_cast<float>(wc), static_cast<float>(hc - 4), 0.0F);
    t.vertex(static_cast<float>(wc), static_cast<float>(hc + 5), 0.0F);
    t.vertex(static_cast<float>(wc + 1), static_cast<float>(hc + 5), 0.0F);
    t.vertex(static_cast<float>(wc + 5), static_cast<float>(hc), 0.0F);
    t.vertex(static_cast<float>(wc - 4), static_cast<float>(hc), 0.0F);
    t.vertex(static_cast<float>(wc - 4), static_cast<float>(hc + 1), 0.0F);
    t.vertex(static_cast<float>(wc + 5), static_cast<float>(hc + 1), 0.0F);
    t.flush();
}

void RubyDung::setupFog(int_t i) {
    if (i == 0) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.001F);
        glFogfv(GL_FOG_COLOR, this->fogColor0.data());
        glDisable(GL_LIGHTING);
    } else if (i == 1) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.06F);
        glFogfv(GL_FOG_COLOR, this->fogColor1.data());
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        float br = 0.6F;
        this->lightBrightnessBuffer.clear();
        this->lightBrightnessBuffer.put({br, br, br, 1.0F});
        this->lightBrightnessBuffer.flip();
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->lightBrightnessBuffer.data());
    }
}

void RubyDung::destory() {
    if (this->level != nullptr) {
        this->level->save();
    }
    lwjgl::Display::Destory();
}
