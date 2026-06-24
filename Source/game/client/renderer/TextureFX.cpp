//
// Created by Keir Davis on 24/06/2026.
//

#include "TextureFX.h"

#include <glad/glad.h>

#include "RenderEngine.h"

TextureFX::TextureFX(const int_t iconIndex) : iconIndex(iconIndex) {
}

void TextureFX::onTick() {
}

void TextureFX::bindImage(RenderEngine &renderEngine) {
    if (tileImage == 0) {
        glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/terrain.png"));
    } else if (tileImage == 1) {
        glBindTexture(GL_TEXTURE_2D, renderEngine.getTexture(u"/gui/items.png"));
    }
}
