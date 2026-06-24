//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_SCALEDRESOLUTION_H
#define MCPORT_SCALEDRESOLUTION_H

#include "java/Type.h"

class ScaledResolution {
public:
    int_t scaleFactor;

    ScaledResolution(int_t width, int_t height);

    int_t getScaledWidth() const;

    int_t getScaledHeight() const;

private:
    int_t scaledWidth;
    int_t scaledHeight;
};

#endif //MCPORT_SCALEDRESOLUTION_H
