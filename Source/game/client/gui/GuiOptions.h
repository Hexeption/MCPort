//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUIOPTIONS_H
#define MCPORT_GUIOPTIONS_H

#include <memory>

#include "GuiScreen.h"

class GameSettings;

class GuiOptions : public GuiScreen {
private:
    std::shared_ptr<GuiScreen> parentScreen;
    GameSettings *options;

protected:
    jstring screenTitle = u"Options";

    void actionPerformed(GuiButton *button) override;

public:
    GuiOptions(std::shared_ptr<GuiScreen> parentScreen, GameSettings *options);

    void initGui() override;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;
};

#endif //MCPORT_GUIOPTIONS_H
