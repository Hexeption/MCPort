//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUIINGAMEMENU_H
#define MCPORT_GUIINGAMEMENU_H

#include "GuiScreen.h"

class GuiIngameMenu : public GuiScreen {
private:
    int_t updateCounter2 = 0;
    int_t updateCounter = 0;

protected:
    void actionPerformed(GuiButton *button) override;

public:
    void initGui() override;

    void updateScreen() override;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;
};

#endif //MCPORT_GUIINGAMEMENU_H
