//
// Created by Keir Davis on 24/06/2026.
//

#include "World.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "game/block/Block.h"
#include "game/block/BlockFluid.h"
#include "game/client/MathHelper.h"
#include "game/client/renderer/RenderGlobal.h"
#include "game/entity/Entity.h"
#include "game/entity/EntityPlayer.h"
#include "game/nbt/CompressedStreamTools.h"
#include "java/System.h"

std::array<float, 16> World::lightBrightnessTable = World::makeLightBrightnessTable();

static std::unique_ptr<NBTTagCompound> copyLevelData(NBTTagCompound &levelData) {
    auto copy = std::make_unique<NBTTagCompound>();
    copy->setLong(u"RandomSeed", levelData.getLong(u"RandomSeed"));
    copy->setInteger(u"SpawnX", levelData.getInteger(u"SpawnX"));
    copy->setInteger(u"SpawnY", levelData.getInteger(u"SpawnY"));
    copy->setInteger(u"SpawnZ", levelData.getInteger(u"SpawnZ"));
    copy->setLong(u"Time", levelData.getLong(u"Time"));
    copy->setLong(u"SizeOnDisk", levelData.getLong(u"SizeOnDisk"));
    copy->setBoolean(u"SnowCovered", levelData.getBoolean(u"SnowCovered"));
    copy->setLong(u"LastPlayed", levelData.getLong(u"LastPlayed"));
    return copy;
}

static std::unique_ptr<NBTTagCompound> createLevelDataTag(const World &world) {
    auto levelData = std::make_unique<NBTTagCompound>();
    levelData->setLong(u"RandomSeed", world.randomSeed);
    levelData->setInteger(u"SpawnX", world.spawnX);
    levelData->setInteger(u"SpawnY", world.spawnY);
    levelData->setInteger(u"SpawnZ", world.spawnZ);
    levelData->setLong(u"Time", world.worldTime);
    levelData->setLong(u"SizeOnDisk", world.sizeOnDisk);
    levelData->setBoolean(u"SnowCovered", world.snowCovered);
    levelData->setLong(u"LastPlayed", System::currentTimeMillis());
    return levelData;
}

std::unique_ptr<NBTTagCompound> World::getLevelData(const File &minecraftDir, const jstring &levelName) {
    const File savesDirectory(minecraftDir, u"saves");
    const File levelDirectory(savesDirectory, levelName);
    if (!levelDirectory.exists()) {
        return nullptr;
    }

    const File levelDat(levelDirectory, u"level.dat");
    if (!levelDat.exists()) {
        return nullptr;
    }

    try {
        std::unique_ptr<NBTTagCompound> levelDataRoot = CompressedStreamTools::readCompressed(levelDat);
        if (!levelDataRoot->hasKey(u"Data")) {
            return nullptr;
        }
        NBTTagCompound *levelData = levelDataRoot->getCompoundTag(u"Data");
        std::unique_ptr<NBTTagCompound> copy = copyLevelData(*levelData);
        copy->setLong(u"SizeOnDisk", getDirectorySize(levelDirectory));
        return copy;
    } catch (...) {
        auto levelData = std::make_unique<NBTTagCompound>();
        levelData->setLong(u"SizeOnDisk", getDirectorySize(levelDirectory));
        return levelData;
    }
}

void World::deleteWorld(const File &minecraftDir, const jstring &levelName) {
    const File savesDirectory(minecraftDir, u"saves");
    const File levelDirectory(savesDirectory, levelName);
    if (levelDirectory.exists()) {
        deleteWorldFiles(levelDirectory.listFiles());
        levelDirectory.deleteFile();
    }
}

World::World(const File &savesDirectory, const jstring &levelName)
    : World(savesDirectory, levelName, Random().nextLong()) {
}

World::World(const File &savesDirectory, const jstring &levelName, const long_t seed)
    : saveDirectory(savesDirectory, levelName), levelName(levelName), lockTimestamp(System::currentTimeMillis()) {
    savesDirectory.mkdirs();
    saveDirectory.mkdirs();
    createSessionLock();

    const File levelDat(saveDirectory, u"level.dat");
    isNewWorld = !levelDat.exists();
    sizeOnDisk = getDirectorySize(saveDirectory);
    if (!isNewWorld) {
        try {
            std::unique_ptr<NBTTagCompound> levelDataRoot = CompressedStreamTools::readCompressed(levelDat);
            if (levelDataRoot->hasKey(u"Data")) {
                NBTTagCompound *levelData = levelDataRoot->getCompoundTag(u"Data");
                randomSeed = levelData->getLong(u"RandomSeed");
                spawnX = levelData->getInteger(u"SpawnX");
                spawnY = levelData->getInteger(u"SpawnY");
                spawnZ = levelData->getInteger(u"SpawnZ");
                worldTime = levelData->getLong(u"Time");
                sizeOnDisk = levelData->getLong(u"SizeOnDisk");
                snowCovered = levelData->getBoolean(u"SnowCovered");
            }
        } catch (const std::exception &e) {
            std::cerr << "Failed to load level.dat: " << e.what() << '\n';
        }
    }

    if (randomSeed == 0) {
        randomSeed = seed;
        chunkProvider = std::make_unique<ChunkProviderGenerate>(*this, randomSeed);
        worldChunkLoadOverride = true;
        spawnX = 0;
        spawnY = 64;
        spawnZ = 0;
        while (!findSpawn(spawnX, spawnZ)) {
            spawnX += rand.nextInt(64) - rand.nextInt(64);
            spawnZ += rand.nextInt(64) - rand.nextInt(64);
        }
        spawnY = getHeightValue(spawnX, spawnZ);
        worldChunkLoadOverride = false;
    }
    if (chunkProvider == nullptr) {
        chunkProvider = std::make_unique<ChunkProviderGenerate>(*this, randomSeed);
    }

    if (isNewWorld) {
        snowCovered = rand.nextInt(4) == 0;
        sizeOnDisk = getDirectorySize(saveDirectory);
    }

    calculateInitialSkylight();
}

int_t World::getBlockId(const int_t x, const int_t y, const int_t z) const {
    if (!isValidBlockPosition(x, y, z)) {
        return 0;
    }

    const auto found = blockOverrides.find(blockKey(x, y, z));
    if (found != blockOverrides.end()) {
        return found->second;
    }

    return getChunkFromBlockCoords(x, z).getBlockID(blockToLocalCoord(x), y, blockToLocalCoord(z));
}

bool World::blockExists(const int_t x, const int_t y, const int_t z) const {
    if (!isValidBlockPosition(x, y, z)) {
        return false;
    }

    const long_t key = chunkKey(blockToChunkCoord(x), blockToChunkCoord(z));
    return loadedChunks.find(key) != loadedChunks.end();
}

bool World::checkChunksExist(const int_t x0, const int_t y0, const int_t z0, const int_t x1, const int_t y1,
                             const int_t z1) const {
    return y1 >= 0 && y0 < 128 && x0 >= -32000000 && z0 >= -32000000 && x1 < 32000000 && z1 <= 32000000;
}

bool World::setBlockAndMetadata(const int_t x, const int_t y, const int_t z, const int_t blockId,
                                const int_t metadata) {
    if (!isValidBlockPosition(x, y, z)) {
        return false;
    }

    const int_t oldBlockId = getBlockId(x, y, z);
    const int_t oldMetadata = getBlockMetadata(x, y, z);
    if (oldBlockId == blockId && oldMetadata == metadata) {
        return false;
    }

    const long_t key = blockKey(x, y, z);
    blockOverrides[key] = blockId;
    metadataOverrides[key] = metadata;
    Chunk &chunk = getChunkFromBlockCoords(x, z);
    if (oldBlockId != blockId) {
        chunk.setBlockID(blockToLocalCoord(x), y, blockToLocalCoord(z), blockId);
    }
    chunk.setBlockMetadata(blockToLocalCoord(x), y, blockToLocalCoord(z), metadata);
    if (oldBlockId != blockId && blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) &&
        Block::blocksList[blockId] != nullptr && !multiplayerWorld) {
        Block::blocksList[blockId]->onBlockAdded(*this, x, y, z);
    }
    return true;
}

bool World::setBlock(const int_t x, const int_t y, const int_t z, const int_t blockId) {
    if (!isValidBlockPosition(x, y, z)) {
        return false;
    }

    const int_t oldBlockId = getBlockId(x, y, z);
    if (oldBlockId == blockId) {
        return false;
    }

    blockOverrides[blockKey(x, y, z)] = blockId;
    metadataOverrides[blockKey(x, y, z)] = 0;
    getChunkFromBlockCoords(x, z).setBlockID(blockToLocalCoord(x), y, blockToLocalCoord(z), blockId);
    if (blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) &&
        Block::blocksList[blockId] != nullptr && !multiplayerWorld) {
        Block::blocksList[blockId]->onBlockAdded(*this, x, y, z);
    }
    return true;
}

Material *World::getBlockMaterial(const int_t x, const int_t y, const int_t z) const {
    const int_t blockId = getBlockId(x, y, z);
    Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    return block == nullptr ? Material::air : block->material;
}

int_t World::getBlockMetadata(const int_t x, const int_t y, const int_t z) const {
    if (!isValidBlockPosition(x, y, z)) {
        return 0;
    }

    const auto found = metadataOverrides.find(blockKey(x, y, z));
    if (found != metadataOverrides.end()) {
        return found->second;
    }
    return getChunkFromBlockCoords(x, z).getBlockMetadata(blockToLocalCoord(x), y, blockToLocalCoord(z));
}

void World::setBlockMetadataWithNotify(const int_t x, const int_t y, const int_t z, const int_t metadata) {
    setBlockMetadata(x, y, z, metadata);
}

bool World::setBlockMetadata(const int_t x, const int_t y, const int_t z, const int_t metadata) {
    if (!isValidBlockPosition(x, y, z)) {
        return false;
    }

    metadataOverrides[blockKey(x, y, z)] = metadata;
    getChunkFromBlockCoords(x, z).setBlockMetadata(blockToLocalCoord(x), y, blockToLocalCoord(z), metadata);
    return true;
}

bool World::setBlockWithNotify(const int_t x, const int_t y, const int_t z, const int_t blockId) {
    if (!setBlock(x, y, z, blockId)) {
        return false;
    }

    notifyBlockChange(x, y, z, blockId);
    return true;
}

bool World::setBlockAndMetadataWithNotify(const int_t x, const int_t y, const int_t z, const int_t blockId,
                                          const int_t metadata) {
    if (!setBlockAndMetadata(x, y, z, blockId, metadata)) {
        return false;
    }

    notifyBlockChange(x, y, z, blockId);
    return true;
}

void World::markBlockNeedsUpdate(const int_t x, const int_t y, const int_t z) {
    markBlocksDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void World::notifyBlockChange(const int_t x, const int_t y, const int_t z, const int_t blockId) {
    markBlockNeedsUpdate(x, y, z);
    notifyBlocksOfNeighborChange(x, y, z, blockId);
}

void World::markBlockAsNeedsUpdate(const int_t x, const int_t z, int_t y0, int_t y1) {
    if (y0 > y1) {
        std::swap(y0, y1);
    }

    markBlocksDirty(x, y0, z, x, y1, z);
}

void World::markBlocksDirtyVertical(const int_t x, const int_t z, int_t y0, int_t y1) {
    if (y0 > y1) {
        std::swap(y0, y1);
    }

    markBlocksDirty(x, y0, z, x, y1, z);
}

void World::markBlocksDirty(const int_t x0, const int_t y0, const int_t z0, const int_t x1, const int_t y1,
                            const int_t z1) {
    if (renderGlobal != nullptr) {
        renderGlobal->markBlockRangeNeedsUpdate(x0, y0, z0, x1, y1, z1);
    }
}

void World::notifyBlocksOfNeighborChange(const int_t x, const int_t y, const int_t z, const int_t blockId) {
    if (editingBlocks || multiplayerWorld) {
        return;
    }

    constexpr int_t neighborOffsets[6][3] = {
        {0, -1, 0},
        {0, 1, 0},
        {0, 0, -1},
        {0, 0, 1},
        {-1, 0, 0},
        {1, 0, 0},
    };

    for (const auto &offset: neighborOffsets) {
        const int_t neighborX = x + offset[0];
        const int_t neighborY = y + offset[1];
        const int_t neighborZ = z + offset[2];
        const int_t neighborBlockId = getBlockId(neighborX, neighborY, neighborZ);
        if (neighborBlockId <= 0 || neighborBlockId >= static_cast<int_t>(Block::blocksList.size())) {
            continue;
        }

        Block *neighborBlock = Block::blocksList[neighborBlockId];
        if (neighborBlock == nullptr) {
            continue;
        }

        neighborBlock->onNeighborBlockChange(*this, neighborX, neighborY, neighborZ, blockId);
    }
}

bool World::canBlockSeeTheSky(const int_t x, const int_t y, const int_t z) const {
    return y >= getHeightValue(x, z);
}

int_t World::getBlockLightValue(const int_t x, const int_t y, const int_t z) const {
    return getBlockLightValue_do(x, y, z, true);
}

int_t World::getBlockLightValue_do(const int_t x, const int_t y, const int_t z, bool) const {
    if (x < -32000000 || z < -32000000 || x >= 32000000 || z > 32000000) {
        return 15;
    }
    if (y < 0) {
        return 0;
    }
    if (y >= 128) {
        return std::max(0, 15 - skylightSubtracted);
    }

    return getChunkFromChunkCoords(x >> 4, z >> 4).getBlockLightValue(blockToLocalCoord(x), y, blockToLocalCoord(z),
                                                                      skylightSubtracted);
}

bool World::canExistingBlockSeeTheSky(const int_t x, const int_t y, const int_t z) const {
    return blockExists(x, y, z) && canBlockSeeTheSky(x, y, z);
}

bool World::isAirBlock(const int_t x, const int_t y, const int_t z) const {
    return getBlockId(x, y, z) == 0;
}

float World::getBrightness(const int_t x, const int_t y, const int_t z) const {
    return lightBrightnessTable[getBlockLightValue(x, y, z)];
}

bool World::isDayTime() const {
    return skylightSubtracted < 4;
}

bool World::isDaytime() const {
    return skylightSubtracted < 4;
}

int_t World::calculateSkylightSubtracted(const float partialTicks) const {
    const float angle = getCelestialAngle(partialTicks);
    float value = 1.0f - (MathHelper::cos(angle * static_cast<float>(std::acos(-1.0)) * 2.0f) * 2.0f + 0.5f);
    value = std::clamp(value, 0.0f, 1.0f);
    return static_cast<int_t>(value * 11.0f);
}

std::unique_ptr<Vec3D> World::getSkyColor(const float partialTicks) const {
    const float angle = getCelestialAngle(partialTicks);
    float brightness = MathHelper::cos(angle * static_cast<float>(std::acos(-1.0)) * 2.0f) * 2.0f + 0.5f;
    brightness = std::clamp(brightness, 0.0f, 1.0f);
    float red = static_cast<float>(skyColor >> 16 & 255L) / 255.0f;
    float green = static_cast<float>(skyColor >> 8 & 255L) / 255.0f;
    float blue = static_cast<float>(skyColor & 255L) / 255.0f;
    red *= brightness;
    green *= brightness;
    blue *= brightness;
    return Vec3D::createVector(red, green, blue);
}

float World::getCelestialAngle(const float partialTicks) const {
    const int_t time = static_cast<int_t>(worldTime % 24000L);
    float angle = (static_cast<float>(time) + partialTicks) / 24000.0f - 0.25f;
    if (angle < 0.0f) {
        ++angle;
    }
    if (angle > 1.0f) {
        --angle;
    }

    const float originalAngle = angle;
    angle = 1.0f - static_cast<float>((std::cos(static_cast<double>(angle) * std::acos(-1.0)) + 1.0) / 2.0);
    angle = originalAngle + (angle - originalAngle) / 3.0f;
    return angle;
}

std::unique_ptr<Vec3D> World::getCloudColor(const float partialTicks) const {
    const float angle = getCelestialAngle(partialTicks);
    float brightness = MathHelper::cos(angle * static_cast<float>(std::acos(-1.0)) * 2.0f) * 2.0f + 0.5f;
    brightness = std::clamp(brightness, 0.0f, 1.0f);
    float red = static_cast<float>(cloudColor >> 16 & 255L) / 255.0f;
    float green = static_cast<float>(cloudColor >> 8 & 255L) / 255.0f;
    float blue = static_cast<float>(cloudColor & 255L) / 255.0f;
    red *= brightness * 0.9f + 0.1f;
    green *= brightness * 0.9f + 0.1f;
    blue *= brightness * 0.85f + 0.15f;
    return Vec3D::createVector(red, green, blue);
}

std::unique_ptr<Vec3D> World::getFogColor(const float partialTicks) const {
    const float angle = getCelestialAngle(partialTicks);
    float brightness = MathHelper::cos(angle * static_cast<float>(std::acos(-1.0)) * 2.0f) * 2.0f + 0.5f;
    brightness = std::clamp(brightness, 0.0f, 1.0f);
    float red = static_cast<float>(fogColor >> 16 & 255L) / 255.0f;
    float green = static_cast<float>(fogColor >> 8 & 255L) / 255.0f;
    float blue = static_cast<float>(fogColor & 255L) / 255.0f;
    red *= brightness * 0.94f + 0.06f;
    green *= brightness * 0.94f + 0.06f;
    blue *= brightness * 0.91f + 0.09f;
    return Vec3D::createVector(red, green, blue);
}

int_t World::getTopSolidOrLiquidBlock(const int_t x, const int_t z) const {
    for (int_t y = 127; y > 0; --y) {
        Material *material = getBlockMaterial(x, y, z);
        if (material->getIsSolid() || material->getIsLiquid()) {
            return y + 1;
        }
    }

    return -1;
}

int_t World::getPrecipitationHeight(const int_t x, const int_t z) const {
    return getHeightValue(x, z);
}

float World::getStarBrightness(const float partialTicks) const {
    const float angle = getCelestialAngle(partialTicks);
    float brightness = 1.0f - (MathHelper::cos(angle * static_cast<float>(std::acos(-1.0)) * 2.0f) * 2.0f +
                               12.0f / 16.0f);
    brightness = std::clamp(brightness, 0.0f, 1.0f);
    return brightness * brightness * 0.5f;
}

bool World::isBlockNormalCube(const int_t x, const int_t y, const int_t z) const {
    const int_t blockId = getBlockId(x, y, z);
    Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    return block != nullptr && block->isOpaqueCube();
}

std::vector<AxisAlignedBB> World::getCollidingBoundingBoxes(const Entity &, const AxisAlignedBB &box) const {
    std::vector<AxisAlignedBB> boxes;
    const int_t minX = static_cast<int_t>(std::floor(box.minX));
    const int_t maxX = static_cast<int_t>(std::floor(box.maxX + 1.0));
    const int_t minY = static_cast<int_t>(std::floor(box.minY));
    const int_t maxY = static_cast<int_t>(std::floor(box.maxY + 1.0));
    const int_t minZ = static_cast<int_t>(std::floor(box.minZ));
    const int_t maxZ = static_cast<int_t>(std::floor(box.maxZ + 1.0));

    for (int_t x = minX; x < maxX; ++x) {
        for (int_t y = minY; y < maxY; ++y) {
            for (int_t z = minZ; z < maxZ; ++z) {
                const int_t blockId = getBlockId(x, y, z);
                Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                                   ? Block::blocksList[blockId]
                                   : nullptr;
                if (block == nullptr || block->material == nullptr || !block->material->getIsSolid()) {
                    continue;
                }

                AxisAlignedBB blockBox = block->getCollisionBoundingBoxFromPool(*this, x, y, z);
                if (blockBox.intersectsWith(box)) {
                    boxes.push_back(blockBox);
                }
            }
        }
    }

    return boxes;
}

bool World::getIsAnyLiquid(const AxisAlignedBB &box) const {
    int_t minX = MathHelper::floor_double(box.minX);
    int_t maxX = MathHelper::floor_double(box.maxX + 1.0);
    int_t minY = MathHelper::floor_double(box.minY);
    int_t maxY = MathHelper::floor_double(box.maxY + 1.0);
    int_t minZ = MathHelper::floor_double(box.minZ);
    int_t maxZ = MathHelper::floor_double(box.maxZ + 1.0);

    if (box.minX < 0.0) {
        --minX;
    }
    if (box.minY < 0.0) {
        --minY;
    }
    if (box.minZ < 0.0) {
        --minZ;
    }

    for (int_t x = minX; x < maxX; ++x) {
        for (int_t y = minY; y < maxY; ++y) {
            for (int_t z = minZ; z < maxZ; ++z) {
                Block *block = Block::blocksList[getBlockId(x, y, z)];
                if (block != nullptr && block->material->getIsLiquid()) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool World::handleMaterialAcceleration(const AxisAlignedBB &box, Material *material, Entity &entity) {
    int_t minX = MathHelper::floor_double(box.minX);
    int_t maxX = MathHelper::floor_double(box.maxX + 1.0);
    int_t minY = MathHelper::floor_double(box.minY);
    int_t maxY = MathHelper::floor_double(box.maxY + 1.0);
    int_t minZ = MathHelper::floor_double(box.minZ);
    int_t maxZ = MathHelper::floor_double(box.maxZ + 1.0);
    bool inMaterial = false;
    Vec3D acceleration(0.0, 0.0, 0.0);

    for (int_t x = minX; x < maxX; ++x) {
        for (int_t y = minY; y < maxY; ++y) {
            for (int_t z = minZ; z < maxZ; ++z) {
                Block *block = Block::blocksList[getBlockId(x, y, z)];
                if (block != nullptr && block->material == material) {
                    const double fluidHeight = static_cast<double>(y + 1) -
                                               static_cast<double>(BlockFluid::getFluidHeightPercent(
                                                   getBlockMetadata(x, y, z)));
                    if (static_cast<double>(maxY) >= fluidHeight) {
                        inMaterial = true;
                        block->velocityToAddToEntity(*this, x, y, z, entity, acceleration);
                    }
                }
            }
        }
    }

    if (acceleration.lengthVector() > 0.0) {
        std::unique_ptr<Vec3D> normalized = acceleration.normalize();
        constexpr double fluidAcceleration = 0.004;
        entity.motionX += normalized->xCoord * fluidAcceleration;
        entity.motionY += normalized->yCoord * fluidAcceleration;
        entity.motionZ += normalized->zCoord * fluidAcceleration;
    }

    return inMaterial;
}

bool World::isMaterialInBB(const AxisAlignedBB &box, Material *material) const {
    int_t minX = MathHelper::floor_double(box.minX);
    int_t maxX = MathHelper::floor_double(box.maxX + 1.0);
    int_t minY = MathHelper::floor_double(box.minY);
    int_t maxY = MathHelper::floor_double(box.maxY + 1.0);
    int_t minZ = MathHelper::floor_double(box.minZ);
    int_t maxZ = MathHelper::floor_double(box.maxZ + 1.0);

    for (int_t x = minX; x < maxX; ++x) {
        for (int_t y = minY; y < maxY; ++y) {
            for (int_t z = minZ; z < maxZ; ++z) {
                Block *block = Block::blocksList[getBlockId(x, y, z)];
                if (block != nullptr && block->material == material) {
                    return true;
                }
            }
        }
    }

    return false;
}

void World::saveWorld(bool saveChunks, void *progressUpdate) {
    saveLevel();
}

bool World::saveWorld(const int_t progressStep) {
    if (progressStep == 0) {
        saveLevel();
    }

    return true;
}

void World::saveWorldIndirectly(void *progressUpdate) {
    saveWorld(true, progressUpdate);
}

bool World::updatingLighting() {
    int_t updatesRemaining = 1000;
    while (!lightingToUpdate.empty()) {
        --updatesRemaining;
        if (updatesRemaining <= 0) {
            return true;
        }

        MetadataChunkBlock update = lightingToUpdate.back();
        lightingToUpdate.pop_back();
        update.updateLight(*this);
    }

    return false;
}

void World::tick() {
    calculateInitialSkylight();
    ++worldTime;
    if (autosavePeriod > 0 && worldTime % static_cast<long_t>(autosavePeriod) == 0L) {
        saveWorld(false, nullptr);
    }
    tickUpdates(false);
}

int_t World::getHeightValue(int_t x, int_t z) const {
    if (x < -32000000 || z < -32000000 || x >= 32000000 || z > 32000000) {
        return 0;
    }

    if (!chunkExists(x >> 4, z >> 4)) {
        return 0;
    }

    return getChunkFromChunkCoords(x >> 4, z >> 4).getHeightValue(blockToLocalCoord(x), blockToLocalCoord(z));
}

void World::neighborLightPropagationChanged(const EnumSkyBlock skyBlock, const int_t x, const int_t y, const int_t z,
                                            int_t newLightValue) {
    if (!blockExists(x, y, z)) {
        return;
    }

    if (skyBlock == EnumSkyBlock::Sky) {
        if (canExistingBlockSeeTheSky(x, y, z)) {
            newLightValue = 15;
        }
    } else {
        const int_t blockId = getBlockId(x, y, z);
        if (blockId >= 0 && blockId < static_cast<int_t>(Block::lightValue.size()) &&
            Block::lightValue[blockId] > newLightValue) {
            newLightValue = Block::lightValue[blockId];
        }
    }

    if (getSavedLightValue(skyBlock, x, y, z) != newLightValue) {
        scheduleLightingUpdate(skyBlock, x, y, z, x, y, z);
    }
}

int_t World::getSavedLightValue(const EnumSkyBlock skyBlock, const int_t x, const int_t y, const int_t z) const {
    if (y >= 0 && y < 128 && x >= -32000000 && z >= -32000000 && x < 32000000 && z <= 32000000) {
        const int_t chunkX = x >> 4;
        const int_t chunkZ = z >> 4;
        if (!chunkExists(chunkX, chunkZ)) {
            return 0;
        }

        return getChunkFromChunkCoords(chunkX, chunkZ).getSavedLightValue(skyBlock, blockToLocalCoord(x), y,
                                                                          blockToLocalCoord(z));
    }

    return getDefaultLightValue(skyBlock);
}

void World::setLightValue(const EnumSkyBlock skyBlock, const int_t x, const int_t y, const int_t z,
                          const int_t lightValue) {
    if (x < -32000000 || z < -32000000 || x >= 32000000 || z > 32000000) {
        return;
    }
    if (y < 0 || y >= 128) {
        return;
    }
    if (!chunkExists(x >> 4, z >> 4)) {
        return;
    }

    getChunkFromChunkCoords(x >> 4, z >> 4).setLightValue(skyBlock, blockToLocalCoord(x), y, blockToLocalCoord(z),
                                                          lightValue);
    markBlockNeedsUpdate(x, y, z);
}

void World::scheduleLightingUpdate(const EnumSkyBlock skyBlock, const int_t x0, const int_t y0, const int_t z0,
                                   const int_t x1, const int_t y1, const int_t z1) {
    scheduleLightingUpdate_do(skyBlock, x0, y0, z0, x1, y1, z1, true);
}

void World::scheduleLightingUpdate_do(const EnumSkyBlock skyBlock, const int_t x0, const int_t y0, const int_t z0,
                                      const int_t x1, const int_t y1, const int_t z1, const bool checkPrevious) {
    const int_t centerX = (x0 + x1) / 2;
    const int_t centerZ = (z0 + z1) / 2;
    if (!blockExists(centerX, 64, centerZ)) {
        return;
    }

    if (checkPrevious) {
        const int_t scanCount = std::min<int_t>(4, static_cast<int_t>(lightingToUpdate.size()));
        for (int_t i = 0; i < scanCount; ++i) {
            MetadataChunkBlock &entry = lightingToUpdate[lightingToUpdate.size() - i - 1];
            if (entry.skyBlock == skyBlock && entry.getLightUpdated(x0, y0, z0, x1, y1, z1)) {
                return;
            }
        }
    }

    lightingToUpdate.emplace_back(skyBlock, x0, y0, z0, x1, y1, z1);
    if (lightingToUpdate.size() > 100000) {
        while (lightingToUpdate.size() > 50000) {
            updatingLighting();
        }
    }
}

void World::scheduleBlockUpdate(const int_t x, const int_t y, const int_t z, const int_t blockId) {
    constexpr int_t chunkCheckRadius = 8;
    if (!checkChunksExist(x - chunkCheckRadius, y - chunkCheckRadius, z - chunkCheckRadius,
                          x + chunkCheckRadius, y + chunkCheckRadius, z + chunkCheckRadius)) {
        return;
    }

    const NextTickListEntry tickEntry(x, y, z, blockId);
    if (std::any_of(scheduledTickEntries.begin(), scheduledTickEntries.end(),
                    [&tickEntry](const NextTickListEntry &entry) { return entry.equals(tickEntry); })) {
        return;
    }

    long_t scheduledTime = worldTime;
    if (blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) && Block::blocksList[blockId] !=
        nullptr) {
        scheduledTime += Block::blocksList[blockId]->tickRate();
    }
    scheduledTickEntries.push_back(NextTickListEntry(x, y, z, blockId).setScheduledTime(scheduledTime));
}

bool World::tickUpdates(const bool runAllPending) {
    int_t count = static_cast<int_t>(scheduledTickEntries.size());
    if (count > 1000) {
        count = 1000;
    }

    for (int_t i = 0; i < count; ++i) {
        const auto next = std::min_element(scheduledTickEntries.begin(), scheduledTickEntries.end(),
                                           isEarlierScheduledTick);
        if (next == scheduledTickEntries.end()) {
            break;
        }
        if (!runAllPending && next->scheduledTime > worldTime) {
            break;
        }

        const NextTickListEntry entry = *next;
        scheduledTickEntries.erase(next);
        constexpr int_t chunkCheckRadius = 8;
        if (checkChunksExist(entry.xCoord - chunkCheckRadius, entry.yCoord - chunkCheckRadius,
                             entry.zCoord - chunkCheckRadius, entry.xCoord + chunkCheckRadius,
                             entry.yCoord + chunkCheckRadius, entry.zCoord + chunkCheckRadius)) {
            const int_t blockId = getBlockId(entry.xCoord, entry.yCoord, entry.zCoord);
            if (blockId == entry.blockID && blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) &&
                Block::blocksList[blockId] != nullptr) {
                Block::blocksList[blockId]->updateTick(*this, entry.xCoord, entry.yCoord, entry.zCoord, rand);
            }
        }
    }

    return !scheduledTickEntries.empty();
}

void World::randomDisplayUpdates(const int_t x, const int_t y, const int_t z) {
    constexpr int_t radius = 16;
    Random displayRandom;
    for (int_t i = 0; i < 1000; ++i) {
        const int_t blockX = x + rand.nextInt(radius) - rand.nextInt(radius);
        const int_t blockY = y + rand.nextInt(radius) - rand.nextInt(radius);
        const int_t blockZ = z + rand.nextInt(radius) - rand.nextInt(radius);
        const int_t blockId = getBlockId(blockX, blockY, blockZ);
        if (blockId > 0 && blockId < static_cast<int_t>(Block::blocksList.size()) &&
            Block::blocksList[blockId] != nullptr) {
            Block::blocksList[blockId]->randomDisplayTick(*this, blockX, blockY, blockZ, displayRandom);
        }
    }
}

bool World::canBlockBePlacedAt(const int_t blockId, const int_t x, const int_t y, const int_t z,
                               const bool ignoreCollision) {
    (void) ignoreCollision;
    if (blockId <= 0 || blockId >= static_cast<int_t>(Block::blocksList.size())) {
        return false;
    }

    Block *placingBlock = Block::blocksList[blockId];
    if (placingBlock == nullptr) {
        return false;
    }

    const int_t existingBlockId = getBlockId(x, y, z);
    Block *existingBlock = existingBlockId >= 0 && existingBlockId < static_cast<int_t>(Block::blocksList.size())
                               ? Block::blocksList[existingBlockId]
                               : nullptr;
    if (existingBlock != nullptr && !existingBlock->material->getIsLiquid()) {
        return false;
    }

    return placingBlock->canPlaceBlockAt(*this, x, y, z);
}

void World::setSpawnLocation() {
    if (spawnY <= 0) {
        spawnY = 64;
    }

    while (getFirstUncoveredBlock(spawnX, spawnZ) == 0) {
        spawnX += rand.nextInt(8) - rand.nextInt(8);
        spawnZ += rand.nextInt(8) - rand.nextInt(8);
    }
}

void World::sendQuittingDisconnectingPacket() {
}

void World::checkSessionLock() const {
    const File sessionLock(saveDirectory, u"session.lock");
    std::ifstream in(sessionLock.toUtf8(), std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to check session lock, aborting");
    }

    ulong_t value = 0;
    for (int_t shift = 56; shift >= 0; shift -= 8) {
        const int_t byte = in.get();
        if (byte == std::char_traits<char>::eof()) {
            throw std::runtime_error("Failed to check session lock, aborting");
        }
        value |= static_cast<ulong_t>(static_cast<unsigned char>(byte)) << shift;
    }

    if (static_cast<long_t>(value) != lockTimestamp) {
        throw std::runtime_error("The save is being accessed from another location, aborting");
    }
}

void World::setWorldTime(const long_t time) {
    worldTime = time;
    calculateInitialSkylight();
}

void World::setRenderGlobal(RenderGlobal *renderer) {
    renderGlobal = renderer;
}

void World::deleteWorldFiles(const std::vector<File> &files) {
    for (const File &file: files) {
        if (file.isDirectory()) {
            deleteWorldFiles(file.listFiles());
        }
        file.deleteFile();
    }
}

long_t World::getDirectorySize(const File &directory) {
    if (directory.isFile()) {
        return directory.length();
    }

    long_t size = 0;
    for (const File &file: directory.listFiles()) {
        size += getDirectorySize(file);
    }
    return size;
}

long_t World::blockKey(const int_t x, const int_t y, const int_t z) {
    return (static_cast<long_t>(x) & 0x3FFFFFFL) << 38 | (static_cast<long_t>(z) & 0x3FFFFFFL) << 12 |
           (static_cast<long_t>(y) & 0xFFFL);
}

bool World::isValidBlockPosition(const int_t x, const int_t y, const int_t z) {
    return x >= -32000000 && z >= -32000000 && x < 32000000 && z <= 32000000 && y >= 0 && y < 128;
}

int_t World::blockToChunkCoord(const int_t blockCoord) {
    return blockCoord >= 0 ? blockCoord / Chunk::width : (blockCoord - Chunk::width + 1) / Chunk::width;
}

int_t World::blockToLocalCoord(const int_t blockCoord) {
    int_t local = blockCoord % Chunk::width;
    if (local < 0) {
        local += Chunk::width;
    }
    return local;
}

long_t World::chunkKey(const int_t chunkX, const int_t chunkZ) {
    return (static_cast<long_t>(chunkX) << 32) ^ (static_cast<long_t>(chunkZ) & 0xFFFFFFFFL);
}

std::array<float, 16> World::makeLightBrightnessTable() {
    std::array < float, 16 > table{};
    constexpr float baseBrightness = 0.05f;
    for (int_t i = 0; i <= 15; ++i) {
        const float value = 1.0f - static_cast<float>(i) / 15.0f;
        table[i] = (1.0f - value) / (value * 3.0f + 1.0f) * (1.0f - baseBrightness) + baseBrightness;
    }
    return table;
}

int_t World::getDefaultLightValue(const EnumSkyBlock skyBlock) {
    return skyBlock == EnumSkyBlock::Sky ? 15 : 0;
}

bool World::isSpawnBeach(const int_t x, const int_t z) {
    return x >= -4 && x <= 4 && z >= -4 && z <= 4;
}

bool World::isEarlierScheduledTick(const NextTickListEntry &left, const NextTickListEntry &right) {
    return left.comparer(right) < 0;
}

void World::createSessionLock() const {
    const File sessionLock(saveDirectory, u"session.lock");
    std::ofstream out(sessionLock.toUtf8(), std::ios::binary | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Failed to check session lock, aborting");
    }

    const ulong_t value = static_cast<ulong_t>(lockTimestamp);
    for (int_t shift = 56; shift >= 0; shift -= 8) {
        out.put(static_cast<char>((value >> shift) & 0xFF));
    }
}

void World::saveLevel() {
    checkSessionLock();
    sizeOnDisk = getDirectorySize(saveDirectory);

    std::unique_ptr<NBTTagCompound> levelData = createLevelDataTag(*this);
    auto root = std::make_unique<NBTTagCompound>();
    root->setTag(u"Data", levelData.release());

    const File levelDatNew(saveDirectory, u"level.dat_new");
    const File levelDatOld(saveDirectory, u"level.dat_old");
    const File levelDat(saveDirectory, u"level.dat");

    CompressedStreamTools::writeCompressed(*root, levelDatNew);
    if (levelDatOld.exists()) {
        levelDatOld.deleteFile();
    }
    if (levelDat.exists()) {
        levelDat.renameTo(levelDatOld);
    }
    if (levelDat.exists()) {
        levelDat.deleteFile();
    }
    levelDatNew.renameTo(levelDat);
    if (levelDatNew.exists()) {
        levelDatNew.deleteFile();
    }
}

bool World::findSpawn(const int_t x, const int_t z) const {
    return getInitialSpawnLocation(x, z) == Block::sand->blockID;
}

int_t World::getInitialSpawnLocation(const int_t x, const int_t z) const {
    return getFirstUncoveredBlock(x, z);
}

int_t World::getFirstUncoveredBlock(const int_t x, const int_t z) const {
    int_t y = 63;
    while (y < 127 && getBlockId(x, y + 1, z) != 0) {
        ++y;
    }

    return getBlockId(x, y, z);
}

int_t World::getGeneratedBlockId(const int_t x, const int_t y, const int_t z) const {
    if (y >= getHeightValue(x, z)) {
        return 0;
    }
    if (y == getHeightValue(x, z) - 1) {
        return isSpawnBeach(x, z) ? Block::sand->blockID : Block::grass->blockID;
    }
    if (y >= getHeightValue(x, z) - 4) {
        return Block::dirt->blockID;
    }

    return Block::stone->blockID;
}

void World::calculateInitialSkylight() {
    const int_t value = calculateSkylightSubtracted(1.0f);
    if (value != skylightSubtracted) {
        skylightSubtracted = value;
        if (renderGlobal != nullptr) {
            renderGlobal->updateAllRenderers();
        }
    }
}

bool World::chunkExists(const int_t chunkX, const int_t chunkZ) const {
    return loadedChunks.find(chunkKey(chunkX, chunkZ)) != loadedChunks.end();
}

Chunk &World::getChunkFromChunkCoords(const int_t chunkX, const int_t chunkZ) const {
    const long_t key = chunkKey(chunkX, chunkZ);
    const auto found = loadedChunks.find(key);
    if (found != loadedChunks.end()) {
        return *found->second;
    }

    auto chunk = std::make_unique<Chunk>(chunkProvider->provideChunk(chunkX, chunkZ));
    Chunk &chunkRef = *chunk;
    loadedChunks.emplace(key, std::move(chunk));
    return chunkRef;
}

Chunk &World::getChunkFromBlockCoords(const int_t x, const int_t z) const {
    return getChunkFromChunkCoords(blockToChunkCoord(x), blockToChunkCoord(z));
}

MovingObjectPosition World::rayTraceBlocks(const Vec3D &start, const Vec3D &end) {
    return rayTraceBlocks_do(start, end, false);
}

MovingObjectPosition World::rayTraceBlocks_do(const Vec3D &start, const Vec3D &end, bool ignoreLiquid) {
    Vec3D currentPos = start;
    Vec3D endPos = end;

    if (std::isnan(currentPos.xCoord) || std::isnan(currentPos.yCoord) || std::isnan(currentPos.zCoord)) {
        return {};
    }

    if (std::isnan(endPos.xCoord) || std::isnan(endPos.yCoord) || std::isnan(endPos.zCoord)) {
        return {};
    }

    int endX = MathHelper::floor_double(endPos.xCoord);
    int endY = MathHelper::floor_double(endPos.yCoord);
    int endZ = MathHelper::floor_double(endPos.zCoord);

    int blockX = MathHelper::floor_double(currentPos.xCoord);
    int blockY = MathHelper::floor_double(currentPos.yCoord);
    int blockZ = MathHelper::floor_double(currentPos.zCoord);


    int maxSteps = 20;

    while (maxSteps-- >= 0) {
        if (std::isnan(currentPos.xCoord) || std::isnan(currentPos.yCoord) || std::isnan(currentPos.zCoord)) {
            return {};
        }

        if (blockX == endX && blockY == endY && blockZ == endZ) {
            return {};
        }

        double nextXBoundary = 999.0;
        double nextYBoundary = 999.0;
        double nextZBoundary = 999.0;

        if (endX > blockX) nextXBoundary = blockX + 1.0;
        if (endX < blockX) nextXBoundary = blockX;

        if (endY > blockY) nextYBoundary = blockY + 1.0;
        if (endY < blockY) nextYBoundary = blockY;

        if (endZ > blockZ) nextZBoundary = blockZ + 1.0;
        if (endZ < blockZ) nextZBoundary = blockZ;

        double xScale = 999.0;
        double yScale = 999.0;
        double zScale = 999.0;

        double deltaX = endPos.xCoord - currentPos.xCoord;
        double deltaY = endPos.yCoord - currentPos.yCoord;
        double deltaZ = endPos.zCoord - currentPos.zCoord;

        if (nextXBoundary != 999.0)
            xScale = (nextXBoundary - currentPos.xCoord) / deltaX;

        if (nextYBoundary != 999.0)
            yScale = (nextYBoundary - currentPos.yCoord) / deltaY;

        if (nextZBoundary != 999.0)
            zScale = (nextZBoundary - currentPos.zCoord) / deltaZ;

        int hitSide;

        if (xScale < yScale && xScale < zScale) {
            hitSide = (endX > blockX) ? 4 : 5;

            currentPos.xCoord = nextXBoundary;
            currentPos.yCoord += deltaY * xScale;
            currentPos.zCoord += deltaZ * xScale;
        } else if (yScale < zScale) {
            hitSide = (endY > blockY) ? 0 : 1;

            currentPos.xCoord += deltaX * yScale;
            currentPos.yCoord = nextYBoundary;
            currentPos.zCoord += deltaZ * yScale;
        } else {
            hitSide = (endZ > blockZ) ? 2 : 3;

            currentPos.xCoord += deltaX * zScale;
            currentPos.yCoord += deltaY * zScale;
            currentPos.zCoord = nextZBoundary;
        }

        std::unique_ptr<Vec3D> hitVec = Vec3D::createVector(currentPos.xCoord, currentPos.yCoord, currentPos.zCoord);

        blockX = MathHelper::floor_double(currentPos.xCoord);
        hitVec.get()->xCoord = blockX;
        if (hitSide == 5) {
            --blockX;
            ++hitVec.get()->xCoord;
        }

        blockY = MathHelper::floor_double(currentPos.yCoord);
        hitVec.get()->yCoord = blockY;
        if (hitSide == 1) {
            --blockY;
            ++hitVec.get()->yCoord;
        }

        blockZ = MathHelper::floor_double(currentPos.zCoord);
        hitVec.get()->zCoord = blockZ;
        if (hitSide == 3) {
            --blockZ;
            ++hitVec.get()->zCoord;
        }

        const int_t blockId = getBlockId(blockX, blockY, blockZ);
        const int_t metadata = getBlockMetadata(blockX, blockY, blockZ);
        Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                           ? Block::blocksList[blockId]
                           : nullptr;

        if (blockId > 0 && block != nullptr && block->canCollideCheck(metadata, ignoreLiquid)) {
            MovingObjectPosition result = block->collisionRayTrace(*this, blockX, blockY, blockZ, currentPos, endPos);

            if (result.hitVec != nullptr) {
                return result;
            }
        }
    }

    return {};
}
