//
// Created by Keir Davis on 25/06/2026.
//

#include "ChunkProviderGenerate.h"

#include <algorithm>
#include <cmath>

#include "Chunk.h"
#include "game/block/Block.h"
#include "game/world/World.h"

ChunkProviderGenerate::ChunkProviderGenerate(World &world, const long_t seed) : worldObj(world), rand(seed) {
    noiseGen1 = std::make_unique<NoiseGeneratorOctaves>(rand, 16);
    noiseGen2 = std::make_unique<NoiseGeneratorOctaves>(rand, 16);
    noiseGen3 = std::make_unique<NoiseGeneratorOctaves>(rand, 8);
    noiseGen4 = std::make_unique<NoiseGeneratorOctaves>(rand, 4);
    noiseGen5 = std::make_unique<NoiseGeneratorOctaves>(rand, 4);
    noiseGen6 = std::make_unique<NoiseGeneratorOctaves>(rand, 10);
    noiseGen7 = std::make_unique<NoiseGeneratorOctaves>(rand, 16);
}

void ChunkProviderGenerate::generateTerrain(const int_t chunkX, const int_t chunkZ,
                                            std::array<int_t, 16 * 128 * 16> &blocks) {
    blocks.fill(0);
    constexpr int_t coarseSize = 4;
    constexpr int_t seaLevel = 64;
    constexpr int_t coarseSizeX = coarseSize + 1;
    constexpr int_t coarseSizeY = 17;
    constexpr int_t coarseSizeZ = coarseSize + 1;
    noiseArray = initializeNoiseField(noiseArray, chunkX * coarseSize, 0, chunkZ * coarseSize, coarseSizeX,
                                      coarseSizeY, coarseSizeZ);

    for (int_t coarseX = 0; coarseX < coarseSize; ++coarseX) {
        for (int_t coarseZ = 0; coarseZ < coarseSize; ++coarseZ) {
            for (int_t coarseY = 0; coarseY < 16; ++coarseY) {
                constexpr double yStep = 0.125;
                double density000 = noiseArray[((coarseX + 0) * coarseSizeZ + coarseZ + 0) * coarseSizeY + coarseY + 0];
                double density001 = noiseArray[((coarseX + 0) * coarseSizeZ + coarseZ + 1) * coarseSizeY + coarseY + 0];
                double density100 = noiseArray[((coarseX + 1) * coarseSizeZ + coarseZ + 0) * coarseSizeY + coarseY + 0];
                double density101 = noiseArray[((coarseX + 1) * coarseSizeZ + coarseZ + 1) * coarseSizeY + coarseY + 0];
                const double delta000 = (noiseArray[((coarseX + 0) * coarseSizeZ + coarseZ + 0) * coarseSizeY +
                                                    coarseY + 1] - density000) * yStep;
                const double delta001 = (noiseArray[((coarseX + 0) * coarseSizeZ + coarseZ + 1) * coarseSizeY +
                                                    coarseY + 1] - density001) * yStep;
                const double delta100 = (noiseArray[((coarseX + 1) * coarseSizeZ + coarseZ + 0) * coarseSizeY +
                                                    coarseY + 1] - density100) * yStep;
                const double delta101 = (noiseArray[((coarseX + 1) * coarseSizeZ + coarseZ + 1) * coarseSizeY +
                                                    coarseY + 1] - density101) * yStep;

                for (int_t subY = 0; subY < 8; ++subY) {
                    constexpr double xStep = 0.25;
                    double densityX0 = density000;
                    double densityX1 = density001;
                    const double deltaX0 = (density100 - density000) * xStep;
                    const double deltaX1 = (density101 - density001) * xStep;

                    for (int_t subX = 0; subX < 4; ++subX) {
                        int_t blockIndex = (subX + coarseX * 4) << 11 | (coarseZ * 4) << 7 | coarseY * 8 + subY;
                        constexpr int_t zStride = 128;
                        constexpr double zStep = 0.25;
                        double density = densityX0;
                        const double deltaZ = (densityX1 - densityX0) * zStep;

                        for (int_t subZ = 0; subZ < 4; ++subZ) {
                            int_t blockId = 0;
                            if (coarseY * 8 + subY < seaLevel) {
                                blockId = 0;
                            }
                            if (density > 0.0) {
                                blockId = Block::stone->blockID;
                            }

                            blocks[blockIndex] = blockId;
                            blockIndex += zStride;
                            density += deltaZ;
                        }

                        densityX0 += deltaX0;
                        densityX1 += deltaX1;
                    }

                    density000 += delta000;
                    density001 += delta001;
                    density100 += delta100;
                    density101 += delta101;
                }
            }
        }
    }
}

void ChunkProviderGenerate::replaceSurfaceBlocks(const int_t chunkX, const int_t chunkZ,
                                                 std::array<int_t, 16 * 128 * 16> &blocks) {
    constexpr int_t seaLevel = 64;
    constexpr double noiseScale = 1.0 / 32.0;
    sandNoise = noiseGen4->generateNoiseOctaves(sandNoise, static_cast<double>(chunkX * 16),
                                                static_cast<double>(chunkZ * 16), 0.0, 16, 16, 1, noiseScale,
                                                noiseScale, 1.0);
    gravelNoise = noiseGen4->generateNoiseOctaves(gravelNoise, static_cast<double>(chunkZ * 16), 109.0134,
                                                  static_cast<double>(chunkX * 16), 16, 1, 16, noiseScale, 1.0,
                                                  noiseScale);
    stoneNoise = noiseGen5->generateNoiseOctaves(stoneNoise, static_cast<double>(chunkX * 16),
                                                 static_cast<double>(chunkZ * 16), 0.0, 16, 16, 1,
                                                 noiseScale * 2.0, noiseScale * 2.0, noiseScale * 2.0);

    for (int_t x = 0; x < 16; ++x) {
        for (int_t z = 0; z < 16; ++z) {
            const bool makeSand = sandNoise[x + z * 16] + rand.nextDouble() * 0.2 > 0.0;
            const int_t surfaceThickness = static_cast<int_t>(stoneNoise[x + z * 16] / 3.0 + 3.0 +
                                                              rand.nextDouble() * 0.25);
            int_t surfaceDepth = -1;
            int_t topBlock = Block::grass->blockID;
            int_t fillerBlock = Block::dirt->blockID;
            const int_t worldX = chunkX * 16 + x;
            const int_t worldZ = chunkZ * 16 + z;
            if (makeSand || (worldX >= -4 && worldX <= 4 && worldZ >= -4 && worldZ <= 4)) {
                topBlock = Block::sand->blockID;
                fillerBlock = Block::sand->blockID;
            }

            for (int_t y = 127; y >= 0; --y) {
                const int_t index = x << 11 | z << 7 | y;
                if (blocks[index] == 0) {
                    surfaceDepth = -1;
                } else if (blocks[index] == Block::stone->blockID) {
                    if (surfaceDepth == -1) {
                        if (surfaceThickness <= 0) {
                            topBlock = 0;
                            fillerBlock = Block::stone->blockID;
                        } else if (y >= seaLevel - 4 && y <= seaLevel + 1) {
                            if (makeSand) {
                                topBlock = Block::sand->blockID;
                                fillerBlock = Block::sand->blockID;
                            } else {
                                topBlock = Block::grass->blockID;
                                fillerBlock = Block::dirt->blockID;
                            }
                        }

                        surfaceDepth = surfaceThickness;
                        blocks[index] = y >= seaLevel - 1 ? topBlock : fillerBlock;
                    } else if (surfaceDepth > 0) {
                        --surfaceDepth;
                        blocks[index] = fillerBlock;
                    }
                }
            }
        }
    }
}

Chunk ChunkProviderGenerate::provideChunk(const int_t chunkX, const int_t chunkZ) {
    rand.setSeed(static_cast<long_t>(chunkX) * 341873128712L + static_cast<long_t>(chunkZ) * 132897987541L);
    std::array<int_t, 16 * 128 * 16> blocks{};
    generateTerrain(chunkX, chunkZ, blocks);
    replaceSurfaceBlocks(chunkX, chunkZ, blocks);
    Chunk chunk(worldObj, blocks, chunkX, chunkZ);
    chunk.generateSkylightMap();
    return chunk;
}

std::vector<double> ChunkProviderGenerate::initializeNoiseField(std::vector<double> noise, const int_t x,
                                                                const int_t y, const int_t z, const int_t sizeX,
                                                                const int_t sizeY, const int_t sizeZ) {
    if (static_cast<int_t>(noise.size()) != sizeX * sizeY * sizeZ) {
        noise.assign(sizeX * sizeY * sizeZ, 0.0);
    }

    constexpr double horizontalScale = 684.412;
    constexpr double verticalScale = 684.412;
    noise6 = noiseGen6->generateNoiseOctaves(noise6, static_cast<double>(x), static_cast<double>(y),
                                             static_cast<double>(z), sizeX, 1, sizeZ, 1.0, 0.0, 1.0);
    noise7 = noiseGen7->generateNoiseOctaves(noise7, static_cast<double>(x), static_cast<double>(y),
                                             static_cast<double>(z), sizeX, 1, sizeZ, 100.0, 0.0, 100.0);
    noise3 = noiseGen3->generateNoiseOctaves(noise3, static_cast<double>(x), static_cast<double>(y),
                                             static_cast<double>(z), sizeX, sizeY, sizeZ, horizontalScale / 80.0,
                                             verticalScale / 160.0, horizontalScale / 80.0);
    noise1 = noiseGen1->generateNoiseOctaves(noise1, static_cast<double>(x), static_cast<double>(y),
                                             static_cast<double>(z), sizeX, sizeY, sizeZ, horizontalScale,
                                             verticalScale, horizontalScale);
    noise2 = noiseGen2->generateNoiseOctaves(noise2, static_cast<double>(x), static_cast<double>(y),
                                             static_cast<double>(z), sizeX, sizeY, sizeZ, horizontalScale,
                                             verticalScale, horizontalScale);

    int_t noiseIndex = 0;
    int_t columnIndex = 0;
    for (int_t ix = 0; ix < sizeX; ++ix) {
        for (int_t iz = 0; iz < sizeZ; ++iz) {
            double terrainScale = (noise6[columnIndex] + 256.0) / 512.0;
            if (terrainScale > 1.0) {
                terrainScale = 1.0;
            }

            constexpr double minimumHeight = 0.0;
            double heightNoise = noise7[columnIndex] / 8000.0;
            if (heightNoise < 0.0) {
                heightNoise = -heightNoise;
            }
            heightNoise = heightNoise * 3.0 - 3.0;
            if (heightNoise < 0.0) {
                heightNoise /= 2.0;
                if (heightNoise < -1.0) {
                    heightNoise = -1.0;
                }
                heightNoise /= 1.4;
                heightNoise /= 2.0;
                terrainScale = 0.0;
            } else {
                if (heightNoise > 1.0) {
                    heightNoise = 1.0;
                }
                heightNoise /= 6.0;
            }

            terrainScale += 0.5;
            heightNoise = heightNoise * static_cast<double>(sizeY) / 16.0;
            const double centerY = static_cast<double>(sizeY) / 2.0 + heightNoise * 4.0;
            ++columnIndex;

            for (int_t iy = 0; iy < sizeY; ++iy) {
                double density = 0.0;
                double yOffset = (static_cast<double>(iy) - centerY) * 12.0 / terrainScale;
                if (yOffset < 0.0) {
                    yOffset *= 4.0;
                }

                const double lowNoise = noise1[noiseIndex] / 512.0;
                const double highNoise = noise2[noiseIndex] / 512.0;
                const double selector = (noise3[noiseIndex] / 10.0 + 1.0) / 2.0;
                if (selector < 0.0) {
                    density = lowNoise;
                } else if (selector > 1.0) {
                    density = highNoise;
                } else {
                    density = lowNoise + (highNoise - lowNoise) * selector;
                }

                density -= yOffset;
                if (iy > sizeY - 4) {
                    const double fade = static_cast<double>(iy - (sizeY - 4)) / 3.0;
                    density = density * (1.0 - fade) + -10.0 * fade;
                }
                if (static_cast<double>(iy) < minimumHeight) {
                    double fade = (minimumHeight - static_cast<double>(iy)) / 4.0;
                    fade = std::clamp(fade, 0.0, 1.0);
                    density = density * (1.0 - fade) + -10.0 * fade;
                }

                noise[noiseIndex] = density;
                ++noiseIndex;
            }
        }
    }

    return noise;
}
