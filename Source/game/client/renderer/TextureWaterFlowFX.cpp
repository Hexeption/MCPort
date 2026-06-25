//
// Created by Keir Davis on 25/06/2026.
//

#include "TextureWaterFlowFX.h"

#include <cstdlib>

TextureWaterFlowFX::TextureWaterFlowFX() : TextureFX(12 * 16 + 14) {
    tileSize = 2;
}

void TextureWaterFlowFX::onTick() {
    ++tickCounter;

    for (int_t y = 0; y < 16; ++y) {
        for (int_t x = 0; x < 16; ++x) {
            float sum = 0.0f;
            for (int_t sampleY = y - 2; sampleY <= y; ++sampleY) {
                const int_t wrappedX = x & 15;
                const int_t wrappedY = sampleY & 15;
                sum += red[wrappedX + wrappedY * 16];
            }
            green[x + y * 16] = sum / 3.2f + blue[x + y * 16] * 0.8f;
        }
    }

    for (int_t y = 0; y < 16; ++y) {
        for (int_t x = 0; x < 16; ++x) {
            blue[x + y * 16] += alpha[x + y * 16] * 0.05f;
            if (blue[x + y * 16] < 0.0f) {
                blue[x + y * 16] = 0.0f;
            }

            alpha[x + y * 16] -= 0.3f;
            if (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX) < 0.2) {
                alpha[x + y * 16] = 0.5f;
            }
        }
    }

    for (int_t i = 0; i < 256; ++i) {
        red[i] = green[i];
        float value = red[(i - tickCounter * 16) & 255];
        if (value > 1.0f) {
            value = 1.0f;
        }
        if (value < 0.0f) {
            value = 0.0f;
        }

        const float squared = value * value;
        int_t r = static_cast<int_t>(32.0f + squared * 32.0f);
        int_t g = static_cast<int_t>(50.0f + squared * 64.0f);
        int_t b = 255;
        int_t a = static_cast<int_t>(146.0f + squared * 50.0f);
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
        imageData[i * 4 + 3] = static_cast<byte_t>(a);
    }
}
