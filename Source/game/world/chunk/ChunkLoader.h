//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CHUNKLOADER_H
#define MCPORT_CHUNKLOADER_H

#include <memory>

#include "game/world/chunk/Chunk.h"
#include "java/File.h"
#include "java/Type.h"

#include "game/nbt/NBTTagCompound.h"

class World;

class ChunkLoader {
public:
    ChunkLoader(const File &saveDir, bool createIfNecessary);

    File chunkFileForXZ(int_t chunkX, int_t chunkZ) const;

    std::unique_ptr<Chunk> loadChunk(World &world, int_t chunkX, int_t chunkZ) const;

    void saveChunk(World &world, Chunk &chunk) const;

    void storeChunkInCompound(Chunk &chunk, World &world, NBTTagCompound &nbt) const;

    static std::unique_ptr<Chunk> loadChunkIntoWorldFromCompound(World &world, NBTTagCompound &nbt);

    void saveExtraChunkData(World &world, const Chunk &chunk) const;

    void chunkTick() const;

    void saveExtraData() const;

private:
    File saveDir;
    bool createIfNecessary = false;
};

#endif //MCPORT_CHUNKLOADER_H
