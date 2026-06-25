//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_NOISEGENERATOROCTAVES_H
#define MCPORT_NOISEGENERATOROCTAVES_H

#include <memory>
#include <vector>

#include "NoiseGenerator.h"
#include "NoiseGeneratorPerlin.h"
#include "java/Random.h"
#include "java/Type.h"

class NoiseGeneratorOctaves : public NoiseGenerator {
public:
    NoiseGeneratorOctaves(Random &random, int_t octaves);

    double generateNoiseOctaves(double x, double y) const;

    std::vector<double> generateNoiseOctaves(std::vector<double> noiseArray, double x, double y, double z,
                                             int_t sizeX, int_t sizeY, int_t sizeZ, double scaleX, double scaleY,
                                             double scaleZ) const;

private:
    std::vector<std::unique_ptr<NoiseGeneratorPerlin> > generatorCollection;
    int_t octaves;
};

#endif //MCPORT_NOISEGENERATOROCTAVES_H
