//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_TEXTURELAVAFLOWFX_H
#define MCPORT_TEXTURELAVAFLOWFX_H

#include "TextureFX.h"

class TextureLavaFlowFX : public TextureFX {
private:
    float red[256] = {};
    float green[256] = {};
    float blue[256] = {};
    float alpha[256] = {};
    int_t tickCounter = 0;

public:
    TextureLavaFlowFX();

    void onTick() override;
};

#endif //MCPORT_TEXTURELAVAFLOWFX_H
