//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_HITRESULT_H
#define MCPORT_HITRESULT_H

#include "java/Type.h"

class HitResult {
public:
    int_t type;
    int_t x;
    int_t y;
    int_t z;
    int_t f;

    HitResult(int_t type, int_t x, int_t y, int_t z, int_t f);
};

#endif //MCPORT_HITRESULT_H
