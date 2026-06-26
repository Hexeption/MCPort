//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_FRUSTUM_H
#define MCPORT_FRUSTUM_H

#include "ICamera.h"

class ClippingHelper;
class AxisAlignedBB;

class Frustum : public ICamera {
private:
    ClippingHelper *clippingHelper = nullptr;
    double xPosition = 0.0;
    double yPosition = 0.0;
    double zPosition = 0.0;

public:
    Frustum();

    void setPosition(double x, double y, double z) override;

    bool isBoxInFrustum(double x0, double y0, double z0, double x1, double y1, double z1);

    bool isBoundingBoxInFrustum(AxisAlignedBB &box) override;
};

#endif //MCPORT_FRUSTUM_H
