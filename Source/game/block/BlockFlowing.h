//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_BLOCKFLOWING_H
#define MCPORT_BLOCKFLOWING_H

#include <array>

#include "BlockFluid.h"

class BlockFlowing : public BlockFluid {
public:
    BlockFlowing(int_t blockID, Material *material);

    void updateTick(World &world, int_t x, int_t y, int_t z, Random &random) override;

    void onBlockAdded(World &world, int_t x, int_t y, int_t z) override;

private:
    int_t numAdjacentSources = 0;
    std::array<bool, 4> isOptimalFlowDirection{};
    std::array<int_t, 4> flowCost{};

    void updateFlow(World &world, int_t x, int_t y, int_t z) const;

    int_t getFlowDecay(World &world, int_t x, int_t y, int_t z) const;

    int_t getSmallestFlowDecay(World &world, int_t x, int_t y, int_t z, int_t currentSmallest);

    int_t calculateFlowCost(World &world, int_t x, int_t y, int_t z, int_t accumulatedCost, int_t previousDirection);

    const std::array<bool, 4> &getOptimalFlowDirections(World &world, int_t x, int_t y, int_t z);

    bool blockBlocksFlow(World &world, int_t x, int_t y, int_t z) const;

    bool liquidCanDisplaceBlock(World &world, int_t x, int_t y, int_t z) const;

    void flowIntoBlock(World &world, int_t x, int_t y, int_t z, int_t metadata) const;
};

#endif //MCPORT_BLOCKFLOWING_H
