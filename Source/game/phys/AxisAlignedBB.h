//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_AXISALIGNEDBB_H
#define MCPORT_AXISALIGNEDBB_H

class AxisAlignedBB {
public:
    double minX = 0.0;
    double minY = 0.0;
    double minZ = 0.0;
    double maxX = 0.0;
    double maxY = 0.0;
    double maxZ = 0.0;

    AxisAlignedBB() = default;

    AxisAlignedBB(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);

    static AxisAlignedBB getBoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);

    AxisAlignedBB &setBounds(double newMinX, double newMinY, double newMinZ, double newMaxX, double newMaxY,
                             double newMaxZ);

    AxisAlignedBB addCoord(double x, double y, double z) const;

    AxisAlignedBB expand(double x, double y, double z) const;

    AxisAlignedBB getOffsetBoundingBox(double x, double y, double z) const;

    double calculateXOffset(const AxisAlignedBB &other, double offset) const;

    double calculateYOffset(const AxisAlignedBB &other, double offset) const;

    double calculateZOffset(const AxisAlignedBB &other, double offset) const;

    bool intersectsWith(const AxisAlignedBB &other) const;

    AxisAlignedBB &offset(double x, double y, double z);

    AxisAlignedBB copy() const;
};

#endif //MCPORT_AXISALIGNEDBB_H
