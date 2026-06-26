//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_CHUNKPROVIDERLOADORGENERATE_H
#define MCPORT_CHUNKPROVIDERLOADORGENERATE_H

#include <array>
#include <memory>

#include "game/world/chunk/Chunk.h"
#include "game/world/chunk/ChunkLoader.h"
#include "game/world/chunk/ChunkProviderGenerate.h"
#include "game/world/chunk/IChunkProvider.h"
#include "java/Type.h"

class World;

class ChunkProviderLoadOrGenerate : public IChunkProvider {
public:
    ChunkProviderLoadOrGenerate(World &world, std::unique_ptr<ChunkLoader> chunkLoader,
                                std::unique_ptr<ChunkProviderGenerate> chunkProvider);

    bool chunkExists(int_t chunkX, int_t chunkZ) override;

    Chunk &provideChunkRef(int_t chunkX, int_t chunkZ);

    Chunk provideChunk(int_t chunkX, int_t chunkZ) override;

    void populate(IChunkProvider &provider, int_t chunkX, int_t chunkZ) override;

    bool saveChunks(bool forceSave, void *progressUpdate) override;

    bool unload100OldestChunks() override;

    bool canSave() const override;

private:
    World &worldObj;
    std::unique_ptr<Chunk> blankChunk;
    std::unique_ptr<ChunkLoader> chunkLoader;
    std::unique_ptr<ChunkProviderGenerate> chunkProvider;
    std::array<std::unique_ptr<Chunk>, 1024> chunks{};
    int_t lastQueriedChunkXPos = -999999999;
    int_t lastQueriedChunkZPos = -999999999;
    Chunk *lastQueriedChunk = nullptr;

    std::unique_ptr<Chunk> getChunkAt(int_t chunkX, int_t chunkZ, bool &loadedFromDisk);

    void saveExtraChunkData(Chunk &chunk) const;

    void saveChunk(Chunk &chunk) const;

    void loadChunkEntities(int_t chunkX, int_t chunkZ);
};

#endif //MCPORT_CHUNKPROVIDERLOADORGENERATE_H
