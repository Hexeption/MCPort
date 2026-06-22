//
// Created by Keir Davis on 22/06/2026.
//

#include "RubyDung.h"

#include "java/System.h"
#include "lwjgl/Display.h"

RubyDung::RubyDung(int_t width, int_t height) {
    this->width = width;
    this->height = height;
}

void RubyDung::init() {
    lwjgl::Display::setDisplayMode(lwjgl::DisplayMode(width, height));
    lwjgl::Display::setTitle(u"RubyDung");
    lwjgl::Display::create();
    printf("RubyDung initialized\n");
}

void RubyDung::run() {
    init();

    long_t lastTime = System::currentTimeMillis();
    int_t frames = 0;
    try {
        while (!lwjgl::Display::isCloseRequested()) {
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
}

void RubyDung::render(float a) {
    lwjgl::Display::update();
}

void RubyDung::destory() {
    lwjgl::Display::Destory();
}
