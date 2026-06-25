//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_TEXTUREWATERFX_H
#define MCPORT_TEXTUREWATERFX_H

#include "TextureFX.h"

class TextureWaterFX : public TextureFX {
private:
    float red[256] = {};
    float green[256] = {};
    float blue[256] = {};
    float alpha[256] = {};
    int_t tickCounter = 0;

public:
    TextureWaterFX();

    void onTick() override;
};

#endif //MCPORT_TEXTUREWATERFX_H
