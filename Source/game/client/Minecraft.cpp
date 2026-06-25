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
#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/ScaledResolution.h"
#include "game/client/renderer/Tessellator.h"
#include "game/util/Vec3D.h"
#include "java/Math.h"
#include "java/System.h"
#include "lwjgl/Display.h"
#include "lwjgl/Mouse.h"
#include "utils/GLU.h"

#include <glad/glad.h>

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
    renderGlobal = std::make_unique<RenderGlobal>(*this, *renderEngine);
    glViewport(0, 0, displayWidth, displayHeight);

    checkGLError("Post startup");
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

            renderCurrentScreen(timer.renderPartialTicks);

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
    }
}

void Minecraft::displayGuiScreen(std::shared_ptr<GuiScreen> guiScreen) {
    if (currentScreen != nullptr) {
        currentScreen->onGuiClosed();
    }

    currentScreen = std::move(guiScreen);
    if (currentScreen == nullptr) {
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
        mouseHelper.grabMouseCursor();
    }
}

void Minecraft::setIngameNotInFocus() {
    if (thePlayer != nullptr) {
        thePlayer->resetPlayerKeyState();
    }
    mouseHelper.ungrabMouseCursor();
}

// Temp till Entity renderer
void Minecraft::renderCurrentScreen(const float partialTicks) {
    ScaledResolution scaledResolution(displayWidth, displayHeight);
    const int_t scaledWidth = scaledResolution.getScaledWidth();
    const int_t scaledHeight = scaledResolution.getScaledHeight();

    glViewport(0, 0, displayWidth, displayHeight);

    if (theWorld != nullptr) {
        if (currentScreen == nullptr && thePlayer != nullptr) {
            updatePlayerLook();
        }

        std::unique_ptr<Vec3D> skyColor = theWorld->getSkyColor(partialTicks);
        glClearColor(static_cast<float>(skyColor->xCoord), static_cast<float>(skyColor->yCoord),
                     static_cast<float>(skyColor->zCoord), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLU::gluPerspective(70.0f, static_cast<float>(displayWidth) / static_cast<float>(displayHeight), 0.05f,
                            100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        double cameraX = 0.0;
        double cameraY = 0.0;
        double cameraZ = 0.0;
        float cameraYaw = 0.0f;
        float cameraPitch = 0.0f;
        if (thePlayer != nullptr) {
            cameraX = thePlayer->prevPosX + (thePlayer->posX - thePlayer->prevPosX) * partialTicks;
            cameraY = thePlayer->prevPosY + (thePlayer->posY - thePlayer->prevPosY) * partialTicks;
            cameraZ = thePlayer->prevPosZ + (thePlayer->posZ - thePlayer->prevPosZ) * partialTicks;
            cameraYaw = thePlayer->prevRotationYaw + (thePlayer->rotationYaw - thePlayer->prevRotationYaw) *
                        partialTicks;
            cameraPitch = thePlayer->prevRotationPitch +
                          (thePlayer->rotationPitch - thePlayer->prevRotationPitch) * partialTicks;
        }
        glRotatef(cameraPitch, 1.0f, 0.0f, 0.0f);
        glRotatef(cameraYaw + 180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(static_cast<float>(-cameraX), static_cast<float>(-cameraY), static_cast<float>(-cameraZ));
        renderGlobal->renderWorld(partialTicks);
        if (currentScreen == nullptr) {
            return;
        }
    } else {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(scaledWidth), static_cast<double>(scaledHeight), 0.0, 1000.0, 3000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2000.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    if (currentScreen != nullptr) {
        const int_t mouseX = lwjgl::Mouse::getX() * scaledWidth / displayWidth;
        const int_t mouseY = scaledHeight - lwjgl::Mouse::getY() * scaledHeight / displayHeight - 1;
        currentScreen->drawScreen(mouseX, mouseY, partialTicks);
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
    while (lwjgl::Keyboard::next()) {
        const bool pressed = lwjgl::Keyboard::getEventKeyState();
        const int_t key = lwjgl::Keyboard::getEventKey();
        if (thePlayer != nullptr) {
            thePlayer->handleKeyPress(key, pressed);
        }
        if (pressed && key == lwjgl::Keyboard::KEY_ESCAPE) {
            displayGuiScreen(std::make_shared<GuiIngameMenu>());
            return;
        }
    }

    if (thePlayer != nullptr && !isGamePaused) {
        thePlayer->onUpdate();
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
