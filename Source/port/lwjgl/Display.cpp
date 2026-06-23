//
// Created by Keir Davis on 22/06/2026.
//

#include "Display.h"

#include <SDL3/SDL.h>
#include <string>

#include "java/String.h"
#include "GLContext.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace lwjgl {
    namespace Display {
        static bool closeRequested = false;
        static DisplayMode currentDisplayMode(0, 0);

        void setDisplayMode(const DisplayMode &mode) {
            currentDisplayMode = mode;
        }

        DisplayMode getDisplayMode() {
            return currentDisplayMode;
        }

        void setTitle(const jstring &title) {
            const std::string utf8Title = String::toUtf8(title);
            SDL_SetWindowTitle(GLContext::detail::getWindow(), utf8Title.c_str());
        }

        void create() {
            SDL_ShowWindow(GLContext::detail::getWindow());
        }

        bool isCloseRequested() { return closeRequested; }

        bool isVisible() {
            const auto windowFlags = SDL_GetWindowFlags(GLContext::detail::getWindow());
            return (windowFlags & SDL_WINDOW_HIDDEN) == 0;
        }

        int_t getWidth() {
            return currentDisplayMode.getWidth();
        }

        int_t getHeight() {
            return currentDisplayMode.getHeight();
        }

        void processMessages() {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_EVENT_QUIT:
                        closeRequested = true;
                        break;
                    case SDL_EVENT_KEY_DOWN:
                    case SDL_EVENT_KEY_UP:
                    case SDL_EVENT_TEXT_INPUT:
                        Keyboard::detail::pushEvent(event);
                        break;
                    case SDL_EVENT_MOUSE_MOTION:
                    case SDL_EVENT_MOUSE_WHEEL:
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        Mouse::detail::pushEvent(event);
                        break;
                }
            }
        }

        void swapBuffers() {
            SDL_GL_SwapWindow(GLContext::detail::getWindow());
        }

        void update(bool doProcessEvents) {
            swapBuffers();
            if (doProcessEvents) {
                processMessages();
            }
        }

        void Destory() {
            GLContext::destroy();
            SDL_Quit();
            closeRequested = false;
        }
    }
}
