//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKSAND_H
#define MCPORT_BLOCKSAND_H

#include "Block.h"

class World;

class BlockSand : public Block {
public:
    static bool fallInstantly;

    BlockSand(int_t blockID, int_t blockIndexInTexture);

    void onBlockAdded(World &world, int_t x, int_t y, int_t z) override;

    void onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId) override;

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;

    int_t tickRate() const override;

    static bool canFallBelow(World &world, int_t x, int_t y, int_t z);

private:
    void tryToFall(World &world, int_t x, int_t y, int_t z);
};

#endif //MCPORT_BLOCKSAND_H
