//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_RUBYDUNG_H
#define MCPORT_RUBYDUNG_H

#include <java/Type.h>
#include <memory>
#include <optional>

#include "HitResult.h"
#include "Player.h"
#include "Timer.h"
#include "level/Level.h"
#include "level/LevelRenderer.h"
#include "lwjgl/BufferUtils.h"
#include "particle/ParticleEngine.h"

class RubyDung {
public:
    RubyDung(int_t width, int_t height);

    void init();

    void run();

    void tick();

    void render(float a);

    void destory();

private:
    void moveCameraToPlayer(float a);

    void setupCamera(float a);

    void setupPickCamera(float a, int x, int y);

    void pick(float a);

    void drawGui(float a);

    void setupFog(int_t i);

public:
    int_t width = 0;
    int_t height = 0;

private:
    lwjgl::FloatBuffer fogColor0 = lwjgl::BufferUtils::createFloatBuffer(4);
    lwjgl::FloatBuffer fogColor1 = lwjgl::BufferUtils::createFloatBuffer(4);
    lwjgl::FloatBuffer lightBrightnessBuffer = lwjgl::BufferUtils::createFloatBuffer(16);
    lwjgl::IntBuffer viewportBuffer = lwjgl::BufferUtils::createIntBuffer(16);
    lwjgl::IntBuffer selectBuffer = lwjgl::BufferUtils::createIntBuffer(2000);
    std::optional<HitResult> hitResult;

    Timer timer = Timer(20.0f);

    std::unique_ptr<Level> level = nullptr;
    std::unique_ptr<LevelRenderer> levelRenderer = nullptr;
    std::unique_ptr<Player> player = nullptr;
    int_t paintTexture = 1;
    std::unique_ptr<ParticleEngine> particleEngine = nullptr;
    int_t startupFrames = 0;
};


#endif //MCPORT_RUBYDUNG_H
