//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_RUBYDUNG_H
#define MCPORT_RUBYDUNG_H

#include <java/Type.h>

#include "Timer.h"
#include "lwjgl/BufferUtils.h"

class RubyDung {
public:
    RubyDung(int_t width, int_t height);

    void init();

    void run();

    void tick();

    void render(float a);

    void destory();

public:
    int_t width = 0;
    int_t height = 0;

private:
    lwjgl::FloatBuffer fogColor0 = lwjgl::BufferUtils::createFloatBuffer(4);
    lwjgl::FloatBuffer fogColor1 = lwjgl::BufferUtils::createFloatBuffer(4);
    lwjgl::IntBuffer viewportBuffer = lwjgl::BufferUtils::createIntBuffer(16);
    lwjgl::IntBuffer selectBuffer = lwjgl::BufferUtils::createIntBuffer(2000);

    Timer timer = Timer(20.0f);
};


#endif //MCPORT_RUBYDUNG_H
