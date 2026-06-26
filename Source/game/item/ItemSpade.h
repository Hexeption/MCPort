//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ITEMSPADE_H
#define MCPORT_ITEMSPADE_H

#include "ItemTool.h"

class ItemSpade : public ItemTool {
public:
    ItemSpade(int_t id, int_t material);

    bool canHarvestBlock(Block *block) override;
};

#endif //MCPORT_ITEMSPADE_H
