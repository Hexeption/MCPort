//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKSTONE_H
#define MCPORT_BLOCKSTONE_H
#include "Block.h"


class BlockStone : public Block {
public:
    BlockStone(int_t blockID, int_t blockIndexInTexture);

    int_t idDropped(int_t var1, Random random) override;
};


#endif //MCPORT_BLOCKSTONE_H
