//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_NOISEGENERATORPERLIN_H
#define MCPORT_NOISEGENERATORPERLIN_H

#include <array>
#include <vector>

#include "NoiseGenerator.h"
#include "java/Random.h"
#include "java/Type.h"

class NoiseGeneratorPerlin : public NoiseGenerator {
public:
    double xCoord = 0.0;
    double yCoord = 0.0;
    double zCoord = 0.0;

    explicit NoiseGeneratorPerlin(Random &random);

    double generateNoise(double x, double y, double z) const;

    double generateNoise(double x, double y) const;

    void populateNoiseArray(std::vector<double> &noiseArray, double x, double y, double z, int_t sizeX,
                            int_t sizeY, int_t sizeZ, double scaleX, double scaleY, double scaleZ,
                            double noiseScale) const;

private:
    std::array<int_t, 512> permutations{};

    static double lerp(double amount, double start, double end);

    static double grad(int_t hash, double x, double y, double z);
};

#endif //MCPORT_NOISEGENERATORPERLIN_H
