//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUISMALLBUTTON_H
#define MCPORT_GUISMALLBUTTON_H

#include "GuiButton.h"

class GuiSmallButton : public GuiButton {
public:
    GuiSmallButton(int_t id, int_t x, int_t y, const jstring &text);
};

#endif //MCPORT_GUISMALLBUTTON_H
