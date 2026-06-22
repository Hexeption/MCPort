//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_DISPLAY_H
#define MCPORT_DISPLAY_H
#include "java/String.h"
#include "java/Type.h"

namespace lwjgl {
    struct DisplayMode {
    private:
        int_t width = 0;
        int_t height = 0;
        int_t bpp = 0;
        int_t freq = 0;
        bool fullscreen = false;

        DisplayMode(int_t width, int_t height, int_t bpp, int_t freq, bool fullscreen)
            : width(width),
              height(height),
              bpp(bpp),
              freq(freq),
              fullscreen(fullscreen) {
        }

    public:
        DisplayMode(const DisplayMode &) = default;

        DisplayMode(DisplayMode &&) = default;

        DisplayMode &operator=(const DisplayMode &) = default;

        DisplayMode(int_t width, int_t height) : DisplayMode(width, height, 0, 0, false) {
        }

        DisplayMode(int_t width, int_t height, int_t bpp, int_t freq)
            : DisplayMode(width, height, bpp, freq, true) {
        }

        int_t getWidth() const {
            return width;
        }

        int_t getHeight() const {
            return height;
        }

        int_t getBitsPerPixel() const {
            return bpp;
        }

        int_t getFrequency() const {
            return freq;
        }

        int_t isFullscreen() const {
            return fullscreen;
        }
    };

    namespace Display {
        void setDisplayMode(const DisplayMode &mode);

        DisplayMode getDisplayMode();

        void setTitle(const jstring &title);

        void setFullscreen(bool fullscreen);

        bool isCloseRequested();

        bool isVisible();

        bool isActive();

        void processMessages();

        void swapBuffers();

        void update(bool doProcessEvents = true);

        void create();

        void Destory();

        int_t getX();

        int_t getY();

        int_t getWidth();

        int_t getHeight();
    }
}

#endif //MCPORT_DISPLAY_H
