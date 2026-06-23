//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_RANDOM_H
#define MCPORT_RANDOM_H
#include "Type.h"


class Random {
private:
    long_t seed;

public:
    Random();

    Random(long_t seed);

    void setSeed(long_t seed);

    int_t next(int_t bits);

    bool nextBoolean();

    int_t nextInt();

    int_t nextInt(int_t bound);

    long_t nextLong();

    float nextFloat();

    double nextDouble();
};


#endif //MCPORT_RANDOM_H
