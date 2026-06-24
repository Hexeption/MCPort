//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_LOGOEFFECTRANDOMIZER_H
#define MCPORT_LOGOEFFECTRANDOMIZER_H

#include "java/Type.h"

class GuiMainMenu;

class LogoEffectRandomizer {
public:
    double height;
    double prevHeight;
    double dropSpeed = 0.0;
    GuiMainMenu *mainMenu;

    LogoEffectRandomizer(GuiMainMenu *mainMenu, int_t x, int_t y);

    void updateLogoEffects();
};

#endif //MCPORT_LOGOEFFECTRANDOMIZER_H
