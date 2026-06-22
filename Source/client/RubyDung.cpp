//
// Created by Keir Davis on 22/06/2026.
//

#include "RubyDung.h"

#include "java/String.h"
#include "java/System.h"
#include "lwjgl/Display.h"
#include "lwjgl/Keyboard.h"

RubyDung::RubyDung(int_t width, int_t height) {
    this->width = width;
    this->height = height;
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
}

void RubyDung::run() {
    init();

    long_t lastTime = System::currentTimeMillis();
    int_t frames = 0;
    try {
        while (!lwjgl::Keyboard::isKeyDown(1) && !lwjgl::Display::isCloseRequested()) {
            this->timer.advanceTime();

            for (int i = 0; i < this->timer.ticks; ++i) {
                this->tick();
            }

            this->render(this->timer.a);
            frames++;

            while (System::currentTimeMillis() >= lastTime + 1000L) {
                lwjgl::Display::setTitle(u"RubyDung - FPS: " + String::toString(frames));
                // chunk updates
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
        const std::string keyName = String::toUtf8(
            lwjgl::Keyboard::getKeyName(lwjgl::Keyboard::getEventKey())
        );
        printf("Key pressed: %s\n", keyName.c_str());
    }
}

void RubyDung::render(float a) {
    lwjgl::Display::update();
}

void RubyDung::destory() {
    lwjgl::Display::Destory();
}
