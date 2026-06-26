//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKGRASS_H
#define MCPORT_BLOCKGRASS_H
#include "Block.h"


class BlockGrass : public Block {
public:
    BlockGrass(int_t blockID);

    int_t getBlockTexture(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const override;
    int_t getBlockTextureFromSide(int_t side) const override;

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;

    int_t idDropped(int_t var1, Random random) override;
};


#endif //MCPORT_BLOCKGRASS_H
