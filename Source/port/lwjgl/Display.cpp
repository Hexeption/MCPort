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
#include "utils/SDLException.h"

namespace lwjgl {
    namespace Display {
        static bool closeRequested = false;
        static DisplayMode currentDisplayMode(0, 0);

        static void updateCurrentDisplayModeFromWindow() {
            int w = 0;
            int h = 0;
            SDL_GetWindowSizeInPixels(GLContext::detail::getWindow(), &w, &h);
            currentDisplayMode = DisplayMode(w, h);
        }

        void setDisplayMode(const DisplayMode &mode) {
            if (!mode.isFullscreen()) {
                SDL_SetWindowSize(GLContext::detail::getWindow(), mode.getWidth(), mode.getHeight());
            }
            currentDisplayMode = mode;
            setFullscreen(mode.isFullscreen());
        }

        DisplayMode getDisplayMode() {
            return currentDisplayMode;
        }

        void setTitle(const jstring &title) {
            const std::string utf8Title = String::toUtf8(title);
            SDL_SetWindowTitle(GLContext::detail::getWindow(), utf8Title.c_str());
        }

        void setFullscreen(bool fullscreen) {
            if (!SDL_SetWindowFullscreen(GLContext::detail::getWindow(), fullscreen)) {
                throw SDLException("setting fullscreen mode");
            }

            if (fullscreen) {
                const SDL_DisplayMode *sdlMode = SDL_GetWindowFullscreenMode(GLContext::detail::getWindow());
                if (sdlMode != nullptr) {
                    currentDisplayMode = DisplayMode(
                        sdlMode->w,
                        sdlMode->h,
                        SDL_BITSPERPIXEL(sdlMode->format),
                        static_cast<int_t>(sdlMode->refresh_rate)
                    );
                } else {
                    updateCurrentDisplayModeFromWindow();
                }
            } else {
                updateCurrentDisplayModeFromWindow();
            }
        }

        void create() {
            SDL_ShowWindow(GLContext::detail::getWindow());
        }

        bool isCloseRequested() { return closeRequested; }

        bool isVisible() {
            const auto windowFlags = SDL_GetWindowFlags(GLContext::detail::getWindow());
            return (windowFlags & SDL_WINDOW_HIDDEN) == 0;
        }

        bool isActive() {
            const auto windowFlags = SDL_GetWindowFlags(GLContext::detail::getWindow());
            return (windowFlags & SDL_WINDOW_INPUT_FOCUS) != 0;
        }

        int_t getWidth() {
            updateCurrentDisplayModeFromWindow();
            return currentDisplayMode.getWidth();
        }

        int_t getHeight() {
            updateCurrentDisplayModeFromWindow();
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

            if (!currentDisplayMode.isFullscreen()) {
                updateCurrentDisplayModeFromWindow();
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
