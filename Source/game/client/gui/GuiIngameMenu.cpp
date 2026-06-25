//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiIngameMenu.h"

#include <cmath>
#include <memory>

#include "GuiMainMenu.h"
#include "GuiOptions.h"
#include "game/client/MathHelper.h"
#include "game/client/Minecraft.h"
#include "game/world/World.h"

void GuiIngameMenu::initGui() {
    updateCounter2 = 0;
    controlList.clear();
    controlList.push_back(std::make_unique<GuiButton>(1, width / 2 - 100, height / 4 + 48,
                                                      u"Save and quit to title"));
    if (mc->isMultiplayerWorld()) {
        controlList[0]->displayString = u"Disconnect";
    }
    controlList.push_back(std::make_unique<GuiButton>(4, width / 2 - 100, height / 4 + 24, u"Back to game"));
    controlList.push_back(std::make_unique<GuiButton>(0, width / 2 - 100, height / 4 + 96, u"Options..."));
}

void GuiIngameMenu::actionPerformed(GuiButton *button) {
    if (button->id == 0) {
        mc->displayGuiScreen(std::make_shared<GuiOptions>(shared_from_this(), mc->options.get()));
    }

    if (button->id == 1) {
        if (mc->isMultiplayerWorld() && mc->theWorld != nullptr) {
            mc->theWorld->sendQuittingDisconnectingPacket();
        }
        mc->changeWorld1(nullptr);
        mc->displayGuiScreen(std::make_shared<GuiMainMenu>());
    }

    if (button->id == 4) {
        mc->displayGuiScreen(nullptr);
        mc->setIngameFocus();
    }
}

void GuiIngameMenu::updateScreen() {
    GuiScreen::updateScreen();
    ++updateCounter;
}

void GuiIngameMenu::drawScreen(const int_t mouseX, const int_t mouseY, const float partialTicks) {
    drawDefaultBackground();
    const bool saving = mc->theWorld != nullptr && (!mc->theWorld->saveWorld(updateCounter2++) || updateCounter < 20);
    if (saving) {
        float alpha = (static_cast<float>(updateCounter % 10) + partialTicks) / 10.0f;
        alpha = MathHelper::sin(alpha * static_cast<float>(std::acos(-1.0)) * 2.0f) * 0.2f + 0.8f;
        const int_t color = static_cast<int_t>(255.0f * alpha);
        drawString(fontRenderer, u"Saving level..", 8, height - 16, color << 16 | color << 8 | color);
    }

    drawCenteredString(fontRenderer, u"Game menu", width / 2, 40, 16777215);
    GuiScreen::drawScreen(mouseX, mouseY, partialTicks);
}
