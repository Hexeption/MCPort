//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiOptions.h"

#include <memory>

#include "GuiControls.h"
#include "GuiSmallButton.h"
#include "game/client/Minecraft.h"
#include "game/client/options/GameSettings.h"

GuiOptions::GuiOptions(std::shared_ptr<GuiScreen> parentScreen, GameSettings *options)
    : parentScreen(std::move(parentScreen)), options(options) {
}

void GuiOptions::initGui() {
    controlList.clear();

    for (int_t var1 = 0; var1 < options->numberOfOptions; ++var1) {
        controlList.push_back(std::make_unique<GuiSmallButton>(
            var1, width / 2 - 155 + var1 % 2 * 160, height / 6 + 24 * (var1 >> 1),
            options->getOptionDisplayString(var1)));
    }

    controlList.push_back(std::make_unique<GuiButton>(100, width / 2 - 100, height / 6 + 120 + 12, u"Controls..."));
    controlList.push_back(std::make_unique<GuiButton>(200, width / 2 - 100, height / 6 + 168, u"Done"));
}

void GuiOptions::actionPerformed(GuiButton *button) {
    if (button->enabled) {
        if (button->id < 100) {
            options->setOptionValue(button->id, 1);
            button->displayString = options->getOptionDisplayString(button->id);
        }

        if (button->id == 100) {
            mc->displayGuiScreen(std::make_shared<GuiControls>(shared_from_this(), options));
        }

        if (button->id == 200) {
            mc->displayGuiScreen(parentScreen);
        }
    }
}

void GuiOptions::keyTyped(const char_t ch, const int_t key) {
    if (key == 1) {
        mc->displayGuiScreen(parentScreen);
        return;
    }

    GuiScreen::keyTyped(ch, key);
}

void GuiOptions::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    drawCenteredString(fontRenderer, screenTitle, width / 2, 20, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}
