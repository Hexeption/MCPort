//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKGRAVEL_H
#define MCPORT_BLOCKGRAVEL_H

#include "BlockSand.h"

class BlockGravel : public BlockSand {
public:
    BlockGravel(int_t blockID, int_t blockIndexInTexture);

    int_t idDropped(int_t metadata, Random random) override;
};

#endif //MCPORT_BLOCKGRAVEL_H
