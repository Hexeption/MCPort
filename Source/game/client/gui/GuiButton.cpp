//
// Created by Keir Davis on 24/06/2026.
//

#include "GuiButton.h"

#include <glad/glad.h>

#include "game/client/Minecraft.h"

GuiButton::GuiButton(const int_t id, const int_t x, const int_t y, const jstring &text)
    : GuiButton(id, x, y, 200, 20, text) {
}

GuiButton::GuiButton(const int_t id, const int_t x, const int_t y, const int_t width, const int_t height,
                     const jstring &text)
    : width(width), height(height), xPosition(x), yPosition(y), displayString(text), id(id), enabled(true),
      visible(true) {
}

void GuiButton::drawButton(Minecraft *minecraft, const int_t mouseX, const int_t mouseY) {
    if (visible) {
        FontRenderer *var4 = minecraft->fontRenderer.get();
        glBindTexture(GL_TEXTURE_2D, minecraft->renderEngine->getTexture(u"/gui/gui.png"));
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        byte_t var5 = 1;
        const bool var6 = mouseX >= xPosition && mouseY >= yPosition && mouseX < xPosition + width &&
                          mouseY < yPosition + height;
        if (!enabled) {
            var5 = 0;
        } else if (var6) {
            var5 = 2;
        }

        drawTexturedModalRect(xPosition, yPosition, 0, 46 + var5 * 20, width / 2, height);
        drawTexturedModalRect(xPosition + width / 2, yPosition, 200 - width / 2, 46 + var5 * 20, width / 2, height);
        if (!enabled) {
            drawCenteredString(var4, displayString, xPosition + width / 2, yPosition + (height - 8) / 2, -6250336);
        } else if (var6) {
            drawCenteredString(var4, displayString, xPosition + width / 2, yPosition + (height - 8) / 2, 16777120);
        } else {
            drawCenteredString(var4, displayString, xPosition + width / 2, yPosition + (height - 8) / 2, 14737632);
        }
    }
}

bool GuiButton::mousePressed(const int_t mouseX, const int_t mouseY) const {
    return enabled && mouseX >= xPosition && mouseY >= yPosition && mouseX < xPosition + width &&
           mouseY < yPosition + height;
}
