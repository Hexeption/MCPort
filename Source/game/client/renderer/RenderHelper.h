//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_RENDERHELPER_H
#define MCPORT_RENDERHELPER_H

#include <glad/glad.h>

#ifndef GL_RESCALE_NORMAL
#define GL_RESCALE_NORMAL 0x803A
#endif

class RenderHelper {
public:
    static void disableStandardItemLighting();

    static void enableStandardItemLighting();

private:
    static const float *setColorBuffer(float red, float green, float blue, float alpha);
};

#endif //MCPORT_RENDERHELPER_H
