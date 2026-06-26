//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ICHUNKPROVIDER_H
#define MCPORT_ICHUNKPROVIDER_H

#include "java/Type.h"

class Chunk;

class IChunkProvider {
public:
    virtual ~IChunkProvider() = default;

    virtual bool chunkExists(int_t chunkX, int_t chunkZ) = 0;

    virtual Chunk provideChunk(int_t chunkX, int_t chunkZ) = 0;

    virtual void populate(IChunkProvider &provider, int_t chunkX, int_t chunkZ) = 0;

    virtual bool saveChunks(bool forceSave, void *progressUpdate) = 0;

    virtual bool unload100OldestChunks() = 0;

    virtual bool canSave() const = 0;
};

#endif //MCPORT_ICHUNKPROVIDER_H
