//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKFLOWER_H
#define MCPORT_BLOCKFLOWER_H
#include "Block.h"


class BlockFlower : public Block {
public:
    BlockFlower(int_t blockID, int_t blockIndexInTexture);

    bool canPlaceBlockAt(World &world, int_t x, int_t y, int_t z) const override;

    bool canThisPlantGrowOnThisBlockID(int blockID) const;

    void onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId) override;

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;

    void checkFlowerChange(World &world, int_t x, int_t y, int_t z);

    bool canBlockStay(World &world, int x, int y, int z) override;

    AxisAlignedBB getCollisionBoundingBoxFromPool(const World &world, int_t x, int_t y, int_t z) const override;

    bool isOpaqueCube() const override;

    bool renderAsNormalBlock() override;

    int_t getRenderType() const override;
};


#endif //MCPORT_BLOCKFLOWER_H
