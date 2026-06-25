//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUIYESNO_H
#define MCPORT_GUIYESNO_H

#include <memory>

#include "GuiScreen.h"

class GuiYesNo : public GuiScreen {
private:
    std::shared_ptr<GuiScreen> parentScreen;
    jstring message1;
    jstring message2;
    int_t worldNumber;

protected:
    void actionPerformed(GuiButton *button) override;

public:
    GuiYesNo(std::shared_ptr<GuiScreen> parentScreen, jstring message1, jstring message2, int_t worldNumber);

    void initGui() override;

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;
};

#endif //MCPORT_GUIYESNO_H
