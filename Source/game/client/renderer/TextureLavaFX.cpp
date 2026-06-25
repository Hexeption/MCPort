//
// Created by Keir Davis on 25/06/2026.
//

#include "TextureLavaFX.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "game/client/MathHelper.h"

TextureLavaFX::TextureLavaFX() : TextureFX(14 * 16 + 13) {
}

void TextureLavaFX::onTick() {
    for (int_t x = 0; x < 16; ++x) {
        for (int_t y = 0; y < 16; ++y) {
            float sum = 0.0f;
            const int_t xOffset = static_cast<int_t>(MathHelper::sin(static_cast<float>(y) *
                static_cast<float>(std::acos(-1.0)) * 2.0f / 16.0f) * 1.2f);
            const int_t yOffset = static_cast<int_t>(MathHelper::sin(static_cast<float>(x) *
                static_cast<float>(std::acos(-1.0)) * 2.0f / 16.0f) * 1.2f);

            for (int_t sampleX = x - 1; sampleX <= x + 1; ++sampleX) {
                for (int_t sampleY = y - 1; sampleY <= y + 1; ++sampleY) {
                    const int_t wrappedX = sampleX + xOffset & 15;
                    const int_t wrappedY = sampleY + yOffset & 15;
                    sum += red[wrappedX + wrappedY * 16];
                }
            }

            green[x + y * 16] = sum / 10.0f +
                                (blue[(x + 0 & 15) + (y + 0 & 15) * 16] +
                                 blue[(x + 1 & 15) + (y + 0 & 15) * 16] +
                                 blue[(x + 1 & 15) + (y + 1 & 15) * 16] +
                                 blue[(x + 0 & 15) + (y + 1 & 15) * 16]) / 4.0f * 0.8f;
            blue[x + y * 16] += alpha[x + y * 16] * 0.01f;
            if (blue[x + y * 16] < 0.0f) {
                blue[x + y * 16] = 0.0f;
            }

            alpha[x + y * 16] -= 0.06f;
            if (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX) < 0.005) {
                alpha[x + y * 16] = 1.5f;
            }
        }
    }

    for (int_t i = 0; i < 256; ++i) {
        red[i] = green[i];
        float value = red[i] * 2.0f;
        value = std::clamp(value, 0.0f, 1.0f);

        int_t r = static_cast<int_t>(value * 100.0f + 155.0f);
        int_t g = static_cast<int_t>(value * value * 255.0f);
        int_t b = static_cast<int_t>(value * value * value * value * 128.0f);
        if (anaglyphEnabled) {
            const int_t anaglyphRed = (r * 30 + g * 59 + b * 11) / 100;
            const int_t anaglyphGreen = (r * 30 + g * 70) / 100;
            const int_t anaglyphBlue = (r * 30 + b * 70) / 100;
            r = anaglyphRed;
            g = anaglyphGreen;
            b = anaglyphBlue;
        }

        imageData[i * 4 + 0] = static_cast<byte_t>(r);
        imageData[i * 4 + 1] = static_cast<byte_t>(g);
        imageData[i * 4 + 2] = static_cast<byte_t>(b);
        imageData[i * 4 + 3] = static_cast<byte_t>(255);
    }
}
