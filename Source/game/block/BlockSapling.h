//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKSAPLING_H
#define MCPORT_BLOCKSAPLING_H
#include "BlockFlower.h"

class BlockSapling : public BlockFlower {
public:
    BlockSapling(int_t blockID, int_t blockIndexInTexture);

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;
};


#endif //MCPORT_BLOCKSAPLING_H
