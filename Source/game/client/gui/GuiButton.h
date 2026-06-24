//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUIBUTTON_H
#define MCPORT_GUIBUTTON_H

#include "Gui.h"

class Minecraft;

class GuiButton : public Gui {
private:
    int_t width;
    int_t height;

public:
    int_t xPosition;
    int_t yPosition;
    jstring displayString;
    int_t id;
    bool enabled;
    bool visible;

    GuiButton(int_t id, int_t x, int_t y, const jstring &text);

    GuiButton(int_t id, int_t x, int_t y, int_t width, int_t height, const jstring &text);

    void drawButton(Minecraft *minecraft, int_t mouseX, int_t mouseY);

    bool mousePressed(int_t mouseX, int_t mouseY) const;
};

#endif //MCPORT_GUIBUTTON_H
