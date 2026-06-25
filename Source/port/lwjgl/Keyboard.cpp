//
// Created by Keir Davis on 22/06/2026.
//

#include "Keyboard.h"

#include <queue>

#include "utils/SDLException.h"

namespace {
    struct KeyMapEntry {
        int_t lwjgl;
        SDL_Keycode sdl;
    };

    static const KeyMapEntry KEY_MAP[] = {
        {1, SDLK_ESCAPE},
        {2, SDLK_1},
        {3, SDLK_2},
        {4, SDLK_3},
        {5, SDLK_4},
        {6, SDLK_5},
        {7, SDLK_6},
        {8, SDLK_7},
        {9, SDLK_8},
        {10, SDLK_9},
        {11, SDLK_0},
        {12, SDLK_MINUS},
        {13, SDLK_EQUALS},
        {15, SDLK_TAB},
        {16, SDLK_Q},
        {17, SDLK_W},
        {18, SDLK_E},
        {19, SDLK_R},
        {20, SDLK_T},
        {21, SDLK_Y},
        {22, SDLK_U},
        {23, SDLK_I},
        {24, SDLK_O},
        {25, SDLK_P},
        {26, SDLK_LEFTBRACKET},
        {27, SDLK_RIGHTBRACKET},
        {28, SDLK_RETURN},
        {29, SDLK_LCTRL},
        {30, SDLK_A},
        {31, SDLK_S},
        {32, SDLK_D},
        {33, SDLK_F},
        {34, SDLK_G},
        {35, SDLK_H},
        {36, SDLK_J},
        {37, SDLK_K},
        {38, SDLK_L},
        {39, SDLK_SEMICOLON},
        {40, SDLK_APOSTROPHE},
        {41, SDLK_GRAVE},
        {42, SDLK_LSHIFT},
        {43, SDLK_BACKSLASH},
        {44, SDLK_Z},
        {45, SDLK_X},
        {46, SDLK_C},
        {47, SDLK_V},
        {48, SDLK_B},
        {49, SDLK_N},
        {50, SDLK_M},
        {51, SDLK_COMMA},
        {52, SDLK_PERIOD},
        {53, SDLK_SLASH},
        {54, SDLK_RSHIFT},
        {55, SDLK_KP_MULTIPLY},
        {57, SDLK_SPACE},
        {59, SDLK_F1},
        {60, SDLK_F2},
        {61, SDLK_F3},
        {62, SDLK_F4},
        {63, SDLK_F5},
        {64, SDLK_F6},
        {65, SDLK_F7},
        {66, SDLK_F8},
        {67, SDLK_F9},
        {68, SDLK_F10},
        {69, SDLK_NUMLOCKCLEAR},
        {71, SDLK_KP_7},
        {72, SDLK_KP_8},
        {73, SDLK_KP_9},
        {74, SDLK_KP_MINUS},
        {75, SDLK_KP_4},
        {76, SDLK_KP_5},
        {77, SDLK_KP_6},
        {78, SDLK_KP_PLUS},
        {79, SDLK_KP_1},
        {80, SDLK_KP_2},
        {81, SDLK_KP_3},
        {82, SDLK_KP_0},
        {83, SDLK_KP_DECIMAL},
        {87, SDLK_F11},
        {88, SDLK_F12},
        {100, SDLK_F13},
        {101, SDLK_F14},
        {102, SDLK_F15},
        {103, SDLK_F16},
        {104, SDLK_F17},
        {105, SDLK_F18},
        {113, SDLK_F19},
        {141, SDLK_KP_EQUALS},
        {145, SDLK_AT},
        {146, SDLK_COLON},
        {149, SDLK_STOP},
        {156, SDLK_KP_ENTER},
        {157, SDLK_RCTRL},
        {179, SDLK_KP_COMMA},
        {181, SDLK_KP_DIVIDE},
        {183, SDLK_SYSREQ},
        {197, SDLK_PAUSE},
        {199, SDLK_HOME},
        {200, SDLK_UP},
        {201, SDLK_PAGEUP},
        {203, SDLK_LEFT},
        {205, SDLK_RIGHT},
        {207, SDLK_END},
        {208, SDLK_DOWN},
        {210, SDLK_INSERT},
        {211, SDLK_DELETE},
        {218, SDLK_CLEAR},
        {219, SDLK_LGUI},
        {220, SDLK_RGUI},
        {222, SDLK_POWER},
        {223, SDLK_SLEEP},
    };

    static SDL_Keycode toSDLKeycode(int_t key) {
        for (const auto &entry: KEY_MAP) {
            if (entry.lwjgl == key)
                return entry.sdl;
        }
        return SDLK_UNKNOWN;
    }

    static int_t toLWJGLKey(SDL_Keycode key) {
        for (const auto &entry: KEY_MAP) {
            if (entry.sdl == key)
                return entry.lwjgl;
        }
        return 0;
    }
}

namespace lwjgl {
    namespace Keyboard {
        namespace detail {
            struct Event {
                int_t key;
                int_t character;
                bool repeat;
                bool down;
            };

            static Event event_current = {};
            static std::queue<Event> event_queue;

            static bool has_retained_event = false;
            static Event event_retained = {};

            static void flushRetained() {
                if (!has_retained_event)
                    return;

                has_retained_event = false;
                event_queue.push(event_retained);
            }

            static void handleKey(int_t key, bool repeat, bool down) {
                flushRetained();
                has_retained_event = true;

                event_retained.key = key;
                event_retained.character = 0;
                event_retained.repeat = repeat;
                event_retained.down = down;
            }

            static void handleCharacter(int_t character) {
                if (has_retained_event && event_retained.character != 0)
                    flushRetained();
                if (!has_retained_event)
                    event_queue.push({KEY_NONE, character, false, true});
                else
                    event_retained.character = character;
            }

            void pushEvent(const SDL_Event &e) {
                if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP) {
                    handleKey(toLWJGLKey(e.key.key), e.key.repeat, e.key.down);
                } else if (e.type == SDL_EVENT_TEXT_INPUT) {
                    for (char_t c: String::fromUtf8(e.text.text))
                        handleCharacter(c);
                }
            }
        }

        // Keyboard functions
        jstring getKeyName(int_t key) {
            return String::fromUtf8(SDL_GetKeyName(toSDLKeycode(key)));
        }

        // Event handling
        static bool allow_repeat_events = false;

        bool next() {
            detail::flushRetained();

            if (detail::event_queue.empty())
                return false;

            if (!allow_repeat_events) {
                while (1) {
                    if (detail::event_queue.empty())
                        return false;
                    if (detail::event_queue.front().repeat)
                        detail::event_queue.pop();
                    else
                        break;
                }
            }

            if (detail::event_queue.empty())
                return false;

            detail::event_current = detail::event_queue.front();
            detail::event_queue.pop();
            return true;
        }

        void enableRepeatEvents(bool repeat) {
            allow_repeat_events = repeat;
        }

        bool areRepeatEventsEnabled() {
            return allow_repeat_events;
        }

        char_t getEventCharacter() {
            return detail::event_current.character;
        }

        int_t getEventKey() {
            return detail::event_current.key;
        }

        bool getEventKeyState() {
            return detail::event_current.down;
        }

        static const bool *keyboard_state = nullptr;
        static int keyboard_state_size = 0;

        bool isKeyDown(int_t key) {
            if (keyboard_state == nullptr) {
                keyboard_state = SDL_GetKeyboardState(&keyboard_state_size);
                if (keyboard_state == nullptr)
                    throw SDLException();
            }

            SDL_Keycode sdl_key = toSDLKeycode(key);
            if (sdl_key == SDLK_UNKNOWN)
                return false;
            SDL_Scancode sc = SDL_GetScancodeFromKey(sdl_key, nullptr);
            if (sc == SDL_SCANCODE_UNKNOWN)
                return false;
            if (sc < 0 || sc >= keyboard_state_size)
                return false;
            return keyboard_state[sc] != 0;
        }
    }
}
