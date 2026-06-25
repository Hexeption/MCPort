//
// Created by Keir Davis on 25/06/2026.
//

#include "NoiseGeneratorOctaves.h"

#include <algorithm>

NoiseGeneratorOctaves::NoiseGeneratorOctaves(Random &random, const int_t octaves) : octaves(octaves) {
    generatorCollection.reserve(octaves);
    for (int_t i = 0; i < octaves; ++i) {
        generatorCollection.push_back(std::make_unique<NoiseGeneratorPerlin>(random));
    }
}

double NoiseGeneratorOctaves::generateNoiseOctaves(const double x, const double y) const {
    double result = 0.0;
    double scale = 1.0;
    for (int_t i = 0; i < octaves; ++i) {
        result += generatorCollection[i]->generateNoise(x * scale, y * scale) / scale;
        scale /= 2.0;
    }
    return result;
}

std::vector<double> NoiseGeneratorOctaves::generateNoiseOctaves(std::vector<double> noiseArray, const double x,
                                                                const double y, const double z, const int_t sizeX,
                                                                const int_t sizeY, const int_t sizeZ,
                                                                const double scaleX, const double scaleY,
                                                                const double scaleZ) const {
    const int_t size = sizeX * sizeY * sizeZ;
    if (static_cast<int_t>(noiseArray.size()) != size) {
        noiseArray.assign(size, 0.0);
    } else {
        std::fill(noiseArray.begin(), noiseArray.end(), 0.0);
    }

    double octaveScale = 1.0;
    for (int_t i = 0; i < octaves; ++i) {
        generatorCollection[i]->populateNoiseArray(noiseArray, x, y, z, sizeX, sizeY, sizeZ, scaleX * octaveScale,
                                                   scaleY * octaveScale, scaleZ * octaveScale, octaveScale);
        octaveScale /= 2.0;
    }
    return noiseArray;
}
