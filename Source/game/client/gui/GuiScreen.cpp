//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiScreen.h"

#include <glad/glad.h>

#include "GuiButton.h"
#include "GuiMainMenu.h"
#include "game/client/Minecraft.h"
#include "game/client/renderer/Tessellator.h"
#include "lwjgl/Keyboard.h"
#include "lwjgl/Mouse.h"

void GuiScreen::drawScreen(const int_t mouseX, const int_t mouseY, const float) {
    for (auto &var5: controlList) {
        var5->drawButton(mc, mouseX, mouseY);
    }
}

void GuiScreen::keyTyped(char_t, const int_t key) {
    if (key == 1) {
        if (mc->theWorld != nullptr) {
            mc->displayGuiScreen(nullptr);
            mc->setIngameFocus();
        } else {
            mc->displayGuiScreen(std::make_shared<GuiMainMenu>());
        }
    }
}

jstring GuiScreen::getClipboardString() {
    // todo: We going to need to handle all os clipboards sub for now
    return u"";
}

void GuiScreen::mouseClicked(const int_t mouseX, const int_t mouseY, const int_t mouseButton) {
    if (mouseButton == 0) {
        for (auto &var5: controlList) {
            if (var5->mousePressed(mouseX, mouseY)) {
                actionPerformed(var5.get());
            }
        }
    }
}

void GuiScreen::mouseMovedOrUp(int_t, int_t, int_t) {
}

void GuiScreen::actionPerformed(GuiButton *) {
}

void GuiScreen::setWorldAndResolution(Minecraft *minecraft, const int_t screenWidth, const int_t screenHeight) {
    mc = minecraft;
    fontRenderer = minecraft->fontRenderer.get();
    width = screenWidth;
    height = screenHeight;

    printf("Screen size: %d x %d\n", width, height);

    initGui();
}

void GuiScreen::initGui() {
}

void GuiScreen::handleInput() {
    while (lwjgl::Mouse::next()) {
        handleMouseInput();
    }

    while (lwjgl::Keyboard::next()) {
        handleKeyboardInput();
    }
}

void GuiScreen::handleMouseInput() {
    int_t mouseX;
    int_t mouseY;
    if (lwjgl::Mouse::getEventButtonState()) {
        mouseX = lwjgl::Mouse::getEventX() * width / mc->displayWidth;
        mouseY = height - lwjgl::Mouse::getEventY() * height / mc->displayHeight - 1;
        mouseClicked(mouseX, mouseY, lwjgl::Mouse::getEventButton());
    } else {
        mouseX = lwjgl::Mouse::getEventX() * width / mc->displayWidth;
        mouseY = height - lwjgl::Mouse::getEventY() * height / mc->displayHeight - 1;
        mouseMovedOrUp(mouseX, mouseY, lwjgl::Mouse::getEventButton());
    }
}

void GuiScreen::handleKeyboardInput() {
    if (lwjgl::Keyboard::getEventKeyState()) {
        if (lwjgl::Keyboard::getEventKey() == lwjgl::Keyboard::KEY_F11) {
            // todo: full screen toggle
        }

        keyTyped(lwjgl::Keyboard::getEventCharacter(), lwjgl::Keyboard::getEventKey());
    }
}

void GuiScreen::updateScreen() {
}

void GuiScreen::onGuiClosed() {
}

void GuiScreen::drawDefaultBackground() {
    drawWorldBackground(0);
}

void GuiScreen::drawWorldBackground(const int_t scroll) {
    if (mc->theWorld != nullptr) {
        drawGradientRect(0, 0, width, height, -1072689136, -804253680);
    } else {
        drawBackground(scroll);
    }
}

void GuiScreen::drawBackground(const int_t scroll) {
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    Tessellator &var2 = Tessellator::instance;
    glBindTexture(GL_TEXTURE_2D, mc->renderEngine->getTexture(u"/dirt.png"));
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    constexpr float var3 = 32.0f;
    var2.startDrawingQuads();
    var2.setColorOpaque_I(4210752);
    var2.addVertexWithUV(0.0, static_cast<double>(height), 0.0, 0.0,
                         static_cast<double>(static_cast<float>(height) / var3 + static_cast<float>(scroll)));
    var2.addVertexWithUV(static_cast<double>(width), static_cast<double>(height), 0.0,
                         static_cast<double>(static_cast<float>(width) / var3),
                         static_cast<double>(static_cast<float>(height) / var3 + static_cast<float>(scroll)));
    var2.addVertexWithUV(static_cast<double>(width), 0.0, 0.0,
                         static_cast<double>(static_cast<float>(width) / var3),
                         static_cast<double>(scroll));
    var2.addVertexWithUV(0.0, 0.0, 0.0, 0.0, static_cast<double>(scroll));
    var2.draw();
}

bool GuiScreen::doesGuiPauseGame() {
    return true;
}

void GuiScreen::deleteWorld(bool, int_t) {
}
