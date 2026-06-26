//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKLEAVES_H
#define MCPORT_BLOCKLEAVES_H
#include "BlockLeavesBase.h"


class BlockLeaves : public BlockLeavesBase {
private:
    int_t leafTexIndex;
    int_t decayCounter = 0;

public:
    BlockLeaves(int_t blockID, int_t blockIndexInTexture);

    int_t quantityDropped(Random random) override;

    int_t idDropped(int_t var1, Random random) override;

    void onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId) override;

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;

    bool isOpaqueCube() const override;

    void setGraphicsLevel(bool fancyLeaves);

private:
    void updateConnectedLeaves(World &world, int_t x, int_t y, int_t z, int_t strength);

    void updateCurrentLeaves(World &world, int_t x, int_t y, int_t z);

    int_t getConnectionStrength(World &world, int_t x, int_t y, int_t z, int_t strength);

    void removeLeaves(World &world, int_t x, int_t y, int_t z);
};


#endif //MCPORT_BLOCKLEAVES_H
