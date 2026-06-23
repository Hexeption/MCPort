//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_DIRTYCHUNKSORTER_H
#define MCPORT_DIRTYCHUNKSORTER_H

#include <cstdio>

#include "java/Type.h"

class Chunk;
class Frustum;
class Player;

class DirtyChunkSorter {
private:
    Player *player;
    Frustum *frustum;
    long_t now;

public:
    DirtyChunkSorter(Player &player, Frustum &frustum);

    int compare(Chunk *c0, Chunk *c1) const;

    bool operator()(Chunk *c0, Chunk *c1) const;
};

#endif //MCPORT_DIRTYCHUNKSORTER_H
