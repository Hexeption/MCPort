//
// Created by Keir Davis on 23/06/2026.
//

#include "Minecraft.h"

#include <cstdlib>
#include <stdexcept>

#include "java/System.h"
#include "lwjgl/Display.h"

namespace {
    bool contains(const jstring &value, const jstring &needle) {
        return value.find(needle) != jstring::npos;
    }

    jstring lowerCase(jstring value) {
        for (auto &ch: value) {
            if (ch >= u'A' && ch <= u'Z') {
                ch = static_cast<char16_t>(ch - u'A' + u'a');
            }
        }
        return value;
    }

    jstring dotAppDir(const jstring &appDir) {
        jstring dir = u".";
        dir += appDir;
        dir += u"/";
        return dir;
    }
}

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

    const std::string minecraftDirPath = String::toUtf8(getMinecraftDir().getAbsolutePath());
    printf("Dir %s\n", minecraftDirPath.c_str());
}

File Minecraft::getMinecraftDir() {
    if (minecraftDir == nullptr) {
        minecraftDir = std::make_unique<File>(getAppDir(u"mcport"));
    }
    return *minecraftDir;
}

File Minecraft::getAppDir(const jstring &appDir) {
    const jstring userHome = System::getProperty(u"user.home", u".");
    File appDirectory(userHome, appDir + u"/");
    switch (getOs()) {
        case LINUX:
            appDirectory = File(userHome, dotAppDir(appDir));
            break;
        case WINDOWS:
            if (const char *appData = std::getenv("APPDATA")) {
                appDirectory = File(String::fromUtf8(appData), dotAppDir(appDir));
            } else {
                appDirectory = File(userHome, dotAppDir(appDir));
            }
            break;
        case MACOS:
            appDirectory = File(userHome, u"Library/Application Support/" + appDir);
            break;
        default:
            appDirectory = File(userHome, appDir + u"/");
            break;
    }

    if (!appDirectory.exists() && !appDirectory.mkdirs()) {
        throw std::runtime_error("The working directory could not be created: " + appDirectory.toUtf8());
    }

    return appDirectory;
}

EnumOS Minecraft::getOs() {
    const jstring os = lowerCase(System::getProperty(u"os.name"));
    if (contains(os, u"win")) {
        return WINDOWS;
    }
    if (contains(os, u"mac")) {
        return MACOS;
    }
    if (contains(os, u"linux") || contains(os, u"unix")) {
        return LINUX;
    }
    return UNKNOWN;
}
