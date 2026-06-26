//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_BLOCKLOG_H
#define MCPORT_BLOCKLOG_H
#include "Block.h"


class BlockLog : public Block {
public:
    BlockLog(int_t blockID);

    int_t getBlockTextureFromSide(int_t side) const override;

    int_t quantityDropped(Random random) override;

    int_t idDropped(int_t var1, Random random) override;
};


#endif //MCPORT_BLOCKLOG_H
