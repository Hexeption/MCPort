//
// Created by Keir Davis on 26/06/2026.
//

#include "Frustum.h"

#include "AxisAlignedBB.h"
#include "ClippingHelperImplementation.h"

Frustum::Frustum() : clippingHelper(ClippingHelperImplementation::getInstance()) {
}

void Frustum::setPosition(const double x, const double y, const double z) {
    xPosition = x;
    yPosition = y;
    zPosition = z;
}

bool Frustum::isBoxInFrustum(const double x0, const double y0, const double z0, const double x1, const double y1,
                             const double z1) {
    return clippingHelper->isBoxInFrustum(
        x0 - xPosition, y0 - yPosition, z0 - zPosition,
        x1 - xPosition, y1 - yPosition, z1 - zPosition
    );
}

bool Frustum::isBoundingBoxInFrustum(AxisAlignedBB &box) {
    return isBoxInFrustum(box.minX, box.minY, box.minZ, box.maxX, box.maxY, box.maxZ);
}
