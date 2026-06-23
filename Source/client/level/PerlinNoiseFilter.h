//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_PERLINNOISEFILTER_H
#define MCPORT_PERLINNOISEFILTER_H

#include <vector>

#include "java/Random.h"

class PerlinNoiseFilter {
private:
    Random random = Random();
    int_t seed = 0;
    int_t levels = 0;
    int_t fuzz = 16;

public:
    explicit PerlinNoiseFilter(int_t levels);

    std::vector<int_t> read(int_t width, int_t height);
};


#endif //MCPORT_PERLINNOISEFILTER_H
