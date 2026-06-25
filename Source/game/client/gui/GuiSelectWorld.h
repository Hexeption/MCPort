//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_GUISELECTWORLD_H
#define MCPORT_GUISELECTWORLD_H

#include <memory>

#include "GuiScreen.h"

class GuiSelectWorld : public GuiScreen {
protected:
    std::shared_ptr<GuiScreen> parentScreen;
    jstring screenTitle = u"Select world";

    void actionPerformed(GuiButton *button) override;

public:
    bool selected = false;

    explicit GuiSelectWorld(std::shared_ptr<GuiScreen> parentScreen);

    void initGui() override;

    jstring getSaveName(int_t id);

    virtual void initButtons();

    virtual void selectWorld(int_t id);

    void drawScreen(int_t mouseX, int_t mouseY, float partialTicks) override;
};

#endif //MCPORT_GUISELECTWORLD_H
