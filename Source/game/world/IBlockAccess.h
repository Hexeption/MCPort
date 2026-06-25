//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_IBLOCKACCESS_H
#define MCPORT_IBLOCKACCESS_H

#include "game/block/Material.h"
#include "java/Type.h"


class IBlockAccess {
public:
    virtual ~IBlockAccess() = default;

    virtual int_t getBlockId(int_t x, int_t y, int_t z) const = 0;

    virtual float getBrightness(int_t x, int_t y, int_t z) const = 0;

    virtual int_t getBlockMetadata(int_t x, int_t y, int_t z) const = 0;

    virtual Material *getBlockMaterial(int_t x, int_t y, int_t z) const = 0;

    virtual bool isBlockNormalCube(int_t x, int_t y, int_t z) const = 0;
};


#endif //MCPORT_IBLOCKACCESS_H
