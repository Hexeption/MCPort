//
// Created by Keir Davis on 26/06/2026.
//

#include "ChunkProviderLoadOrGenerate.h"

#include <iostream>

#include "game/nbt/CompressedStreamTools.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/world/World.h"

ChunkProviderLoadOrGenerate::ChunkProviderLoadOrGenerate(World &world, std::unique_ptr<ChunkLoader> chunkLoader,
                                                         std::unique_ptr<ChunkProviderGenerate> chunkProvider)
    : worldObj(world), blankChunk(std::make_unique<Chunk>(world, 0, 0)), chunkLoader(std::move(chunkLoader)),
      chunkProvider(std::move(chunkProvider)) {
    blankChunk->isChunkRendered = true;
    blankChunk->neverSave = true;
}

bool ChunkProviderLoadOrGenerate::chunkExists(const int_t chunkX, const int_t chunkZ) {
    if (chunkX == lastQueriedChunkXPos && chunkZ == lastQueriedChunkZPos && lastQueriedChunk != nullptr) {
        return true;
    }

    const int_t localX = chunkX & 31;
    const int_t localZ = chunkZ & 31;
    const int_t index = localX + localZ * 32;
    return chunks[index] != nullptr && (chunks[index].get() == blankChunk.get() || chunks[index]->isAtLocation(chunkX, chunkZ));
}

Chunk &ChunkProviderLoadOrGenerate::provideChunkRef(const int_t chunkX, const int_t chunkZ) {
    if (chunkX == lastQueriedChunkXPos && chunkZ == lastQueriedChunkZPos && lastQueriedChunk != nullptr) {
        return *lastQueriedChunk;
    }

    const int_t localX = chunkX & 31;
    const int_t localZ = chunkZ & 31;
    const int_t index = localX + localZ * 32;
    if (!chunkExists(chunkX, chunkZ)) {
        if (chunks[index] != nullptr) {
            saveChunk(*chunks[index]);
            saveExtraChunkData(*chunks[index]);
        }

        bool loadedFromDisk = false;
        std::unique_ptr<Chunk> chunk = getChunkAt(chunkX, chunkZ, loadedFromDisk);
        if (chunk == nullptr) {
            if (chunkProvider == nullptr) {
                chunk = std::make_unique<Chunk>(*blankChunk);
            } else {
                chunk = std::make_unique<Chunk>(chunkProvider->provideChunk(chunkX, chunkZ));
            }
        }

        Chunk *chunkPtr = chunk.get();
        chunks[index] = std::move(chunk);

        if (loadedFromDisk) {
            loadChunkEntities(chunkX, chunkZ);
        }

        if (!chunkPtr->isTerrainPopulated && chunkExists(chunkX + 1, chunkZ + 1) &&
            chunkExists(chunkX, chunkZ + 1) && chunkExists(chunkX + 1, chunkZ)) {
            populate(*this, chunkX, chunkZ);
        }

        if (chunkExists(chunkX - 1, chunkZ) && !provideChunkRef(chunkX - 1, chunkZ).isTerrainPopulated &&
            chunkExists(chunkX - 1, chunkZ + 1) && chunkExists(chunkX, chunkZ + 1) &&
            chunkExists(chunkX - 1, chunkZ)) {
            populate(*this, chunkX - 1, chunkZ);
        }

        if (chunkExists(chunkX, chunkZ - 1) && !provideChunkRef(chunkX, chunkZ - 1).isTerrainPopulated &&
            chunkExists(chunkX + 1, chunkZ - 1) && chunkExists(chunkX, chunkZ - 1) &&
            chunkExists(chunkX + 1, chunkZ)) {
            populate(*this, chunkX, chunkZ - 1);
        }

        if (chunkExists(chunkX - 1, chunkZ - 1) && !provideChunkRef(chunkX - 1, chunkZ - 1).isTerrainPopulated &&
            chunkExists(chunkX - 1, chunkZ - 1) && chunkExists(chunkX, chunkZ - 1) &&
            chunkExists(chunkX - 1, chunkZ)) {
            populate(*this, chunkX - 1, chunkZ - 1);
        }
    }

    lastQueriedChunkXPos = chunkX;
    lastQueriedChunkZPos = chunkZ;
    lastQueriedChunk = chunks[index].get();
    return *chunks[index];
}

Chunk ChunkProviderLoadOrGenerate::provideChunk(const int_t chunkX, const int_t chunkZ) {
    return provideChunkRef(chunkX, chunkZ);
}

void ChunkProviderLoadOrGenerate::populate(IChunkProvider &provider, const int_t chunkX, const int_t chunkZ) {
    Chunk &chunk = provideChunkRef(chunkX, chunkZ);
    if (chunk.isTerrainPopulated) {
        return;
    }

    chunk.isTerrainPopulated = true;
    if (chunkProvider != nullptr) {
        chunkProvider->populate(provider, chunkX, chunkZ);
        chunk.setChunkModified();
    }
}

bool ChunkProviderLoadOrGenerate::saveChunks(const bool forceSave, void *) {
    int_t saved = 0;
    for (auto &chunk: chunks) {
        if (chunk == nullptr) {
            continue;
        }

        if (forceSave && !chunk->neverSave) {
            saveExtraChunkData(*chunk);
        }

        if (chunk->needsSaving(forceSave)) {
            saveChunk(*chunk);
            chunk->isModified = false;
            ++saved;
            if (saved == 2 && !forceSave) {
                return false;
            }
        }
    }

    if (forceSave && chunkLoader != nullptr) {
        chunkLoader->saveExtraData();
    }

    return chunkProvider == nullptr ? true : chunkProvider->unload100OldestChunks();
}

bool ChunkProviderLoadOrGenerate::unload100OldestChunks() {
    if (chunkLoader != nullptr) {
        chunkLoader->chunkTick();
    }
    return true;
}

bool ChunkProviderLoadOrGenerate::canSave() const {
    return true;
}

std::unique_ptr<Chunk> ChunkProviderLoadOrGenerate::getChunkAt(const int_t chunkX, const int_t chunkZ,
                                                               bool &loadedFromDisk) {
    loadedFromDisk = false;
    if (chunkLoader == nullptr) {
        return nullptr;
    }

    try {
        std::unique_ptr<Chunk> chunk = chunkLoader->loadChunk(worldObj, chunkX, chunkZ);
        if (chunk != nullptr) {
            chunk->lastSaveTime = worldObj.worldTime;
            loadedFromDisk = true;
        }
        return chunk;
    } catch (const std::exception &e) {
        std::cerr << "Failed to load chunk " << chunkX << "," << chunkZ << ": " << e.what() << '\n';
        return nullptr;
    }
}

void ChunkProviderLoadOrGenerate::saveExtraChunkData(Chunk &chunk) const {
    if (chunkLoader == nullptr) {
        return;
    }

    try {
        chunkLoader->saveExtraChunkData(worldObj, chunk);
    } catch (const std::exception &e) {
        std::cerr << "Failed to save extra chunk data " << chunk.xPosition << "," << chunk.zPosition << ": "
                  << e.what() << '\n';
    }
}

void ChunkProviderLoadOrGenerate::saveChunk(Chunk &chunk) const {
    if (chunkLoader == nullptr) {
        return;
    }

    try {
        chunk.lastSaveTime = worldObj.worldTime;
        chunkLoader->saveChunk(worldObj, chunk);
    } catch (const std::exception &e) {
        std::cerr << "Failed to save chunk " << chunk.xPosition << "," << chunk.zPosition << ": " << e.what() << '\n';
    }
}

void ChunkProviderLoadOrGenerate::loadChunkEntities(const int_t chunkX, const int_t chunkZ) {
    if (chunkLoader == nullptr) {
        return;
    }

    try {
        std::unique_ptr<NBTTagCompound> root = CompressedStreamTools::readCompressed(chunkLoader->chunkFileForXZ(chunkX, chunkZ));
        NBTTagCompound *level = root->hasKey(u"Level") ? root->getCompoundTag(u"Level") : root.get();
        worldObj.loadChunkEntities(*level);
    } catch (const std::exception &e) {
        std::cerr << "Failed to load chunk entities " << chunkX << "," << chunkZ << ": " << e.what() << '\n';
    }
}
