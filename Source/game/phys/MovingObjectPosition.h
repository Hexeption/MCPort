//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_MOVINGOBJECTPOSITION_H
#define MCPORT_MOVINGOBJECTPOSITION_H
#include "game/entity/Entity.h"
#include "game/util/Vec3D.h"


class MovingObjectPosition {
public:
    int typeOfHit{};
    int blockX{};
    int blockY{};
    int blockZ{};
    int sideHit{};
    std::unique_ptr<Vec3D> hitVec;
    std::unique_ptr<Entity> entityHit;

    MovingObjectPosition() = default;

    MovingObjectPosition(int blockX, int blockY, int blockZ, int sideHit, std::unique_ptr<Vec3D> hitVec);

    MovingObjectPosition(const Entity &entityHit);
};


#endif //MCPORT_MOVINGOBJECTPOSITION_H
