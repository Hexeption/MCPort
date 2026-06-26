//
// Created by Keir Davis on 26/06/2026.
//

#include "ChunkLoader.h"

#include <algorithm>
#include <iostream>

#include "game/entity/Entity.h"
#include "game/nbt/CompressedStreamTools.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/nbt/NBTTagList.h"
#include "game/world/World.h"

template<std::size_t N>
static std::vector<byte_t> intArrayToByteArray(const std::array<int_t, N> &values) {
    std::vector<byte_t> bytes(values.size());
    for (std::size_t i = 0; i < values.size(); ++i) {
        bytes[i] = static_cast<byte_t>(values[i] & 0xFF);
    }
    return bytes;
}

template<std::size_t N>
static std::array<int_t, N> byteArrayToIntArray(const std::vector<byte_t> &bytes) {
    std::array<int_t, N> values{};
    const std::size_t count = std::min(values.size(), bytes.size());
    for (std::size_t i = 0; i < count; ++i) {
        values[i] = static_cast<int_t>(static_cast<ubyte_t>(bytes[i]));
    }
    return values;
}

ChunkLoader::ChunkLoader(const File &saveDir, const bool createIfNecessary)
    : saveDir(saveDir), createIfNecessary(createIfNecessary) {
}

File ChunkLoader::chunkFileForXZ(const int_t chunkX, const int_t chunkZ) const {
    const jstring fileName = u"c." + String::toString(chunkX, 36) + u"." + String::toString(chunkZ, 36) + u".dat";
    const jstring xDirName = String::toString(chunkX & 63, 36);
    const jstring zDirName = String::toString(chunkZ & 63, 36);
    File xDir(saveDir, xDirName);
    if (!xDir.exists() && createIfNecessary) {
        xDir.mkdir();
    }

    File zDir(xDir, zDirName);
    if (!zDir.exists() && createIfNecessary) {
        zDir.mkdir();
    }

    return File(zDir, fileName);
}

std::unique_ptr<Chunk> ChunkLoader::loadChunk(World &world, const int_t chunkX, const int_t chunkZ) const {
    const File chunkFile = chunkFileForXZ(chunkX, chunkZ);
    if (!chunkFile.exists()) {
        return nullptr;
    }

    try {
        std::unique_ptr<NBTTagCompound> root = CompressedStreamTools::readCompressed(chunkFile);
        if (!root->hasKey(u"Level")) {
            std::cout << "Chunk file at " << chunkX << "," << chunkZ << " is missing level data, skipping\n";
            return nullptr;
        }

        NBTTagCompound *level = root->getCompoundTag(u"Level");
        if (!level->hasKey(u"Blocks")) {
            std::cout << "Chunk file at " << chunkX << "," << chunkZ << " is missing block data, skipping\n";
            return nullptr;
        }

        std::unique_ptr<Chunk> chunk = loadChunkIntoWorldFromCompound(world, *level);
        if (chunk->xPosition != chunkX || chunk->zPosition != chunkZ) {
            std::cout << "Chunk file at " << chunkX << "," << chunkZ << " is in the wrong location; relocating. "
                      << "(Expected " << chunkX << ", " << chunkZ << ", got "
                      << chunk->xPosition << ", " << chunk->zPosition << ")\n";
            level->setInteger(u"xPos", chunkX);
            level->setInteger(u"zPos", chunkZ);
            chunk = loadChunkIntoWorldFromCompound(world, *level);
        }
        return chunk;
    } catch (const std::exception &e) {
        std::cerr << "Failed to load chunk " << chunkX << "," << chunkZ << ": " << e.what() << '\n';
        return nullptr;
    }
}

void ChunkLoader::saveChunk(World &world, Chunk &chunk) const {
    world.checkSessionLock();
    const File chunkFile = chunkFileForXZ(chunk.xPosition, chunk.zPosition);
    if (chunkFile.exists()) {
        world.sizeOnDisk -= chunkFile.length();
    }

    try {
        const File tmpChunk(saveDir, u"tmp_chunk.dat");
        auto root = std::make_unique<NBTTagCompound>();
        auto *level = new NBTTagCompound();
        root->setTag(u"Level", level);
        storeChunkInCompound(chunk, world, *level);
        CompressedStreamTools::writeCompressed(*root, tmpChunk);
        if (chunkFile.exists()) {
            chunkFile.deleteFile();
        }
        tmpChunk.renameTo(chunkFile);
        world.sizeOnDisk += chunkFile.length();
    } catch (const std::exception &e) {
        std::cerr << "Failed to save chunk " << chunk.xPosition << "," << chunk.zPosition << ": " << e.what() << '\n';
    }
}

void ChunkLoader::storeChunkInCompound(Chunk &chunk, World &world, NBTTagCompound &nbt) const {
    world.checkSessionLock();
    nbt.setInteger(u"xPos", chunk.xPosition);
    nbt.setInteger(u"zPos", chunk.zPosition);
    nbt.setLong(u"LastUpdate", world.worldTime);
    nbt.setByteArray(u"Blocks", intArrayToByteArray(chunk.getBlocks()));
    nbt.setByteArray(u"Data", intArrayToByteArray(chunk.getMetadata()));
    nbt.setByteArray(u"SkyLight", intArrayToByteArray(chunk.getSkylightMap()));
    nbt.setByteArray(u"BlockLight", intArrayToByteArray(chunk.getBlocklightMap()));
    nbt.setByteArray(u"HeightMap", intArrayToByteArray(chunk.getHeightMap()));
    nbt.setBoolean(u"TerrainPopulated", chunk.isTerrainPopulated);

    auto *entities = new NBTTagList();
    chunk.hasEntities = false;
    for (const auto &entityBucket: chunk.entities) {
        if (!entityBucket.empty()) {
            chunk.hasEntities = true;
        }

        for (Entity *entity: entityBucket) {
            if (entity == nullptr) {
                continue;
            }

            auto *entityTag = new NBTTagCompound();
            if (entity->addEntityID(*entityTag)) {
                entities->setTag(entityTag);
            } else {
                delete entityTag;
            }
        }
    }
    nbt.setTag(u"Entities", entities);
    nbt.setTag(u"TileEntities", new NBTTagList());
}

std::unique_ptr<Chunk> ChunkLoader::loadChunkIntoWorldFromCompound(World &world, NBTTagCompound &nbt) {
    const int_t chunkX = nbt.getInteger(u"xPos");
    const int_t chunkZ = nbt.getInteger(u"zPos");
    std::array<int_t, Chunk::width * Chunk::height * Chunk::depth> blocks =
            byteArrayToIntArray<Chunk::width * Chunk::height * Chunk::depth>(nbt.getByteArray(u"Blocks"));
    std::array<int_t, Chunk::width * Chunk::height * Chunk::depth> metadata =
            byteArrayToIntArray<Chunk::width * Chunk::height * Chunk::depth>(nbt.getByteArray(u"Data"));

    auto chunk = std::make_unique<Chunk>(world, blocks, metadata, chunkX, chunkZ);
    chunk->isTerrainPopulated = nbt.hasKey(u"TerrainPopulated") && nbt.getBoolean(u"TerrainPopulated");
    if (nbt.hasKey(u"SkyLight") && nbt.hasKey(u"BlockLight") && nbt.hasKey(u"HeightMap")) {
        chunk->setSkylightMap(
            byteArrayToIntArray<Chunk::width * Chunk::height * Chunk::depth>(nbt.getByteArray(u"SkyLight")));
        chunk->setBlocklightMap(
            byteArrayToIntArray<Chunk::width * Chunk::height * Chunk::depth>(nbt.getByteArray(u"BlockLight")));
        chunk->setHeightMap(byteArrayToIntArray<Chunk::width * Chunk::depth>(nbt.getByteArray(u"HeightMap")));
    } else {
        chunk->generateSkylightMap();
    }
    return chunk;
}

void ChunkLoader::saveExtraChunkData(World &, const Chunk &) const {
}

void ChunkLoader::chunkTick() const {
}

void ChunkLoader::saveExtraData() const {
}
