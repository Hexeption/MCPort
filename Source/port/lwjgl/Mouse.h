//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_MOUSE_H
#define MCPORT_MOUSE_H

#include "java/Type.h"
#include <SDL3/SDL.h>

namespace lwjgl {
    namespace Mouse {
        namespace detail {
            void pushEvent(const SDL_Event &e);
        }

        void setCursorPosition(int_t x, int_t y);

        bool next();

        int_t getEventButton();

        bool getEventButtonState();

        int_t getEventDX();

        int_t getEventDY();

        int_t getEventX();

        int_t getEventY();

        int_t getEventDWheel();

        int_t getX();

        int_t getY();

        int_t getDX();

        int_t getDY();

        int_t getDWheel();

        bool isButtonDown(int_t button);

        bool isGrabbed();

        void setGrabbed(bool grabbed);
    }
}

#endif //MCPORT_MOUSE_H
