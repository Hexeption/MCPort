//
// Created by Keir Davis on 23/06/2026.
//

#include "DirtyChunkSorter.h"

#include <cstdarg>
#include <cstdio>

#include "Chunk.h"
#include "client/Player.h"
#include "client/level/Frustum.h"
#include "java/System.h"

DirtyChunkSorter::DirtyChunkSorter(Player &player, Frustum &frustum)
    : player(&player),
      frustum(&frustum),
      now(System::currentTimeMillis()) {
}

int DirtyChunkSorter::compare(Chunk *c0, Chunk *c1) const {
    bool i0 = this->frustum->isVisible(c0->aabb);
    bool i1 = this->frustum->isVisible(c1->aabb);
    if (i0 && !i1) {
        return -1;
    }

    if (i1 && !i0) {
        return 1;
    }

    int t0 = static_cast<int>((this->now - c0->dirtiedTime) / 2000L);
    int t1 = static_cast<int>((this->now - c1->dirtiedTime) / 2000L);
    if (t0 < t1) {
        return -1;
    }

    if (t0 > t1) {
        return 1;
    }

    float d0 = c0->distanceToSqr(*this->player);
    float d1 = c1->distanceToSqr(*this->player);
    int result = d0 < d1 ? -1 : 1;
    return result;
}

bool DirtyChunkSorter::operator()(Chunk *c0, Chunk *c1) const {
    return this->compare(c0, c1) < 0;
}
