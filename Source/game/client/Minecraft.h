//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_MINECRAFT_H
#define MCPORT_MINECRAFT_H
#include <atomic>

#include "Timer.h"
#include "java/System.h"
#include "java/Type.h"


class Minecraft {
private:
    bool fullscreen = false;
    Timer timer = Timer(20.0f);
    int_t ticksRan = 0;
    long_t systemTime = System::currentTimeMillis();

public:
    int_t displayWidth;
    int_t displayHeight;
    std::atomic<bool> running = false;
    std::atomic<bool> isGamePaused = false;

    Minecraft(int_t displayWidth, int_t displayHeight, bool fullscreen);

    void startGame();

    void run();

    void shutdown();

    void runTick();
};


#endif //MCPORT_MINECRAFT_H
