//
// Created by Keir Davis on 24/06/2026.
//

#include "Gui.h"

#include <glad/glad.h>

#include "game/client/renderer/FontRenderer.h"
#include "game/client/renderer/Tessellator.h"

void Gui::drawRect(const int_t x1, const int_t y1, const int_t x2, const int_t y2, const int_t color) {
    const float alpha = static_cast<float>(color >> 24 & 255) / 255.0f;
    const float red = static_cast<float>(color >> 16 & 255) / 255.0f;
    const float green = static_cast<float>(color >> 8 & 255) / 255.0f;
    const float blue = static_cast<float>(color & 255) / 255.0f;
    Tessellator &tessellator = Tessellator::instance;
    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(red, green, blue, alpha);
    tessellator.startDrawingQuads();
    tessellator.addVertex(static_cast<double>(x1), static_cast<double>(y2), 0.0);
    tessellator.addVertex(static_cast<double>(x2), static_cast<double>(y2), 0.0);
    tessellator.addVertex(static_cast<double>(x2), static_cast<double>(y1), 0.0);
    tessellator.addVertex(static_cast<double>(x1), static_cast<double>(y1), 0.0);
    tessellator.draw();
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void Gui::drawGradientRect(const int_t x1, const int_t y1, const int_t x2, const int_t y2, const int_t color1,
                           const int_t color2) {
    const float alpha1 = static_cast<float>(color1 >> 24 & 255) / 255.0f;
    const float red1 = static_cast<float>(color1 >> 16 & 255) / 255.0f;
    const float green1 = static_cast<float>(color1 >> 8 & 255) / 255.0f;
    const float blue1 = static_cast<float>(color1 & 255) / 255.0f;
    const float alpha2 = static_cast<float>(color2 >> 24 & 255) / 255.0f;
    const float red2 = static_cast<float>(color2 >> 16 & 255) / 255.0f;
    const float green2 = static_cast<float>(color2 >> 8 & 255) / 255.0f;
    const float blue2 = static_cast<float>(color2 & 255) / 255.0f;
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    Tessellator &tessellator = Tessellator::instance;
    tessellator.startDrawingQuads();
    tessellator.setColorRGBA_F(red1, green1, blue1, alpha1);
    tessellator.addVertex(static_cast<double>(x2), static_cast<double>(y1), 0.0);
    tessellator.addVertex(static_cast<double>(x1), static_cast<double>(y1), 0.0);
    tessellator.setColorRGBA_F(red2, green2, blue2, alpha2);
    tessellator.addVertex(static_cast<double>(x1), static_cast<double>(y2), 0.0);
    tessellator.addVertex(static_cast<double>(x2), static_cast<double>(y2), 0.0);
    tessellator.draw();
    glShadeModel(GL_FLAT);
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
}

void Gui::drawCenteredString(FontRenderer *fontRenderer, const jstring &text, const int_t x, const int_t y,
                             const int_t color) {
    fontRenderer->drawStringWithShadow(text, x - fontRenderer->getStringWidth(text) / 2, y, color);
}

void Gui::drawString(FontRenderer *fontRenderer, const jstring &text, const int_t x, const int_t y,
                     const int_t color) {
    fontRenderer->drawStringWithShadow(text, x, y, color);
}

void Gui::drawTexturedModalRect(const int_t x, const int_t y, const int_t u, const int_t v, const int_t width,
                                const int_t height) {
    constexpr float var7 = 0.00390625f;
    constexpr float var8 = 0.00390625f;
    Tessellator &var9 = Tessellator::instance;
    var9.startDrawingQuads();
    var9.addVertexWithUV(static_cast<double>(x), static_cast<double>(y + height), static_cast<double>(zLevel),
                         static_cast<double>(static_cast<float>(u) * var7),
                         static_cast<double>(static_cast<float>(v + height) * var8));
    var9.addVertexWithUV(static_cast<double>(x + width), static_cast<double>(y + height), static_cast<double>(zLevel),
                         static_cast<double>(static_cast<float>(u + width) * var7),
                         static_cast<double>(static_cast<float>(v + height) * var8));
    var9.addVertexWithUV(static_cast<double>(x + width), static_cast<double>(y), static_cast<double>(zLevel),
                         static_cast<double>(static_cast<float>(u + width) * var7),
                         static_cast<double>(static_cast<float>(v) * var8));
    var9.addVertexWithUV(static_cast<double>(x), static_cast<double>(y), static_cast<double>(zLevel),
                         static_cast<double>(static_cast<float>(u) * var7),
                         static_cast<double>(static_cast<float>(v) * var8));
    var9.draw();
}
