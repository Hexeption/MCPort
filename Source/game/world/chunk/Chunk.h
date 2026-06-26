//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_CHUNK_H
#define MCPORT_CHUNK_H

#include <array>
#include <vector>

#include "game/entity/Entity.h"
#include "game/world/EnumSkyBlock.h"
#include "java/Type.h"

class World;

class Chunk {
public:
    static constexpr int_t width = 16;
    static constexpr int_t height = 128;
    static constexpr int_t depth = 16;

    static bool isLit;

    World &worldObj;
    std::array<int_t, width * height * depth> blocks{};
    int_t xPosition;
    int_t zPosition;
    std::array<int_t, width * depth> heightMap{};
    int_t heightValue = 0;
    bool isTerrainPopulated = false;
    bool isModified = false;
    std::array<std::vector<Entity *>, 8> entities{};
    bool hasEntities = false;
    bool isChunkRendered = false;

    Chunk(World &world, int_t chunkX, int_t chunkZ);

    Chunk(World &world, const std::array<int_t, width * height * depth> &blocks, int_t chunkX, int_t chunkZ);

    Chunk(World &world, const std::array<int_t, width * height * depth> &blocks,
          const std::array<int_t, width * height * depth> &metadata, int_t chunkX, int_t chunkZ);

    const std::array<int_t, width * height * depth> &getBlocks() const;

    const std::array<int_t, width * height * depth> &getMetadata() const;

    const std::array<int_t, width * depth> &getHeightMap() const;

    const std::array<int_t, width * height * depth> &getSkylightMap() const;

    const std::array<int_t, width * height * depth> &getBlocklightMap() const;

    int_t getBlockID(int_t x, int_t y, int_t z) const;

    bool setBlockID(int_t x, int_t y, int_t z, int_t blockId);

    int_t getBlockMetadata(int_t x, int_t y, int_t z) const;

    bool setBlockMetadata(int_t x, int_t y, int_t z, int_t metadata);

    void setHeightMap(const std::array<int_t, width * depth> &values);

    void setSkylightMap(const std::array<int_t, width * height * depth> &values);

    void setBlocklightMap(const std::array<int_t, width * height * depth> &values);

    int_t getHeightValue(int_t x, int_t z) const;

    int_t getSavedLightValue(EnumSkyBlock skyBlock, int_t x, int_t y, int_t z) const;

    void setLightValue(EnumSkyBlock skyBlock, int_t x, int_t y, int_t z, int_t value);

    int_t getBlockLightValue(int_t x, int_t y, int_t z, int_t skylightSubtracted) const;

    void generateHeightMap();

    void generateSkylightMap();

    void addEntity(Entity &entity);

    void removeEntity(Entity &entity);

    void removeEntityAtIndex(Entity &entity, int_t index);

private:
    std::array<int_t, width * height * depth> metadata{};
    std::array<int_t, width * height * depth> skylightMap{};
    std::array<int_t, width * height * depth> blocklightMap{};

    static int_t blockIndex(int_t x, int_t y, int_t z);

    static int_t heightIndex(int_t x, int_t z);

    static bool isValidLocalBlock(int_t x, int_t y, int_t z);

    void updateSkylight_do(int_t x, int_t z);

    void checkSkylightNeighborHeight(int_t x, int_t z, int_t height);

    void relightBlock(int_t x, int_t y, int_t z);

    void recalculateHeightValue();
};

#endif //MCPORT_CHUNK_H
