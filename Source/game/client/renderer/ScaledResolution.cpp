//
// Created by Keir Davis on 24/06/2026.
//

#include "ScaledResolution.h"

ScaledResolution::ScaledResolution(const int_t width, const int_t height) : scaleFactor(1), scaledWidth(width),
                                                                            scaledHeight(height) {
    while (scaledWidth / (scaleFactor + 1) >= 320 && scaledHeight / (scaleFactor + 1) >= 240) {
        ++scaleFactor;
    }

    scaledWidth /= scaleFactor;
    scaledHeight /= scaleFactor;
}

int_t ScaledResolution::getScaledWidth() const {
    return scaledWidth;
}

int_t ScaledResolution::getScaledHeight() const {
    return scaledHeight;
}
