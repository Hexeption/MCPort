//
// Created by Keir Davis on 26/06/2026.
//

#include "SpawnerAnimals.h"

#include <cmath>
#include <utility>

#include "game/block/Material.h"
#include "game/client/MathHelper.h"
#include "game/entity/Entity.h"
#include "game/entity/EntityLiving.h"
#include "game/entity/EntityPlayer.h"
#include "game/world/World.h"

namespace {
    constexpr int_t chunkRadius = 4;

    long long packChunkCoords(const int_t chunkX, const int_t chunkZ) {
        return (static_cast<long long>(chunkX) << 32) |
               static_cast<unsigned int>(chunkZ);
    }

    void unpackChunkCoords(const long long packed, int_t &chunkX, int_t &chunkZ) {
        chunkX = static_cast<int_t>(packed >> 32);
        chunkZ = static_cast<int_t>(packed & 0xFFFFFFFFLL);
    }

    EntityPlayer *getClosestPlayer(const World &world, const double x, const double y, const double z,
                                   const double radius) {
        EntityPlayer *closest = nullptr;
        double closestDistSq = radius * radius;
        for (EntityPlayer *player : world.playerEntities) {
            if (player == nullptr || player->isDead) {
                continue;
            }

            const double dx = player->posX - x;
            const double dy = player->posY - y;
            const double dz = player->posZ - z;
            const double distSq = dx * dx + dy * dy + dz * dz;
            if (distSq <= closestDistSq) {
                closestDistSq = distSq;
                closest = player;
            }
        }

        return closest;
    }
}

SpawnerAnimals::SpawnerAnimals(const int_t maxSpawns, EntityMatcher entityMatcher,
                               std::vector<EntityFactory> entities)
    : maxSpawns(maxSpawns), entityMatcher(std::move(entityMatcher)), entities(std::move(entities)) {
}

void SpawnerAnimals::onUpdate(World &world) {
    int_t count = 0;
    for (const auto &entity : world.loadedEntityList) {
        if (entity != nullptr && !entity->isDead && entityMatcher(*entity)) {
            ++count;
        }
    }

    if (count < maxSpawns) {
        for (int_t i = 0; i < 3; ++i) {
            performSpawning(world);
        }
    }
}

ChunkPosition SpawnerAnimals::getRandomSpawningPointInChunk(World &world, const int_t chunkX, const int_t chunkZ) {
    const int_t x = chunkX + world.rand.nextInt(16);
    const int_t y = world.rand.nextInt(128);
    const int_t z = chunkZ + world.rand.nextInt(16);
    return {x, y, z};
}

int_t SpawnerAnimals::performSpawning(World &world) {
    eligibleChunksForSpawning.clear();

    for (EntityPlayer *player : world.playerEntities) {
        if (player == nullptr || player->isDead) {
            continue;
        }

        const int_t chunkX = MathHelper::floor_double(player->posX / 16.0);
        const int_t chunkZ = MathHelper::floor_double(player->posZ / 16.0);
        for (int_t offsetX = -chunkRadius; offsetX <= chunkRadius; ++offsetX) {
            for (int_t offsetZ = -chunkRadius; offsetZ <= chunkRadius; ++offsetZ) {
                eligibleChunksForSpawning.insert(packChunkCoords(chunkX + offsetX, chunkZ + offsetZ));
            }
        }
    }

    for (const long long packedChunk : eligibleChunksForSpawning) {
        if (world.rand.nextInt(10) != 0) {
            continue;
        }

        int_t chunkX = 0;
        int_t chunkZ = 0;
        unpackChunkCoords(packedChunk, chunkX, chunkZ);

        const ChunkPosition spawnPoint = getRandomSpawningPointInChunk(world, chunkX * 16, chunkZ * 16);
        if (world.isBlockNormalCube(spawnPoint.x, spawnPoint.y, spawnPoint.z)) {
            return 0;
        }

        if (world.getBlockMaterial(spawnPoint.x, spawnPoint.y, spawnPoint.z) != Material::air) {
            return 0;
        }

        if (entities.empty()) {
            return 0;
        }

        const int_t entityIndex = world.rand.nextInt(static_cast<int_t>(entities.size()));
        for (int_t i = 0; i < 3; ++i) {
            int_t x = spawnPoint.x;
            int_t y = spawnPoint.y;
            int_t z = spawnPoint.z;

            for (int_t j = 0; j < 2; ++j) {
                x += world.rand.nextInt(6) - world.rand.nextInt(6);
                y += world.rand.nextInt(1) - world.rand.nextInt(1);
                z += world.rand.nextInt(6) - world.rand.nextInt(6);

                if (world.isBlockNormalCube(x, y - 1, z) &&
                    !world.isBlockNormalCube(x, y, z) &&
                    !world.getBlockMaterial(x, y, z)->getIsLiquid() &&
                    !world.isBlockNormalCube(x, y + 1, z)) {
                    const double spawnX = static_cast<double>(x) + 0.5;
                    const double spawnY = static_cast<double>(y);
                    const double spawnZ = static_cast<double>(z) + 0.5;

                    if (getClosestPlayer(world, spawnX, spawnY, spawnZ, 24.0) != nullptr) {
                        continue;
                    }

                    const double dx = spawnX - static_cast<double>(world.spawnX);
                    const double dy = spawnY - static_cast<double>(world.spawnY);
                    const double dz = spawnZ - static_cast<double>(world.spawnZ);
                    const double distSq = dx * dx + dy * dy + dz * dz;
                    if (distSq < 576.0) {
                        continue;
                    }

                    std::unique_ptr<EntityLiving> entity = entities[entityIndex](world);
                    entity->setLocationAndAngles(spawnX, spawnY, spawnZ, world.rand.nextFloat() * 360.0f, 0.0f);
                    if (entity->getCanSpawnHere()) {
                        world.spawnEntityInWorld(std::move(entity));
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}
