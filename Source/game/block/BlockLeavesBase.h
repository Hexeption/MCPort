//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKLEAVESBASE_H
#define MCPORT_BLOCKLEAVESBASE_H
#include "Block.h"


class BlockLeavesBase : public Block {
protected:
    bool graphicsLevel;

    BlockLeavesBase(int_t blockID, int_t blockIndexInTexture, Material *material, bool graphicsLevel);

public:
    bool isOpaqueCube() const override;

    bool shouldSideBeRendered(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const override;
};


#endif //MCPORT_BLOCKLEAVESBASE_H
