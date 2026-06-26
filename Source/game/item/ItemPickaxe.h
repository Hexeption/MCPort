//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ITEMPICKAXE_H
#define MCPORT_ITEMPICKAXE_H

#include "ItemTool.h"

class ItemPickaxe : public ItemTool {
private:
    int_t harvestLevel;

public:
    ItemPickaxe(int_t id, int_t material);

    bool canHarvestBlock(Block *block) override;
};

#endif //MCPORT_ITEMPICKAXE_H
