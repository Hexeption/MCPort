//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_BLOCKSTATIONARY_H
#define MCPORT_BLOCKSTATIONARY_H

#include "BlockFluid.h"

class BlockStationary : public BlockFluid {
public:
    BlockStationary(int_t blockID, Material *material);

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;

    void onNeighborBlockChange(World &world, int_t x, int_t y, int_t z, int_t neighborBlockId) override;

private:
    void setNotStationary(World &world, int_t x, int_t y, int_t z) const;
};

#endif //MCPORT_BLOCKSTATIONARY_H
