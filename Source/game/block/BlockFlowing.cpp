//
// Created by Keir Davis on 25/06/2026.
//

#include "BlockFlowing.h"

#include <algorithm>

#include "game/world/World.h"

BlockFlowing::BlockFlowing(const int_t blockID, Material *material)
    : BlockFluid(blockID, material) {
}

void BlockFlowing::updateFlow(World &world, const int_t x, const int_t y, const int_t z) const {
    const int_t metadata = world.getBlockMetadata(x, y, z);
    world.setBlockAndMetadata(x, y, z, blockID + 1, metadata);
    world.markBlocksDirty(x, y, z, x, y, z);
    world.markBlockNeedsUpdate(x, y, z);
}

int_t BlockFlowing::getFlowDecay(World &world, const int_t x, const int_t y, const int_t z) const {
    return world.getBlockMaterial(x, y, z) != material ? -1 : world.getBlockMetadata(x, y, z);
}

int_t BlockFlowing::getSmallestFlowDecay(World &world, const int_t x, const int_t y, const int_t z,
                                         const int_t currentSmallest) {
    int_t flowDecay = getFlowDecay(world, x, y, z);
    if (flowDecay < 0) {
        return currentSmallest;
    }

    if (flowDecay == 0) {
        ++numAdjacentSources;
    }
    if (flowDecay >= 8) {
        flowDecay = 0;
    }

    return currentSmallest >= 0 && flowDecay >= currentSmallest ? currentSmallest : flowDecay;
}

bool BlockFlowing::blockBlocksFlow(World &world, const int_t x, const int_t y, const int_t z) const {
    const int_t blockId = world.getBlockId(x, y, z);
    if (blockId == 0) {
        return false;
    }

    Block *block = blockId >= 0 && blockId < static_cast<int_t>(Block::blocksList.size())
                       ? Block::blocksList[blockId]
                       : nullptr;
    if (block == nullptr || block->material == nullptr) {
        return true;
    }

    return block->material->isSolid();
}

bool BlockFlowing::liquidCanDisplaceBlock(World &world, const int_t x, const int_t y, const int_t z) const {
    Material *blockMaterial = world.getBlockMaterial(x, y, z);
    return blockMaterial != material && blockMaterial != Material::lava && !blockBlocksFlow(world, x, y, z);
}

int_t BlockFlowing::calculateFlowCost(World &world, const int_t x, const int_t y, const int_t z,
                                      const int_t accumulatedCost, const int_t previousDirection) {
    int_t lowestCost = 1000;

    for (int_t direction = 0; direction < 4; ++direction) {
        if ((direction == 0 && previousDirection == 1) || (direction == 1 && previousDirection == 0) ||
            (direction == 2 && previousDirection == 3) || (direction == 3 && previousDirection == 2)) {
            continue;
        }

        int_t nextX = x;
        int_t nextZ = z;
        if (direction == 0) {
            --nextX;
        } else if (direction == 1) {
            ++nextX;
        } else if (direction == 2) {
            --nextZ;
        } else if (direction == 3) {
            ++nextZ;
        }

        if (blockBlocksFlow(world, nextX, y, nextZ) ||
            (world.getBlockMaterial(nextX, y, nextZ) == material && world.getBlockMetadata(nextX, y, nextZ) == 0)) {
            continue;
        }

        if (!blockBlocksFlow(world, nextX, y - 1, nextZ)) {
            return accumulatedCost;
        }

        if (accumulatedCost < 4) {
            const int_t cost = calculateFlowCost(world, nextX, y, nextZ, accumulatedCost + 1, direction);
            if (cost < lowestCost) {
                lowestCost = cost;
            }
        }
    }

    return lowestCost;
}

const std::array<bool, 4> &BlockFlowing::getOptimalFlowDirections(World &world, const int_t x, const int_t y,
                                                                  const int_t z) {
    for (int_t direction = 0; direction < 4; ++direction) {
        flowCost[direction] = 1000;

        int_t nextX = x;
        int_t nextZ = z;
        if (direction == 0) {
            --nextX;
        } else if (direction == 1) {
            ++nextX;
        } else if (direction == 2) {
            --nextZ;
        } else if (direction == 3) {
            ++nextZ;
        }

        if (blockBlocksFlow(world, nextX, y, nextZ) ||
            (world.getBlockMaterial(nextX, y, nextZ) == material && world.getBlockMetadata(nextX, y, nextZ) == 0)) {
            continue;
        }

        if (!blockBlocksFlow(world, nextX, y - 1, nextZ)) {
            flowCost[direction] = 0;
        } else {
            flowCost[direction] = calculateFlowCost(world, nextX, y, nextZ, 1, direction);
        }
    }

    const int_t lowestCost = *std::min_element(flowCost.begin(), flowCost.end());
    for (int_t direction = 0; direction < 4; ++direction) {
        isOptimalFlowDirection[direction] = flowCost[direction] == lowestCost;
    }

    return isOptimalFlowDirection;
}

void BlockFlowing::flowIntoBlock(World &world, const int_t x, const int_t y, const int_t z,
                                 const int_t metadata) const {
    if (!liquidCanDisplaceBlock(world, x, y, z)) {
        return;
    }

    world.setBlockAndMetadataWithNotify(x, y, z, blockID, metadata);
}

void BlockFlowing::updateTick(World &world, const int_t x, const int_t y, const int_t z, Random &random) {
    int_t flowDecay = getFlowDecay(world, x, y, z);
    bool shouldSetStationary = true;

    if (flowDecay > 0) {
        numAdjacentSources = 0;
        int_t smallestFlowDecay = getSmallestFlowDecay(world, x - 1, y, z, -100);
        smallestFlowDecay = getSmallestFlowDecay(world, x + 1, y, z, smallestFlowDecay);
        smallestFlowDecay = getSmallestFlowDecay(world, x, y, z - 1, smallestFlowDecay);
        smallestFlowDecay = getSmallestFlowDecay(world, x, y, z + 1, smallestFlowDecay);

        int_t newFlowDecay = smallestFlowDecay + fluidType;
        if (newFlowDecay >= 8 || smallestFlowDecay < 0) {
            newFlowDecay = -1;
        }

        const int_t upperFlowDecay = getFlowDecay(world, x, y + 1, z);
        if (upperFlowDecay >= 0) {
            newFlowDecay = upperFlowDecay >= 8 ? upperFlowDecay : upperFlowDecay + 8;
        }

        if (numAdjacentSources >= 2 && material == Material::water) {
            if (world.isBlockNormalCube(x, y - 1, z)) {
                newFlowDecay = 0;
            } else if (world.getBlockMaterial(x, y - 1, z) == material && world.getBlockMetadata(x, y, z) == 0) {
                newFlowDecay = 0;
            }
        }

        if (material == Material::lava && flowDecay < 8 && newFlowDecay < 8 && newFlowDecay > flowDecay &&
            random.nextInt(4) != 0) {
            newFlowDecay = flowDecay;
            shouldSetStationary = false;
        }

        if (newFlowDecay != flowDecay) {
            flowDecay = newFlowDecay;
            if (newFlowDecay < 0) {
                world.setBlockWithNotify(x, y, z, 0);
            } else {
                world.setBlockMetadataWithNotify(x, y, z, newFlowDecay);
                world.scheduleBlockUpdate(x, y, z, blockID);
                world.notifyBlocksOfNeighborChange(x, y, z, blockID);
            }
        } else if (shouldSetStationary) {
            updateFlow(world, x, y, z);
        }
    } else {
        updateFlow(world, x, y, z);
    }

    if (liquidCanDisplaceBlock(world, x, y - 1, z)) {
        flowIntoBlock(world, x, y - 1, z, flowDecay >= 8 ? flowDecay : flowDecay + 8);
    } else if (flowDecay >= 0 && (flowDecay == 0 || blockBlocksFlow(world, x, y - 1, z))) {
        const std::array<bool, 4> &optimalFlowDirections = getOptimalFlowDirections(world, x, y, z);
        int_t nextFlowDecay = flowDecay + fluidType;
        if (flowDecay >= 8) {
            nextFlowDecay = 1;
        }
        if (nextFlowDecay >= 8) {
            return;
        }

        if (optimalFlowDirections[0]) {
            flowIntoBlock(world, x - 1, y, z, nextFlowDecay);
        }
        if (optimalFlowDirections[1]) {
            flowIntoBlock(world, x + 1, y, z, nextFlowDecay);
        }
        if (optimalFlowDirections[2]) {
            flowIntoBlock(world, x, y, z - 1, nextFlowDecay);
        }
        if (optimalFlowDirections[3]) {
            flowIntoBlock(world, x, y, z + 1, nextFlowDecay);
        }
    }
}

void BlockFlowing::onBlockAdded(World &world, const int_t x, const int_t y, const int_t z) {
    BlockFluid::onBlockAdded(world, x, y, z);
    if (world.getBlockId(x, y, z) == blockID) {
        world.scheduleBlockUpdate(x, y, z, blockID);
    }
}
