//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_SPAWNERANIMALS_H
#define MCPORT_SPAWNERANIMALS_H

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "java/Type.h"

#include "game/entity/Entity.h"
#include "game/entity/EntityLiving.h"
#include "game/world/World.h"

struct ChunkPosition {
    int_t x;
    int_t y;
    int_t z;
};

class SpawnerAnimals {
public:
    using EntityMatcher = std::function<bool(const Entity &)>;
    using EntityFactory = std::function<std::unique_ptr<EntityLiving>(World &)>;

    SpawnerAnimals(int_t maxSpawns, EntityMatcher entityMatcher, std::vector<EntityFactory> entities);

    void onUpdate(World &world);

protected:
    virtual ChunkPosition getRandomSpawningPointInChunk(World &world, int_t chunkX, int_t chunkZ);

private:
    int_t maxSpawns;
    EntityMatcher entityMatcher;
    std::vector<EntityFactory> entities;
    std::unordered_set<long long> eligibleChunksForSpawning;

    int_t performSpawning(World &world);
};

#endif //MCPORT_SPAWNERANIMALS_H
