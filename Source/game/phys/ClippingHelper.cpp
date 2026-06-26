//
// Created by Keir Davis on 26/06/2026.
//

#include "ClippingHelper.h"

#include "java/Type.h"

bool ClippingHelper::isBoxInFrustum(const double x0, const double y0, const double z0, const double x1,
                                    const double y1, const double z1) {
    for (int plane = 0; plane < 6; ++plane) {
        if (static_cast<double>(frustum[plane][0]) * x0 + static_cast<double>(frustum[plane][1]) * y0 +
            static_cast<double>(frustum[plane][2]) * z0 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x1 + static_cast<double>(frustum[plane][1]) * y0 +
            static_cast<double>(frustum[plane][2]) * z0 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x0 + static_cast<double>(frustum[plane][1]) * y1 +
            static_cast<double>(frustum[plane][2]) * z0 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x1 + static_cast<double>(frustum[plane][1]) * y1 +
            static_cast<double>(frustum[plane][2]) * z0 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x0 + static_cast<double>(frustum[plane][1]) * y0 +
            static_cast<double>(frustum[plane][2]) * z1 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x1 + static_cast<double>(frustum[plane][1]) * y0 +
            static_cast<double>(frustum[plane][2]) * z1 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x0 + static_cast<double>(frustum[plane][1]) * y1 +
            static_cast<double>(frustum[plane][2]) * z1 + static_cast<double>(frustum[plane][3]) <= 0.0 &&
            static_cast<double>(frustum[plane][0]) * x1 + static_cast<double>(frustum[plane][1]) * y1 +
            static_cast<double>(frustum[plane][2]) * z1 + static_cast<double>(frustum[plane][3]) <= 0.0) {
            return false;
        }
    }

    return true;
}
