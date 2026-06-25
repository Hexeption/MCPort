//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_TEXTURELAVAFX_H
#define MCPORT_TEXTURELAVAFX_H

#include "TextureFX.h"

class TextureLavaFX : public TextureFX {
private:
    float red[256] = {};
    float green[256] = {};
    float blue[256] = {};
    float alpha[256] = {};

public:
    TextureLavaFX();

    void onTick() override;
};

#endif //MCPORT_TEXTURELAVAFX_H
