//
// Created by Keir Davis on 22/06/2026.
//

#include "lwjgl/Display.h"
#include "lwjgl/GLContext.h"
#include "SDL3/SDL.h"
#include "utils/SDLException.h"

#include <exception>
#include <cstdio>

#include "game/client/Minecraft.h"

int main(int argc, char *argv[]) {
    try {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw SDLException("initializing SDL");
        }

        lwjgl::GLContext::instantiate();

        Minecraft minecraft(1280, 720, false);
        minecraft.run();
    } catch (const std::exception &e) {
        printf("Exception caught in main: %s\n", e.what());
    }

    return 0;
}
