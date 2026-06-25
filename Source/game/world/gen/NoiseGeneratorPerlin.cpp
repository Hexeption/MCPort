//
// Created by Keir Davis on 25/06/2026.
//

#include "NoiseGeneratorPerlin.h"

NoiseGeneratorPerlin::NoiseGeneratorPerlin(Random &random) {
    xCoord = random.nextDouble() * 256.0;
    yCoord = random.nextDouble() * 256.0;
    zCoord = random.nextDouble() * 256.0;

    for (int_t i = 0; i < 256; ++i) {
        permutations[i] = i;
    }

    for (int_t i = 0; i < 256; ++i) {
        const int_t swapIndex = random.nextInt(256 - i) + i;
        const int_t value = permutations[i];
        permutations[i] = permutations[swapIndex];
        permutations[swapIndex] = value;
        permutations[i + 256] = permutations[i];
    }
}

double NoiseGeneratorPerlin::generateNoise(const double x, const double y, const double z) const {
    double localX = x + xCoord;
    double localY = y + yCoord;
    double localZ = z + zCoord;
    int_t floorX = static_cast<int_t>(localX);
    int_t floorY = static_cast<int_t>(localY);
    int_t floorZ = static_cast<int_t>(localZ);
    if (localX < static_cast<double>(floorX)) {
        --floorX;
    }
    if (localY < static_cast<double>(floorY)) {
        --floorY;
    }
    if (localZ < static_cast<double>(floorZ)) {
        --floorZ;
    }

    const int_t gridX = floorX & 255;
    const int_t gridY = floorY & 255;
    const int_t gridZ = floorZ & 255;
    localX -= static_cast<double>(floorX);
    localY -= static_cast<double>(floorY);
    localZ -= static_cast<double>(floorZ);
    const double fadeX = localX * localX * localX * (localX * (localX * 6.0 - 15.0) + 10.0);
    const double fadeY = localY * localY * localY * (localY * (localY * 6.0 - 15.0) + 10.0);
    const double fadeZ = localZ * localZ * localZ * (localZ * (localZ * 6.0 - 15.0) + 10.0);
    const int_t hashX = permutations[gridX] + gridY;
    const int_t hashXY = permutations[hashX] + gridZ;
    const int_t hashXY1 = permutations[hashX + 1] + gridZ;
    const int_t hashX1 = permutations[gridX + 1] + gridY;
    const int_t hashX1Y = permutations[hashX1] + gridZ;
    const int_t hashX1Y1 = permutations[hashX1 + 1] + gridZ;

    return lerp(fadeZ,
                lerp(fadeY,
                     lerp(fadeX, grad(permutations[hashXY], localX, localY, localZ),
                          grad(permutations[hashX1Y], localX - 1.0, localY, localZ)),
                     lerp(fadeX, grad(permutations[hashXY1], localX, localY - 1.0, localZ),
                          grad(permutations[hashX1Y1], localX - 1.0, localY - 1.0, localZ))),
                lerp(fadeY,
                     lerp(fadeX, grad(permutations[hashXY + 1], localX, localY, localZ - 1.0),
                          grad(permutations[hashX1Y + 1], localX - 1.0, localY, localZ - 1.0)),
                     lerp(fadeX, grad(permutations[hashXY1 + 1], localX, localY - 1.0, localZ - 1.0),
                          grad(permutations[hashX1Y1 + 1], localX - 1.0, localY - 1.0, localZ - 1.0))));
}

double NoiseGeneratorPerlin::generateNoise(const double x, const double y) const {
    return generateNoise(x, y, 0.0);
}

void NoiseGeneratorPerlin::populateNoiseArray(std::vector<double> &noiseArray, const double x, const double y,
                                              const double z, const int_t sizeX, const int_t sizeY,
                                              const int_t sizeZ, const double scaleX, const double scaleY,
                                              const double scaleZ, const double noiseScale) const {
    int_t noiseIndex = 0;
    const double inverseNoiseScale = 1.0 / noiseScale;
    int_t previousGridY = -1;
    double lerpXY0 = 0.0;
    double lerpXY1 = 0.0;
    double lerpXY2 = 0.0;
    double lerpXY3 = 0.0;

    for (int_t ix = 0; ix < sizeX; ++ix) {
        double sampleX = (x + static_cast<double>(ix)) * scaleX + xCoord;
        int_t floorX = static_cast<int_t>(sampleX);
        if (sampleX < static_cast<double>(floorX)) {
            --floorX;
        }
        const int_t gridX = floorX & 255;
        sampleX -= static_cast<double>(floorX);
        const double fadeX = sampleX * sampleX * sampleX * (sampleX * (sampleX * 6.0 - 15.0) + 10.0);

        for (int_t iz = 0; iz < sizeZ; ++iz) {
            double sampleZ = (z + static_cast<double>(iz)) * scaleZ + zCoord;
            int_t floorZ = static_cast<int_t>(sampleZ);
            if (sampleZ < static_cast<double>(floorZ)) {
                --floorZ;
            }
            const int_t gridZ = floorZ & 255;
            sampleZ -= static_cast<double>(floorZ);
            const double fadeZ = sampleZ * sampleZ * sampleZ * (sampleZ * (sampleZ * 6.0 - 15.0) + 10.0);

            for (int_t iy = 0; iy < sizeY; ++iy) {
                double sampleY = (y + static_cast<double>(iy)) * scaleY + yCoord;
                int_t floorY = static_cast<int_t>(sampleY);
                if (sampleY < static_cast<double>(floorY)) {
                    --floorY;
                }
                const int_t gridY = floorY & 255;
                sampleY -= static_cast<double>(floorY);
                const double fadeY = sampleY * sampleY * sampleY * (sampleY * (sampleY * 6.0 - 15.0) + 10.0);
                if (iy == 0 || gridY != previousGridY) {
                    previousGridY = gridY;
                    const int_t hashX = permutations[gridX] + gridY;
                    const int_t hashXZ = permutations[hashX] + gridZ;
                    const int_t hashXZ1 = permutations[hashX + 1] + gridZ;
                    const int_t hashX1 = permutations[gridX + 1] + gridY;
                    const int_t hashX1Z = permutations[hashX1] + gridZ;
                    const int_t hashX1Z1 = permutations[hashX1 + 1] + gridZ;
                    lerpXY0 = lerp(fadeX, grad(permutations[hashXZ], sampleX, sampleY, sampleZ),
                                   grad(permutations[hashX1Z], sampleX - 1.0, sampleY, sampleZ));
                    lerpXY1 = lerp(fadeX, grad(permutations[hashXZ1], sampleX, sampleY - 1.0, sampleZ),
                                   grad(permutations[hashX1Z1], sampleX - 1.0, sampleY - 1.0, sampleZ));
                    lerpXY2 = lerp(fadeX, grad(permutations[hashXZ + 1], sampleX, sampleY, sampleZ - 1.0),
                                   grad(permutations[hashX1Z + 1], sampleX - 1.0, sampleY, sampleZ - 1.0));
                    lerpXY3 = lerp(fadeX, grad(permutations[hashXZ1 + 1], sampleX, sampleY - 1.0, sampleZ - 1.0),
                                   grad(permutations[hashX1Z1 + 1], sampleX - 1.0, sampleY - 1.0,
                                        sampleZ - 1.0));
                }

                const double lerpY0 = lerp(fadeY, lerpXY0, lerpXY1);
                const double lerpY1 = lerp(fadeY, lerpXY2, lerpXY3);
                noiseArray[noiseIndex++] += lerp(fadeZ, lerpY0, lerpY1) * inverseNoiseScale;
            }
        }
    }
}

double NoiseGeneratorPerlin::lerp(const double amount, const double start, const double end) {
    return start + amount * (end - start);
}

double NoiseGeneratorPerlin::grad(const int_t hash, const double x, const double y, const double z) {
    const int_t value = hash & 15;
    const double first = value < 8 ? x : y;
    const double second = value < 4 ? y : (value != 12 && value != 14 ? z : x);
    return ((value & 1) == 0 ? first : -first) + ((value & 2) == 0 ? second : -second);
}
