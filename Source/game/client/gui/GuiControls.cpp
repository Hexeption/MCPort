//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiControls.h"

#include <memory>

#include "GuiSmallButton.h"
#include "game/client/Minecraft.h"
#include "game/client/options/GameSettings.h"

GuiControls::GuiControls(std::shared_ptr<GuiScreen> parentScreen, GameSettings *options)
    : parentScreen(std::move(parentScreen)), options(options) {
}

void GuiControls::initGui() {
    controlList.clear();

    for (int_t var1 = 0; var1 < static_cast<int_t>(options->keyBindings.size()); ++var1) {
        controlList.push_back(std::make_unique<GuiSmallButton>(
            var1, width / 2 - 155 + var1 % 2 * 160, height / 6 + 24 * (var1 >> 1),
            options->getKeybindingDescription(var1)));
    }

    controlList.push_back(std::make_unique<GuiButton>(200, width / 2 - 100, height / 6 + 168, u"Done"));
}

void GuiControls::actionPerformed(GuiButton *button) {
    for (int_t var2 = 0; var2 < static_cast<int_t>(options->keyBindings.size()); ++var2) {
        controlList[var2]->displayString = options->getKeybindingDescription(var2);
    }

    if (button->id == 200) {
        mc->displayGuiScreen(parentScreen);
    } else {
        buttonId = button->id;
        button->displayString = u"> " + options->getKeybindingDescription(button->id) + u" <";
    }
}

void GuiControls::keyTyped(const char_t ch, const int_t key) {
    if (buttonId >= 0) {
        options->setKeybinding(buttonId, key);
        controlList[buttonId]->displayString = options->getKeybindingDescription(buttonId);
        buttonId = -1;
    } else {
        GuiScreen::keyTyped(ch, key);
    }
}

void GuiControls::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    drawCenteredString(fontRenderer, screenTitle, width / 2, 20, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}
