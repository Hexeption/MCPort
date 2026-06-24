//
// Created by Keir Davis on 24/06/2026.
//

#include "FontRenderer.h"

#include <array>
#include <filesystem>
#include <stdexcept>

#include <glad/glad.h>

#include "RenderEngine.h"
#include "Tessellator.h"
#include "game/client/options/GameSettings.h"
#include "java/BufferedImage.h"

FontRenderer::FontRenderer(GameSettings *options, const jstring &fontTexturePath,
                           RenderEngine &renderEngine) : buffer(1024) {
    const BufferedImage image = loadImageResource(fontTexturePath);
    const int_t width = image.getWidth();
    const int_t height = image.getHeight();
    std::vector<int_t> pixels(static_cast<std::size_t>(width) * height);
    for (int_t y = 0; y < height; ++y) {
        for (int_t x = 0; x < width; ++x) {
            pixels[static_cast<std::size_t>(x) + static_cast<std::size_t>(y) * width] = pixelArgb(image, x, y);
        }
    }

    for (int_t i = 0; i < 256; ++i) {
        const int_t glyphX = i % 16;
        const int_t glyphY = i / 16;

        int_t widthScan;
        for (widthScan = 7; widthScan >= 0; --widthScan) {
            const int_t x = glyphX * 8 + widthScan;
            bool emptyColumn = true;

            for (int_t y = 0; y < 8 && emptyColumn; ++y) {
                const int_t offset = (glyphY * 8 + y) * width;
                const int_t blue = pixels[static_cast<std::size_t>(x) + offset] & 255;
                if (blue > 0) {
                    emptyColumn = false;
                }
            }

            if (!emptyColumn) {
                break;
            }
        }

        if (i == 32) {
            widthScan = 2;
        }

        charWidth[static_cast<std::size_t>(i)] = widthScan + 2;
    }

    fontTextureName = renderEngine.allocateAndSetupTexture(image);
    fontDisplayLists = static_cast<int_t>(glGenLists(288));
    Tessellator &tessellator = Tessellator::instance;

    for (int_t i = 0; i < 256; ++i) {
        glNewList(static_cast<GLuint>(fontDisplayLists + i), GL_COMPILE);
        tessellator.startDrawingQuads();
        const int_t glyphU = i % 16 * 8;
        const int_t glyphV = i / 16 * 8;
        constexpr float glyphSize = 7.99f;
        constexpr float uOffset = 0.0f;
        constexpr float vOffset = 0.0f;
        tessellator.addVertexWithUV(0.0, static_cast<double>(0.0f + glyphSize), 0.0,
                                    static_cast<double>(static_cast<float>(glyphU) / 128.0f + uOffset),
                                    static_cast<double>((static_cast<float>(glyphV) + glyphSize) / 128.0f + vOffset));
        tessellator.addVertexWithUV(static_cast<double>(0.0f + glyphSize), static_cast<double>(0.0f + glyphSize), 0.0,
                                    static_cast<double>((static_cast<float>(glyphU) + glyphSize) / 128.0f + uOffset),
                                    static_cast<double>((static_cast<float>(glyphV) + glyphSize) / 128.0f + vOffset));
        tessellator.addVertexWithUV(static_cast<double>(0.0f + glyphSize), 0.0, 0.0,
                                    static_cast<double>((static_cast<float>(glyphU) + glyphSize) / 128.0f + uOffset),
                                    static_cast<double>(static_cast<float>(glyphV) / 128.0f + vOffset));
        tessellator.addVertexWithUV(0.0, 0.0, 0.0,
                                    static_cast<double>(static_cast<float>(glyphU) / 128.0f + uOffset),
                                    static_cast<double>(static_cast<float>(glyphV) / 128.0f + vOffset));
        tessellator.draw();
        glTranslatef(static_cast<float>(charWidth[static_cast<std::size_t>(i)]), 0.0f, 0.0f);
        glEndList();
    }

    for (int_t i = 0; i < 32; ++i) {
        const int_t base = (i >> 3 & 1) * 85;
        int_t red = (i >> 2 & 1) * 170 + base;
        int_t green = (i >> 1 & 1) * 170 + base;
        int_t blue = (i >> 0 & 1) * 170 + base;
        if (i == 6) {
            red += 85;
        }

        const bool dark = i >= 16;
        if (options != nullptr && options->anaglyph) {
            const int_t anaglyphRed = (red * 30 + green * 59 + blue * 11) / 100;
            const int_t anaglyphGreen = (red * 30 + green * 70) / 100;
            const int_t anaglyphBlue = (red * 30 + blue * 70) / 100;
            red = anaglyphRed;
            green = anaglyphGreen;
            blue = anaglyphBlue;
        }

        if (dark) {
            red /= 4;
            green /= 4;
            blue /= 4;
        }

        glNewList(static_cast<GLuint>(fontDisplayLists + 256 + i), GL_COMPILE);
        glColor3f(static_cast<float>(red) / 255.0f, static_cast<float>(green) / 255.0f,
                  static_cast<float>(blue) / 255.0f);
        glEndList();
    }
}

FontRenderer::~FontRenderer() {
    if (fontDisplayLists != 0) {
        glDeleteLists(static_cast<GLuint>(fontDisplayLists), 288);
    }
}

void FontRenderer::drawStringWithShadow(const jstring &text, const int_t x, const int_t y, const int_t color) {
    renderString(text, x + 1, y + 1, color, true);
    drawString(text, x, y, color);
}

void FontRenderer::drawString(const jstring &text, const int_t x, const int_t y, const int_t color) {
    renderString(text, x, y, color, false);
}

void FontRenderer::renderString(const jstring &text, const int_t x, const int_t y, int_t color, const bool shadow) {
    if (text.empty()) {
        return;
    }

    if (shadow) {
        const int_t alpha = color & static_cast<int_t>(0xFF000000);
        color = (color & 0x00FCFCFC) >> 2;
        color += alpha;
    }

    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(fontTextureName));
    const float red = static_cast<float>(color >> 16 & 255) / 255.0f;
    const float green = static_cast<float>(color >> 8 & 255) / 255.0f;
    const float blue = static_cast<float>(color & 255) / 255.0f;
    float alpha = static_cast<float>(color >> 24 & 255) / 255.0f;
    if (alpha == 0.0f) {
        alpha = 1.0f;
    }

    glColor4f(red, green, blue, alpha);
    std::size_t bufferPosition = 0;
    glPushMatrix();
    glTranslatef(static_cast<float>(x), static_cast<float>(y), 0.0f);

    for (std::size_t i = 0; i < text.length(); ++i) {
        int_t displayListIndex;
        for (; i < text.length() && text[i] == u'\u00A7' && text.length() > i + 1; i += 2) {
            displayListIndex = String::indexOf(u"0123456789abcdef", String::lowerCase(jstring(1, text[i + 1]))[0]);
            if (displayListIndex < 0 || displayListIndex > 15) {
                displayListIndex = 15;
            }

            buffer[bufferPosition++] = static_cast<unsigned int>(fontDisplayLists + 256 + displayListIndex +
                                                                 (shadow ? 16 : 0));
            if (bufferPosition == buffer.size()) {
                glCallLists(static_cast<GLsizei>(bufferPosition), GL_UNSIGNED_INT, buffer.data());
                bufferPosition = 0;
            }
        }

        if (i >= text.length()) {
            break;
        }

        displayListIndex = String::indexOf(fontAllowedCharacters(), text[i]);
        if (displayListIndex >= 0) {
            buffer[bufferPosition++] = static_cast<unsigned int>(fontDisplayLists + displayListIndex + 32);
        }

        if (bufferPosition == buffer.size()) {
            glCallLists(static_cast<GLsizei>(bufferPosition), GL_UNSIGNED_INT, buffer.data());
            bufferPosition = 0;
        }
    }

    if (bufferPosition > 0) {
        glCallLists(static_cast<GLsizei>(bufferPosition), GL_UNSIGNED_INT, buffer.data());
    }
    glPopMatrix();
}

int_t FontRenderer::getStringWidth(const jstring &text) const {
    if (text.empty()) {
        return 0;
    }

    int_t width = 0;
    for (std::size_t i = 0; i < text.length(); ++i) {
        if (text[i] == u'\u00A7') {
            ++i;
        } else {
            const int_t charIndex = String::indexOf(fontAllowedCharacters(), text[i]);
            if (charIndex >= 0) {
                width += charWidth[static_cast<std::size_t>(charIndex + 32)];
            }
        }
    }

    return width;
}

const jstring &FontRenderer::fontAllowedCharacters() {
    static const jstring characters =
            u" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~"
            u"\u2302\u00C7\u00FC\u00E9\u00E2\u00E4\u00E0\u00E5\u00E7\u00EA\u00EB\u00E8\u00EF\u00EE\u00EC"
            u"\u00C4\u00C5\u00C9\u00E6\u00C6\u00F4\u00F6\u00F2\u00FB\u00F9\u00FF\u00D6\u00DC\u00F8"
            u"\u00A3\u00D8\u00D7\u0192\u00E1\u00ED\u00F3\u00FA\u00F1\u00D1\u00AA\u00BA\u00BF\u00AE"
            u"\u00AC\u00BD\u00BC\u00A1\u00AB\u00BB";
    return characters;
}

bool FontRenderer::assetExists(const std::filesystem::path &path) {
    return std::filesystem::is_regular_file(path);
}

std::string FontRenderer::stripLeadingSlash(const std::string &path) {
    if (!path.empty() && (path[0] == '/' || path[0] == '\\')) {
        return path.substr(1);
    }
    return path;
}

BufferedImage FontRenderer::loadImageResource(const jstring &resourcePath) {
    const std::string raw = String::toUtf8(resourcePath);
    const std::string stripped = stripLeadingSlash(raw);
    const std::array<std::filesystem::path, 5> candidates = {
        std::filesystem::path(raw),
        std::filesystem::path(stripped),
        std::filesystem::path("Assets") / stripped,
        std::filesystem::path("..") / "Assets" / stripped,
        std::filesystem::path("..") / ".." / "Assets" / stripped,
    };

    for (const std::filesystem::path &candidate: candidates) {
        if (assetExists(candidate)) {
            return BufferedImage::readFile(String::fromUtf8(candidate.string()));
        }
    }

    throw std::runtime_error("font resource not found: " + raw);
}

int_t FontRenderer::pixelArgb(const BufferedImage &image, const int_t x, const int_t y) {
    const std::vector<ubyte_t> &pixels = image.getRgbaPixels();
    const std::size_t offset = (static_cast<std::size_t>(x) + static_cast<std::size_t>(y) * image.getWidth()) * 4;
    return static_cast<int_t>(pixels[offset + 3]) << 24
           | static_cast<int_t>(pixels[offset + 0]) << 16
           | static_cast<int_t>(pixels[offset + 1]) << 8
           | static_cast<int_t>(pixels[offset + 2]);
}
