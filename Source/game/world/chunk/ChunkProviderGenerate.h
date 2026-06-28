//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_CHUNKPROVIDERGENERATE_H
#define MCPORT_CHUNKPROVIDERGENERATE_H

#include <array>
#include <memory>
#include <vector>

#include "game/world/chunk/IChunkProvider.h"
#include "game/world/gen/NoiseGeneratorOctaves.h"
#include "java/Random.h"
#include "java/Type.h"

class World;

class ChunkProviderGenerate : public IChunkProvider {
public:
    World &worldObj;
    Random rand;

    ChunkProviderGenerate(World &world, long_t seed);

    void generateTerrain(int_t chunkX, int_t chunkZ, std::array<int_t, 16 * 128 * 16> &blocks);

    void replaceSurfaceBlocks(int_t chunkX, int_t chunkZ, std::array<int_t, 16 * 128 * 16> &blocks);

    bool chunkExists(int_t chunkX, int_t chunkZ) override;

    Chunk provideChunk(int_t chunkX, int_t chunkZ) override;

    void populate(IChunkProvider &provider, int_t chunkX, int_t chunkZ) override;

    bool saveChunks(bool forceSave, void *progressUpdate) override;

    bool unload100OldestChunks() override;

    bool canSave() const override;

private:
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen1;
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen2;
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen3;
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen4;
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen5;
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen6;
    std::unique_ptr<NoiseGeneratorOctaves> noiseGen7;
    std::unique_ptr<NoiseGeneratorOctaves> mobSpawnerNoise;
    std::vector<double> noiseArray;
    std::vector<double> sandNoise;
    std::vector<double> gravelNoise;
    std::vector<double> stoneNoise;
    std::vector<double> noise1;
    std::vector<double> noise2;
    std::vector<double> noise3;
    std::vector<double> noise6;
    std::vector<double> noise7;

    std::vector<double> initializeNoiseField(std::vector<double> noise, int_t x, int_t y, int_t z, int_t sizeX,
                                             int_t sizeY, int_t sizeZ);
};

#endif //MCPORT_CHUNKPROVIDERGENERATE_H
