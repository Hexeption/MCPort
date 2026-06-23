//
// Created by Keir Davis on 23/06/2026.
//

#include "Minecraft.h"

#include "java/System.h"
#include "lwjgl/Display.h"

Minecraft::Minecraft(int_t displayWidth, int_t displayHeight, bool fullscreen) : displayWidth(displayWidth),
    displayHeight(displayHeight), fullscreen(fullscreen) {
}

void Minecraft::startGame() {
    if (fullscreen) {
        lwjgl::Display::setFullscreen(true);
        displayWidth = lwjgl::Display::getWidth();
        displayHeight = lwjgl::Display::getHeight();
        if (displayWidth <= 0) {
            displayWidth = 1;
        }

        if (displayHeight <= 0) {
            displayHeight = 1;
        }
    } else {
        lwjgl::Display::setDisplayMode(lwjgl::DisplayMode(displayWidth, displayHeight));
    }

    // in java the title is `Minecraft Minecraft Alpha v1.1.0` ha
    lwjgl::Display::setTitle(u"Minecraft Alpha v1.1.0");

    lwjgl::Display::create();
}

void Minecraft::run() {
    running = true;

    try {
        startGame();
    } catch (std::exception &e) {
        printf("Exception caught in main: %s\n", e.what());
    }

    try {
        long_t currentTime = System::currentTimeMillis();
        int_t fps = 0;

        while (running) {
            if (lwjgl::Display::isCloseRequested()) {
                shutdown();
            }

            // add the world
            if (isGamePaused) {
                float renderPartialTicks = timer.renderPartialTicks;
                timer.updateTimer();
                timer.renderPartialTicks = renderPartialTicks;
            } else {
                timer.updateTimer();
            }

            for (int i = 0; i < timer.elapsedTicks; i++) {
                ++ticksRan;

                try {
                    runTick();
                } catch (std::exception &e) {
                    printf("Exception caught in main: %s\n", e.what());
                }
            }

            lwjgl::Display::update();

            ++fps;
            if (currentTime + 1000 < System::currentTimeMillis()) {
                printf("FPS: %d\n", fps);
                fps = 0;
                currentTime = System::currentTimeMillis();
            }
        }
    } catch (std::exception &e) {
        printf("Exception caught in main: %s\n", e.what());
    }
}

void Minecraft::shutdown() {
    running = false;
}

void Minecraft::runTick() {
    systemTime = System::currentTimeMillis();
}
