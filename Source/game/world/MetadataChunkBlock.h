//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_METADATACHUNKBLOCK_H
#define MCPORT_METADATACHUNKBLOCK_H

#include "EnumSkyBlock.h"
#include "java/Type.h"

class World;

class MetadataChunkBlock {
public:
    const EnumSkyBlock skyBlock;
    int_t minX;
    int_t minY;
    int_t minZ;
    int_t maxX;
    int_t maxY;
    int_t maxZ;
    int_t x = 0;
    int_t y = 0;
    int_t z = 0;

    MetadataChunkBlock(EnumSkyBlock skyBlock, int_t minX, int_t minY, int_t minZ, int_t maxX, int_t maxY,
                       int_t maxZ);

    void updateLight(World &world);

    bool getLightUpdated(int_t minX, int_t minY, int_t minZ, int_t maxX, int_t maxY, int_t maxZ);
};

#endif //MCPORT_METADATACHUNKBLOCK_H
