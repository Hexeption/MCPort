//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_TEXTUREFX_H
#define MCPORT_TEXTUREFX_H

#include <vector>

#include "java/Type.h"

class RenderEngine;

class TextureFX {
public:
    std::vector<byte_t> imageData = std::vector<byte_t>(1024);
    int_t iconIndex;
    bool anaglyphEnabled = false;
    int_t textureId = 0;
    int_t tileSize = 1;
    int_t tileImage = 0;

    explicit TextureFX(int_t iconIndex);

    virtual ~TextureFX() = default;

    virtual void onTick();

    virtual void bindImage(RenderEngine &renderEngine);
};

#endif //MCPORT_TEXTUREFX_H
