//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiYesNo.h"

#include <utility>

#include "GuiSmallButton.h"

GuiYesNo::GuiYesNo(std::shared_ptr<GuiScreen> parentScreen, jstring message1, jstring message2,
                   const int_t worldNumber) : parentScreen(std::move(parentScreen)), message1(std::move(message1)),
                                              message2(std::move(message2)), worldNumber(worldNumber) {
}

void GuiYesNo::initGui() {
    controlList.clear();
    controlList.push_back(std::make_unique<GuiSmallButton>(0, width / 2 - 155, height / 6 + 96, u"Yes"));
    controlList.push_back(std::make_unique<GuiSmallButton>(1, width / 2 + 5, height / 6 + 96, u"No"));
}

void GuiYesNo::actionPerformed(GuiButton *button) {
    parentScreen->deleteWorld(button->id == 0, worldNumber);
}

void GuiYesNo::keyTyped(const char_t ch, const int_t key) {
    if (key == 1) {
        parentScreen->deleteWorld(false, worldNumber);
        return;
    }

    GuiScreen::keyTyped(ch, key);
}

void GuiYesNo::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    drawCenteredString(fontRenderer, message1, width / 2, 70, 16777215);
    drawCenteredString(fontRenderer, message2, width / 2, 90, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}
