//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ICAMERA_H
#define MCPORT_ICAMERA_H

class AxisAlignedBB;

class ICamera {
public:
    virtual ~ICamera() = default;

    virtual bool isBoundingBoxInFrustum(AxisAlignedBB &box) = 0;

    virtual void setPosition(double x, double y, double z) = 0;
};

#endif //MCPORT_ICAMERA_H
