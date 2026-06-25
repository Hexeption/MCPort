//
// Created by Keir Davis on 23/06/2026.
//

#include "Minecraft.h"

#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "game/client/options/GameSettings.h"
#include "game/client/gui/GuiIngameMenu.h"
#include "game/client/gui/GuiMainMenu.h"
#include "game/client/gui/GuiScreen.h"
#include "game/client/renderer/EntityRenderer.h"
#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/ScaledResolution.h"
#include "game/client/renderer/Tessellator.h"
#include "game/client/renderer/TextureLavaFX.h"
#include "game/client/renderer/TextureLavaFlowFX.h"
#include "game/client/renderer/TextureWaterFX.h"
#include "game/client/renderer/TextureWaterFlowFX.h"
#include "game/client/player/PlayerControllerSP.h"
#include "java/Math.h"
#include "java/System.h"
#include "lwjgl/Display.h"
#include "lwjgl/Mouse.h"

#include <glad/glad.h>

#include "renderer/WorldRenderer.h"

namespace {
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

Minecraft::~Minecraft() = default;

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

    mcDataDir = std::make_unique<File>(getMinecraftDir());

    options = std::make_unique<GameSettings>(*this, *mcDataDir);
    renderEngine = std::make_unique<RenderEngine>(options.get());
    fontRenderer = std::make_unique<FontRenderer>(options.get(), u"/default.png", *renderEngine);
    loadScreen();

    checkGLError("Pre startup");
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1F);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    checkGLError("Startup");

    glCapabilities = OpenGlCapsChecker();
    renderEngine->registerTextureFX(new TextureLavaFX());
    renderEngine->registerTextureFX(new TextureWaterFX());
    renderEngine->registerTextureFX(new TextureWaterFlowFX());
    renderEngine->registerTextureFX(new TextureLavaFlowFX());
    renderGlobal = std::make_unique<RenderGlobal>(*this, *renderEngine);
    entityRenderer = std::make_unique<EntityRenderer>(*this);
    playerController = std::make_unique<PlayerControllerSP>(*this);
    glViewport(0, 0, displayWidth, displayHeight);

    checkGLError("Post startup");
    ingameGui = std::make_unique<GuiIngame>(*this);
    displayGuiScreen(std::make_shared<GuiMainMenu>());
}

void Minecraft::loadScreen() {
    ScaledResolution scaledResolution(displayWidth, displayHeight);
    const int_t scaledWidth = scaledResolution.getScaledWidth();
    const int_t scaledHeight = scaledResolution.getScaledHeight();

    glViewport(0, 0, displayWidth, displayHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(scaledWidth), static_cast<double>(scaledHeight), 0.0, 1000.0, 3000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2000.0f);

    Tessellator &tessellator = Tessellator::instance;
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_FOG);

    glBindTexture(GL_TEXTURE_2D, renderEngine->getTexture(u"/title/mojang.png"));
    tessellator.startDrawingQuads();
    tessellator.setColorOpaque_I(16777215);
    tessellator.addVertexWithUV(0.0, static_cast<double>(scaledHeight), 0.0, 0.0, 0.0);
    tessellator.addVertexWithUV(static_cast<double>(scaledWidth), static_cast<double>(scaledHeight), 0.0, 0.0, 0.0);
    tessellator.addVertexWithUV(static_cast<double>(scaledWidth), 0.0, 0.0, 0.0, 0.0);
    tessellator.addVertexWithUV(0.0, 0.0, 0.0, 0.0, 0.0);
    tessellator.draw();

    constexpr short_t logoWidth = 256;
    constexpr short_t logoHeight = 256;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    tessellator.setColorOpaque_I(16777215);
    scaledTessellator((scaledWidth - logoWidth) / 2, (scaledHeight - logoHeight) / 2, 0, 0, logoWidth, logoHeight);
    const jstring demoText = u"FontRenderer demo: \u00A7aMinecraft Alpha";
    const int_t demoTextY = std::min<int_t>((scaledHeight + logoHeight) / 2 + 12, scaledHeight - 18);
    fontRenderer->drawStringWithShadow(demoText, (scaledWidth - fontRenderer->getStringWidth(demoText)) / 2,
                                       demoTextY, 16777215);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    lwjgl::Display::swapBuffers();
}

void Minecraft::scaledTessellator(const int_t x, const int_t y, const int_t u, const int_t v, const int_t width,
                                  const int_t height) {
    constexpr float textureScaleU = 0.00390625f;
    constexpr float textureScaleV = 0.00390625f;
    Tessellator &tessellator = Tessellator::instance;
    tessellator.startDrawingQuads();
    tessellator.addVertexWithUV(static_cast<double>(x), static_cast<double>(y + height), 0.0,
                                static_cast<double>(static_cast<float>(u) * textureScaleU),
                                static_cast<double>(static_cast<float>(v + height) * textureScaleV));
    tessellator.addVertexWithUV(static_cast<double>(x + width), static_cast<double>(y + height), 0.0,
                                static_cast<double>(static_cast<float>(u + width) * textureScaleU),
                                static_cast<double>(static_cast<float>(v + height) * textureScaleV));
    tessellator.addVertexWithUV(static_cast<double>(x + width), static_cast<double>(y), 0.0,
                                static_cast<double>(static_cast<float>(u + width) * textureScaleU),
                                static_cast<double>(static_cast<float>(v) * textureScaleV));
    tessellator.addVertexWithUV(static_cast<double>(x), static_cast<double>(y), 0.0,
                                static_cast<double>(static_cast<float>(u) * textureScaleU),
                                static_cast<double>(static_cast<float>(v) * textureScaleV));
    tessellator.draw();
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
            lwjgl::Display::processMessages();

            if (lwjgl::Display::isCloseRequested()) {
                shutdown();
            }

            // add the world
            if (isGamePaused && theWorld != nullptr) {
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

            checkGLError("Pre render");

            glEnable(GL_TEXTURE_2D);
            if (theWorld != nullptr) {
                while (theWorld->updatingLighting()) {
                }
            }

            if (!skipRenderWorld) {
                if (playerController != nullptr) {
                    playerController->setPartialTime(timer.renderPartialTicks);
                }

                entityRenderer->updateCameraAndRender(timer.renderPartialTicks);
            }

            lwjgl::Display::update();
            if (!fullscreen && (lwjgl::Display::getWidth() != displayWidth || lwjgl::Display::getHeight() !=
                                displayHeight)) {
                displayWidth = lwjgl::Display::getWidth();
                displayHeight = lwjgl::Display::getHeight();
                if (displayWidth <= 0) {
                    displayWidth = 1;
                }

                if (displayHeight <= 0) {
                    displayHeight = 1;
                }

                resize(displayWidth, displayHeight);
            }

            if (options->limitFramerate) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }

            checkGLError("Post render");
            ++fps;
            for (isGamePaused = !isMultiplayerWorld() && currentScreen != nullptr && currentScreen->doesGuiPauseGame();
                 System::currentTimeMillis() >= currentTime + 1000L; fps = 0) {
                debug = String::toString(fps) + u" fps, " + String::toString(WorldRenderer::chunksUpdated) +
                        u" chunk updates";
                WorldRenderer::chunksUpdated = 0;
                currentTime += 1000L;
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
    ingameGui->updateTick();
    systemTime = System::currentTimeMillis();
    if (currentScreen != nullptr) {
        std::shared_ptr<GuiScreen> screen = currentScreen;
        screen->handleInput();
        if (screen == currentScreen) {
            screen->updateScreen();
        }
    } else if (theWorld != nullptr) {
        handleIngameInput();
    }

    if (theWorld != nullptr) {
        theWorld->difficultySetting = options != nullptr ? options->difficulty : 0;
        if (!isGamePaused || isMultiplayerWorld()) {
            theWorld->tick();
        }
        if (playerController != nullptr) {
            playerController->onUpdate();
        }
        if (entityRenderer != nullptr) {
            entityRenderer->updateRenderer();
        }
    }

    glBindTexture(GL_TEXTURE_2D, renderEngine->getTexture(u"/terrain.png"));
    if (!isGamePaused) {
        renderEngine->updateDynamicTextures();
    }
}

void Minecraft::displayGuiScreen(std::shared_ptr<GuiScreen> guiScreen) {
    if (currentScreen != nullptr) {
        currentScreen->onGuiClosed();
    }

    currentScreen = std::move(guiScreen);
    if (currentScreen == nullptr) {
        isGamePaused = false;
        setIngameFocus();
        return;
    }

    setIngameNotInFocus();
    ScaledResolution scaledResolution(displayWidth, displayHeight);
    currentScreen->setWorldAndResolution(this, scaledResolution.getScaledWidth(), scaledResolution.getScaledHeight());
    isGamePaused = currentScreen->doesGuiPauseGame();
}

void Minecraft::startWorld(const jstring &levelName) {
    changeWorld1(nullptr);
    auto world = std::make_unique<World>(File(getMinecraftDir(), u"saves"), levelName);
    if (world->isNewWorld) {
        changeWorld(std::move(world), u"Generating level");
    } else {
        changeWorld(std::move(world), u"Loading level");
    }
}

void Minecraft::changeWorld1(std::unique_ptr<World> world) {
    changeWorld(std::move(world), u"");
}

void Minecraft::changeWorld(std::unique_ptr<World> world, const jstring &) {
    theWorld = std::move(world);
    if (theWorld != nullptr) {
        thePlayer = std::make_unique<EntityPlayerSP>(*theWorld, *options);
        thePlayer->preparePlayerToSpawn();
    } else {
        thePlayer = nullptr;
        setIngameNotInFocus();
    }
    if (renderGlobal != nullptr) {
        renderGlobal->changeWorld(theWorld.get());
    }
    if (theWorld != nullptr && playerController != nullptr) {
        playerController->onWorldChange(*theWorld);
    }
}

bool Minecraft::isMultiplayerWorld() const {
    return theWorld != nullptr && theWorld->multiplayerWorld;
}

double Minecraft::getPlayerPosX() const {
    return thePlayer != nullptr ? thePlayer->posX : 0.0;
}

double Minecraft::getPlayerPosY() const {
    return thePlayer != nullptr ? thePlayer->posY : 0.0;
}

double Minecraft::getPlayerPosZ() const {
    return thePlayer != nullptr ? thePlayer->posZ : 0.0;
}

void Minecraft::setIngameFocus() {
    isGamePaused = false;
    if (theWorld != nullptr) {
        inGameHasFocus = true;
        mouseHelper.grabMouseCursor();
    }
}

void Minecraft::setIngameNotInFocus() {
    if (inGameHasFocus) {
        if (thePlayer != nullptr) {
            thePlayer->resetPlayerKeyState();
        }
        inGameHasFocus = false;
        mouseHelper.ungrabMouseCursor();
    }
}

void Minecraft::resize(int_t width, int_t height) {
    if (width <= 0) {
        width = 1;
    }

    if (height <= 0) {
        height = 1;
    }

    displayWidth = width;
    displayHeight = height;

    if (currentScreen != nullptr) {
        ScaledResolution scaledResolution(width, height);
        currentScreen->setWorldAndResolution(this, scaledResolution.getScaledWidth(),
                                             scaledResolution.getScaledHeight());
    }
}

void Minecraft::handleIngameInput() {
    if (leftClickCounter > 0) {
        --leftClickCounter;
    }

    while (lwjgl::Keyboard::next()) {
        const bool pressed = lwjgl::Keyboard::getEventKeyState();
        const int_t key = lwjgl::Keyboard::getEventKey();
        if (thePlayer != nullptr) {
            thePlayer->handleKeyPress(key, pressed);
        }
        if (pressed && key == lwjgl::Keyboard::KEY_ESCAPE) {
            displayInGameMenu();
        }

        if (pressed && key == lwjgl::Keyboard::KEY_F8) {
            thePlayer->noclip = !thePlayer->noclip;
            return;
        }

        if (pressed && key == options->keyBindToggleFog.key) {
            const int_t direction = !lwjgl::Keyboard::isKeyDown(lwjgl::Keyboard::KEY_LSHIFT) &&
                                    !lwjgl::Keyboard::isKeyDown(lwjgl::Keyboard::KEY_RSHIFT)
                                        ? 1
                                        : -1;
            options->setOptionValue(4, direction);
        }
    }

    while (lwjgl::Mouse::next()) {
        if (currentScreen != nullptr || theWorld == nullptr || thePlayer == nullptr) {
            continue;
        }

        const int_t button = lwjgl::Mouse::getEventButton();
        if (lwjgl::Mouse::getEventButtonState() && (button == 0 || button == 1)) {
            clickMouse(button);
        }
    }

    sendClickBlockToController(0, currentScreen == nullptr && lwjgl::Mouse::isButtonDown(0));

    if (thePlayer != nullptr && !isGamePaused) {
        thePlayer->onUpdate();
    }
}

void Minecraft::displayInGameMenu() {
    if (currentScreen == nullptr) {
        displayGuiScreen(std::make_shared<GuiIngameMenu>());
    }
}

jstring Minecraft::debugInfoRenders() {
    return renderGlobal->getDebugInfoRenders();
}

jstring Minecraft::getEntityDebug() {
    return u"";
}

jstring Minecraft::debugInfoEntities() {
    return u"";
}

void Minecraft::sendClickBlockToController(const int_t button, const bool pressed) {
    if (playerController == nullptr || playerController->isInTestMode) {
        return;
    }
    if (button != 0 || leftClickCounter <= 0) {
        if (pressed && objectMouseOver != nullptr && objectMouseOver->typeOfHit == 0 && button == 0) {
            playerController->sendBlockRemoving(objectMouseOver->blockX, objectMouseOver->blockY,
                                                objectMouseOver->blockZ, objectMouseOver->sideHit);
        } else {
            playerController->resetBlockRemoving();
        }
    }
}

void Minecraft::clickMouse(const int_t button) {
    if (playerController == nullptr || (button == 0 && leftClickCounter > 0)) {
        return;
    }

    if (button == 0 && thePlayer != nullptr) {
        thePlayer->swingItem();
    }

    if (objectMouseOver == nullptr) {
        if (button == 0) {
            leftClickCounter = 10;
        }
        return;
    }

    if (objectMouseOver->typeOfHit == 0) {
        const int_t x = objectMouseOver->blockX;
        const int_t y = objectMouseOver->blockY;
        const int_t z = objectMouseOver->blockZ;
        const int_t side = objectMouseOver->sideHit;
        if (button == 0) {
            playerController->clickBlock(x, y, z, side);
        }
    }
}

void Minecraft::updatePlayerLook() {
    if (thePlayer == nullptr) {
        return;
    }

    mouseHelper.mouseXYChange();
    thePlayer->setAngles(static_cast<float>(mouseHelper.deltaX), static_cast<float>(mouseHelper.deltaY));
}

void Minecraft::checkGLError(const std::string &message) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::string errorString;
        switch (error) {
            case GL_NO_ERROR:
                errorString = "No error";
                break;
            case GL_INVALID_ENUM:
                errorString = "Invalid enum";
                break;
            case GL_INVALID_VALUE:
                errorString = "Invalid value";
                break;
            case GL_INVALID_OPERATION:
                errorString = "Invalid operation";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "Stack overflow";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "Stack underflow";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "Out of memory";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "Invalid framebuffer operation";
                break;
            case GL_TABLE_TOO_LARGE:
                errorString = "Table too large";
                break;
            default:
                errorString = "unknown error code";
                break;
        }

        std::cout << "########## GL ERROR ##########\n";
        std::cout << "@ " << message << '\n';
        std::cout << error << ": " << errorString << '\n';
    }
}

File Minecraft::getMinecraftDir() {
    if (minecraftDir == nullptr) {
#if MCPORT_USE_LOCAL_MCDATA_DIR
        minecraftDir = std::make_unique<File>(u".");
#else
        minecraftDir = std::make_unique<File>(getAppDir(u"mcport"));
#endif
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
    const jstring os = String::lowerCase(System::getProperty(u"os.name"));
    if (String::contains(os, u"win")) {
        return WINDOWS;
    }
    if (String::contains(os, u"mac")) {
        return MACOS;
    }
    if (String::contains(os, u"linux") || String::contains(os, u"unix")) {
        return LINUX;
    }
    return UNKNOWN;
}
