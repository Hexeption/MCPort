//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_FONTRENDERER_H
#define MCPORT_FONTRENDERER_H

#include <array>
#include <filesystem>
#include <vector>

#include "java/BufferedImage.h"
#include "java/String.h"
#include "java/Type.h"

#include "game/client/options/GameSettings.h"
#include "game/client/renderer/RenderEngine.h"

class FontRenderer {
public:
    int_t fontTextureName = 0;

    FontRenderer(GameSettings *options, const jstring &fontTexturePath, RenderEngine &renderEngine);

    ~FontRenderer();

    void drawStringWithShadow(const jstring &text, int_t x, int_t y, int_t color);

    void drawString(const jstring &text, int_t x, int_t y, int_t color);

    void renderString(const jstring &text, int_t x, int_t y, int_t color, bool shadow);

    int_t getStringWidth(const jstring &text) const;

private:
    std::array<int_t, 256> charWidth{};
    int_t fontDisplayLists = 0;
    std::vector<unsigned int> buffer;

    static const jstring &fontAllowedCharacters();

    static bool assetExists(const std::filesystem::path &path);

    static std::string stripLeadingSlash(const std::string &path);

    static BufferedImage loadImageResource(const jstring &resourcePath);

    static int_t pixelArgb(const BufferedImage &image, int_t x, int_t y);
};

#endif //MCPORT_FONTRENDERER_H
