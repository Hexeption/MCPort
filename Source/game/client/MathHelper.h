//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_MATHHELPER_H
#define MCPORT_MATHHELPER_H

#include "java/Type.h"

class MathHelper {
public:
    static float sin(float value);

    static float cos(float value);

    static float sqrt_float(float value);

    static float sqrt_double(double value);

    static int_t floor_float(float value);

    static int_t floor_double(double value);

    static float abs(float value);

    static double abs_max(double a, double b);

    static int_t bucketInt(int_t value, int_t bucketSize);
};

#endif //MCPORT_MATHHELPER_H
