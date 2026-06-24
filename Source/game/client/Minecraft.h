//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_MINECRAFT_H
#define MCPORT_MINECRAFT_H
#include <atomic>
#include <memory>

#include "Timer.h"
#include "java/File.h"
#include "java/System.h"
#include "java/Type.h"

class GameSettings;

enum EnumOS {
    UNKNOWN,
    WINDOWS,
    LINUX,
    MACOS,
};

class Minecraft {
private:
    bool fullscreen = false;
    Timer timer = Timer(20.0f);
    int_t ticksRan = 0;
    long_t systemTime = System::currentTimeMillis();
    std::unique_ptr<File> minecraftDir;

public:
    int_t displayWidth;
    int_t displayHeight;
    std::atomic<bool> running = false;
    std::atomic<bool> isGamePaused = false;
    std::unique_ptr<GameSettings> options;
    std::unique_ptr<File> mcDataDir;

    Minecraft(int_t displayWidth, int_t displayHeight, bool fullscreen);

    ~Minecraft();

    void startGame();

    void run();

    void shutdown();

    void runTick();

    File getMinecraftDir();

    File getAppDir(const jstring &appDir);

private:
    EnumOS getOs();
};


#endif //MCPORT_MINECRAFT_H
