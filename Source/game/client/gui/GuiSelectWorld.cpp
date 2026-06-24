//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiSelectWorld.h"

#include <memory>

#include "game/client/Minecraft.h"

GuiSelectWorld::GuiSelectWorld(std::shared_ptr<GuiScreen> parentScreen) : parentScreen(std::move(parentScreen)) {
}

void GuiSelectWorld::initGui() {
    controlList.clear();

    for (int_t var2 = 0; var2 < 5; ++var2) {
        controlList.push_back(std::make_unique<GuiButton>(var2, width / 2 - 100, height / 6 + 24 * var2, u"- empty -"));
    }

    initButtons();
}

jstring GuiSelectWorld::getSaveName(const int_t) {
    return u"";
}

void GuiSelectWorld::initButtons() {
    controlList.push_back(std::make_unique<GuiButton>(5, width / 2 - 100, height / 6 + 120 + 12, u"Delete world..."));
    controlList.push_back(std::make_unique<GuiButton>(6, width / 2 - 100, height / 6 + 168, u"Cancel"));
}

void GuiSelectWorld::actionPerformed(GuiButton *button) {
    if (button->enabled) {
        if (button->id < 5) {
            selectWorld(button->id + 1);
        } else if (button->id == 5) {
            return;
        } else if (button->id == 6) {
            mc->displayGuiScreen(parentScreen);
        }
    }
}

void GuiSelectWorld::selectWorld(const int_t) {
    mc->displayGuiScreen(nullptr);
    if (!selected) {
        selected = true;
        mc->displayGuiScreen(nullptr);
    }
}

void GuiSelectWorld::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    drawCenteredString(fontRenderer, screenTitle, width / 2, 20, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}
