//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKWORKBENCH_H
#define MCPORT_BLOCKWORKBENCH_H

#include "Block.h"

class BlockWorkbench : public Block {
public:
    explicit BlockWorkbench(int_t blockID);

    int_t getBlockTextureFromSide(int_t side) const override;

    bool blockActivated(World &world, int_t x, int_t y, int_t z, EntityPlayer &player) override;
};

#endif //MCPORT_BLOCKWORKBENCH_H
