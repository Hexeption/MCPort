//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CLIPPINGHELPERIMPLEMENTATION_H
#define MCPORT_CLIPPINGHELPERIMPLEMENTATION_H

#include "ClippingHelper.h"

#include "port/lwjgl/BufferUtils.h"

using FloatBuffer = lwjgl::FloatBuffer;

class ClippingHelperImplementation : public ClippingHelper {
private:
    static ClippingHelperImplementation instance;

    FloatBuffer projectionMatrixBuffer = FloatBuffer(16);
    FloatBuffer modelviewMatrixBuffer = FloatBuffer(16);
    FloatBuffer floatBuffer = FloatBuffer(16);

public:
    static ClippingHelper *getInstance();

private:
    static void normalize(float values[6][4], int plane);

    void init();
};

#endif //MCPORT_CLIPPINGHELPERIMPLEMENTATION_H
