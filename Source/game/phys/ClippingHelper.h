//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CLIPPINGHELPER_H
#define MCPORT_CLIPPINGHELPER_H

class ClippingHelper {
public:
    float frustum[6][4]{};
    float projectionMatrix[16]{};
    float modelviewMatrix[16]{};
    float clippingMatrix[16]{};

    bool isBoxInFrustum(double x0, double y0, double z0, double x1, double y1, double z1);
};

#endif //MCPORT_CLIPPINGHELPER_H
