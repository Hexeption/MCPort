//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUI_H
#define MCPORT_GUI_H

#include "java/String.h"
#include "java/Type.h"

class FontRenderer;

class Gui {
protected:
    float zLevel = 0.0f;

    void drawRect(int_t x1, int_t y1, int_t x2, int_t y2, int_t color);

    void drawGradientRect(int_t x1, int_t y1, int_t x2, int_t y2, int_t color1, int_t color2);

public:
    void drawCenteredString(FontRenderer *fontRenderer, const jstring &text, int_t x, int_t y, int_t color);

    void drawString(FontRenderer *fontRenderer, const jstring &text, int_t x, int_t y, int_t color);

    void drawTexturedModalRect(int_t x, int_t y, int_t u, int_t v, int_t width, int_t height);
};

#endif //MCPORT_GUI_H
