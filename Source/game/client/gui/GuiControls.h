//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUICONTROLS_H
#define MCPORT_GUICONTROLS_H

#include <memory>

#include "GuiScreen.h"


class GuiControls : public GuiScreen {
private:
    std::shared_ptr<GuiScreen> parentScreen;
    GameSettings *options;
    int_t buttonId = -1;

protected:
    jstring screenTitle = u"Controls";

    void actionPerformed(GuiButton *button) override;

    void keyTyped(char_t ch, int_t key) override;

public:
    GuiControls(std::shared_ptr<GuiScreen> parentScreen, GameSettings *options);

    void initGui() override;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;
};

#endif //MCPORT_GUICONTROLS_H
