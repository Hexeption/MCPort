//
// Created by Keir Davis on 25/06/2026.
//

#include "AxisAlignedBB.h"

#include <algorithm>

#include "game/util/Vec3D.h"

AxisAlignedBB::AxisAlignedBB(const double minX, const double minY, const double minZ, const double maxX,
                             const double maxY, const double maxZ)
    : minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {
}

AxisAlignedBB AxisAlignedBB::getBoundingBox(const double minX, const double minY, const double minZ,
                                            const double maxX, const double maxY, const double maxZ) {
    return AxisAlignedBB(minX, minY, minZ, maxX, maxY, maxZ);
}

AxisAlignedBB &AxisAlignedBB::setBounds(const double newMinX, const double newMinY, const double newMinZ,
                                        const double newMaxX, const double newMaxY, const double newMaxZ) {
    minX = newMinX;
    minY = newMinY;
    minZ = newMinZ;
    maxX = newMaxX;
    maxY = newMaxY;
    maxZ = newMaxZ;
    return *this;
}

AxisAlignedBB AxisAlignedBB::addCoord(const double x, const double y, const double z) const {
    double newMinX = minX;
    double newMinY = minY;
    double newMinZ = minZ;
    double newMaxX = maxX;
    double newMaxY = maxY;
    double newMaxZ = maxZ;
    if (x < 0.0) {
        newMinX += x;
    }
    if (x > 0.0) {
        newMaxX += x;
    }
    if (y < 0.0) {
        newMinY += y;
    }
    if (y > 0.0) {
        newMaxY += y;
    }
    if (z < 0.0) {
        newMinZ += z;
    }
    if (z > 0.0) {
        newMaxZ += z;
    }
    return AxisAlignedBB(newMinX, newMinY, newMinZ, newMaxX, newMaxY, newMaxZ);
}

AxisAlignedBB AxisAlignedBB::expand(const double x, const double y, const double z) const {
    return AxisAlignedBB(minX - x, minY - y, minZ - z, maxX + x, maxY + y, maxZ + z);
}

AxisAlignedBB AxisAlignedBB::getOffsetBoundingBox(const double x, const double y, const double z) const {
    return AxisAlignedBB(minX + x, minY + y, minZ + z, maxX + x, maxY + y, maxZ + z);
}

double AxisAlignedBB::getAverageEdgeLength() const {
    return ((maxX - minX) + (maxY - minY) + (maxZ - minZ)) / 3.0;
}

double AxisAlignedBB::calculateXOffset(const AxisAlignedBB &other, double offset) const {
    if (other.maxY <= minY || other.minY >= maxY || other.maxZ <= minZ || other.minZ >= maxZ) {
        return offset;
    }
    if (offset > 0.0 && other.maxX <= minX) {
        const double maxOffset = minX - other.maxX;
        if (maxOffset < offset) {
            offset = maxOffset;
        }
    }
    if (offset < 0.0 && other.minX >= maxX) {
        const double maxOffset = maxX - other.minX;
        if (maxOffset > offset) {
            offset = maxOffset;
        }
    }
    return offset;
}

double AxisAlignedBB::calculateYOffset(const AxisAlignedBB &other, double offset) const {
    if (other.maxX <= minX || other.minX >= maxX || other.maxZ <= minZ || other.minZ >= maxZ) {
        return offset;
    }
    if (offset > 0.0 && other.maxY <= minY) {
        const double maxOffset = minY - other.maxY;
        if (maxOffset < offset) {
            offset = maxOffset;
        }
    }
    if (offset < 0.0 && other.minY >= maxY) {
        const double maxOffset = maxY - other.minY;
        if (maxOffset > offset) {
            offset = maxOffset;
        }
    }
    return offset;
}

double AxisAlignedBB::calculateZOffset(const AxisAlignedBB &other, double offset) const {
    if (other.maxX <= minX || other.minX >= maxX || other.maxY <= minY || other.minY >= maxY) {
        return offset;
    }
    if (offset > 0.0 && other.maxZ <= minZ) {
        const double maxOffset = minZ - other.maxZ;
        if (maxOffset < offset) {
            offset = maxOffset;
        }
    }
    if (offset < 0.0 && other.minZ >= maxZ) {
        const double maxOffset = maxZ - other.minZ;
        if (maxOffset > offset) {
            offset = maxOffset;
        }
    }
    return offset;
}

bool AxisAlignedBB::intersectsWith(const AxisAlignedBB &other) const {
    return other.maxX > minX && other.minX < maxX && other.maxY > minY && other.minY < maxY && other.maxZ > minZ &&
           other.minZ < maxZ;
}

AxisAlignedBB &AxisAlignedBB::offset(const double x, const double y, const double z) {
    minX += x;
    minY += y;
    minZ += z;
    maxX += x;
    maxY += y;
    maxZ += z;
    return *this;
}

AxisAlignedBB AxisAlignedBB::copy() const {
    return AxisAlignedBB(minX, minY, minZ, maxX, maxY, maxZ);
}

std::unique_ptr<Vec3D> AxisAlignedBB::calculateIntercept(const Vec3D &start, const Vec3D &end) const {
    const double dx = end.xCoord - start.xCoord;
    const double dy = end.yCoord - start.yCoord;
    const double dz = end.zCoord - start.zCoord;

    double tMin = 0.0, tMax = 1.0;

    auto clip = [&](double s, double e, double lo, double hi) -> bool {
        if (s == e) return s >= lo && s <= hi;
        double t0 = (lo - s) / (e - s);
        double t1 = (hi - s) / (e - s);
        if (t0 > t1) std::swap(t0, t1);
        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);
        return tMin <= tMax;
    };

    if (!clip(start.xCoord, end.xCoord, minX, maxX)) return nullptr;
    if (!clip(start.yCoord, end.yCoord, minY, maxY)) return nullptr;
    if (!clip(start.zCoord, end.zCoord, minZ, maxZ)) return nullptr;

    return std::make_unique<Vec3D>(
        start.xCoord + dx * tMin,
        start.yCoord + dy * tMin,
        start.zCoord + dz * tMin);
}
