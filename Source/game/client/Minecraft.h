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
#include "MouseHelper.h"
#include "OpenGlCapsChecker.h"
#include "gui/GuiScreen.h"
#include "options/GameSettings.h"
#include "renderer/FontRenderer.h"
#include "renderer/RenderEngine.h"
#include "renderer/RenderGlobal.h"
#include "game/entity/EntityPlayerSP.h"
#include "game/world/World.h"

enum EnumOS {
    UNKNOWN,
    WINDOWS,
    LINUX,
    MACOS,
};

class Minecraft {
private:
    bool fullscreen = false;
    OpenGlCapsChecker glCapabilities;
    Timer timer = Timer(20.0f);
    int_t ticksRan = 0;
    long_t systemTime = System::currentTimeMillis();
    std::unique_ptr<File> minecraftDir;

public:
    int_t displayWidth;
    int_t displayHeight;
    std::unique_ptr<World> theWorld = nullptr;
    std::unique_ptr<EntityPlayerSP> thePlayer = nullptr;
    std::atomic<bool> running = false;
    std::atomic<bool> isGamePaused = false;
    std::unique_ptr<GameSettings> options;
    std::unique_ptr<RenderEngine> renderEngine;
    std::unique_ptr<FontRenderer> fontRenderer;
    std::unique_ptr<RenderGlobal> renderGlobal;
    MouseHelper mouseHelper = MouseHelper(*this);
    std::unique_ptr<File> mcDataDir;
    std::shared_ptr<GuiScreen> currentScreen = nullptr;

    Minecraft(int_t displayWidth, int_t displayHeight, bool fullscreen);

    ~Minecraft();

    void startGame();

    void loadScreen();

    void scaledTessellator(int_t x, int_t y, int_t u, int_t v, int_t width, int_t height);

    void run();

    void shutdown();

    void runTick();

    void displayGuiScreen(std::shared_ptr<GuiScreen> guiScreen);

    void startWorld(const jstring &levelName);

    void changeWorld1(std::unique_ptr<World> world);

    void changeWorld(std::unique_ptr<World> world, const jstring &loadingMessage);

    bool isMultiplayerWorld() const;

    double getPlayerPosX() const;

    double getPlayerPosY() const;

    double getPlayerPosZ() const;

    void setIngameFocus();

    void setIngameNotInFocus();

    File getMinecraftDir();

    File getAppDir(const jstring &appDir);

private:
    void checkGLError(const std::string &message);

    void renderCurrentScreen(float partialTicks);

    void resize(int_t width, int_t height);

    void handleIngameInput();

    void updatePlayerLook();

    EnumOS getOs();
};


#endif //MCPORT_MINECRAFT_H
