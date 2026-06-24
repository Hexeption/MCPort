//
// Created by Keir Davis on 23/06/2026.
//

#include "Minecraft.h"

#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "game/client/options/GameSettings.h"
#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/RenderEngine.h"
#include "game/client/renderer/ScaledResolution.h"
#include "game/client/renderer/Tessellator.h"
#include "java/System.h"
#include "lwjgl/Display.h"

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

#if MCPORT_USE_LOCAL_MCDATA_DIR
    mcDataDir = std::make_unique<File>(u".");
#else
    mcDataDir = std::make_unique<File>(getMinecraftDir(), u"mcport");
#endif

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

            checkGLError("Pre render");

            if (renderEngine != nullptr) {
                loadScreen();
                lwjgl::Display::processMessages();
            } else {
                lwjgl::Display::update();
            }

            displayWidth = lwjgl::Display::getWidth();
            displayHeight = lwjgl::Display::getHeight();
            if (displayWidth <= 0) {
                displayWidth = 1;
            }
            if (displayHeight <= 0) {
                displayHeight = 1;
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
