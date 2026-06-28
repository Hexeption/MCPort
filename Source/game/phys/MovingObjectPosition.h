//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_MOVINGOBJECTPOSITION_H
#define MCPORT_MOVINGOBJECTPOSITION_H

#include <memory>

#include "game/util/Vec3D.h"

#include "game/entity/Entity.h"

class MovingObjectPosition {
public:
    int typeOfHit{};
    int blockX{};
    int blockY{};
    int blockZ{};
    int sideHit{};
    std::unique_ptr<Vec3D> hitVec;
    Entity *entityHit = nullptr;

    MovingObjectPosition() = default;

    MovingObjectPosition(int blockX, int blockY, int blockZ, int sideHit, std::unique_ptr<Vec3D> hitVec);

    explicit MovingObjectPosition(Entity *entityHit);
};


#endif //MCPORT_MOVINGOBJECTPOSITION_H
