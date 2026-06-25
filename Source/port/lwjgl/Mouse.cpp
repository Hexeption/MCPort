//
// Created by Keir Davis on 23/06/2026.
//

#include "Mouse.h"

#include <queue>

#include "Display.h"
#include "GLContext.h"
#include "java/Type.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include "utils/SDLException.h"

namespace lwjgl {
    namespace Mouse {
        static int_t toLwjglButton(int_t button) {
            if (button == SDL_BUTTON_LEFT) {
                return 0;
            }
            if (button == SDL_BUTTON_RIGHT) {
                return 1;
            }
            if (button == SDL_BUTTON_MIDDLE) {
                return 2;
            }

            return button - 1;
        }

        static int_t staging_dx = 0;
        static int_t staging_dy = 0;
        static int_t staging_dz = 0;

        namespace detail {
            struct Event {
                Sint8 button, down;
                Sint32 x, y;
                Sint32 xrel, yrel;
                Sint32 wheel;

                Event(Sint8 button = 0, Sint8 down = 0, Sint32 x = 0, Sint32 y = 0, Sint32 xrel = 0, Sint32 yrel = 0,
                      Sint32 wheel = 0)
                    : button(button), down(down), x(x), y(y), xrel(xrel), yrel(yrel), wheel(wheel) {
                }
            };

            static Event event_current = {};
            static std::queue<Event> event_queue;

            void pushEvent(const SDL_Event &e) {
                switch (e.type) {
                    case SDL_EVENT_MOUSE_MOTION:
                        staging_dx += e.motion.xrel;
                        staging_dy -= e.motion.yrel;
                        event_queue.emplace(-1, 0, e.motion.x, Display::getHeight() - e.motion.y - 1, e.motion.xrel,
                                            -e.motion.yrel, 0);
                        break;
                    case SDL_EVENT_MOUSE_WHEEL:
                        staging_dz += e.wheel.y;
                        event_queue.emplace(-1, 0, e.wheel.mouse_x, Display::getHeight() - e.wheel.mouse_y - 1, 0, 0,
                                            e.wheel.y);
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        event_queue.emplace(toLwjglButton(e.button.button), 1, e.button.x,
                                            Display::getHeight() - e.button.y - 1,
                                            0, 0, 0);
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        event_queue.emplace(toLwjglButton(e.button.button), 0, e.button.x,
                                            Display::getHeight() - e.button.y - 1,
                                            0, 0, 0);
                        break;
                }
            }
        }

        void setCursorPosition(int_t x, int_t y) {
            SDL_WarpMouseInWindow(GLContext::detail::getWindow(), static_cast<float>(x), static_cast<float>(y));
        }

        // Event handling
        bool next() {
            if (detail::event_queue.empty())
                return false;
            detail::event_current = detail::event_queue.front();
            detail::event_queue.pop();
            return true;
        }

        int_t getEventButton() {
            return detail::event_current.button;
        }

        bool getEventButtonState() {
            return detail::event_current.down != 0;
        }

        int_t getEventDX() {
            return detail::event_current.xrel;
        }

        int_t getEventDY() {
            return detail::event_current.yrel;
        }

        int_t getEventX() {
            return detail::event_current.x;
        }

        int_t getEventY() {
            return detail::event_current.y;
        }

        int_t getEventDWheel() {
            return detail::event_current.wheel;
        }

        // State
        int_t getX() {
            float x;
            SDL_GetMouseState(&x, nullptr);
            return static_cast<int_t>(x);
        }

        int_t getY() {
            float y;
            SDL_GetMouseState(nullptr, &y);
            return lwjgl::Display::getHeight() - static_cast<int_t>(y) - 1;
        }

        int_t getDX() {
            int_t result = staging_dx;
            staging_dx = 0;
            return result;
        }

        int_t getDY() {
            int_t result = staging_dy;
            staging_dy = 0;
            return result;
        }

        int_t getDWheel() {
            int_t result = staging_dz;
            staging_dz = 0;
            return result;
        }

        bool isButtonDown(int_t button) {
            int_t sdlButton = button + 1;
            if (button == 1) {
                sdlButton = SDL_BUTTON_RIGHT;
            } else if (button == 2) {
                sdlButton = SDL_BUTTON_MIDDLE;
            }
            return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(sdlButton)) != 0;
        }

        bool isGrabbed() {
            return SDL_GetWindowRelativeMouseMode(GLContext::detail::getWindow());
        }

        void setGrabbed(bool grabbed) {
            staging_dx = 0;
            staging_dy = 0;

            if (grabbed) {
                if (!SDL_HideCursor())
                    throw SDLException();
            } else {
                if (!SDL_ShowCursor())
                    throw SDLException();
            }
            if (!SDL_SetWindowRelativeMouseMode(GLContext::detail::getWindow(), grabbed))
                throw SDLException();
        }
    }
}
