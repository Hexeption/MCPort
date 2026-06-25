//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_TEXTUREWATERFLOWFX_H
#define MCPORT_TEXTUREWATERFLOWFX_H

#include "TextureFX.h"

class TextureWaterFlowFX : public TextureFX {
private:
    float red[256] = {};
    float green[256] = {};
    float blue[256] = {};
    float alpha[256] = {};
    int_t tickCounter = 0;

public:
    TextureWaterFlowFX();

    void onTick() override;
};

#endif //MCPORT_TEXTUREWATERFLOWFX_H
