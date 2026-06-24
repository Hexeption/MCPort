//
// Created by Keir Davis on 24/06/2026.
//

#include "LogoEffectRandomizer.h"

#include "GuiMainMenu.h"

LogoEffectRandomizer::LogoEffectRandomizer(GuiMainMenu *mainMenu, const int_t x, const int_t y)
    : height(static_cast<double>(10 + y) + GuiMainMenu::getRandom().nextDouble() * 32.0 + static_cast<double>(x)),
      prevHeight(height), mainMenu(mainMenu) {
}

void LogoEffectRandomizer::updateLogoEffects() {
    prevHeight = height;
    if (height > 0.0) {
        dropSpeed -= 0.6;
    }

    height += dropSpeed;
    dropSpeed *= 0.9;
    if (height < 0.0) {
        height = 0.0;
        dropSpeed = 0.0;
    }
}
