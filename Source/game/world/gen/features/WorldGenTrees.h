//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_WORLDGENTREES_H
#define MCPORT_WORLDGENTREES_H
#include "game/world/gen/WorldGenerator.h"

class WorldGenTrees : public WorldGenerator {
public:
    bool generate(World &world, Random &random, int_t x, int_t y, int_t z) override;
};


#endif //MCPORT_WORLDGENTREES_H
