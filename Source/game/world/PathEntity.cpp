//
// Created by Keir Davis on 26/06/2026.
//

#include "PathEntity.h"

#include <utility>

#include "game/util/Vec3D.h"

PathEntity::PathEntity(std::unique_ptr<Vec3D> target) : target(std::move(target)) {
}

std::unique_ptr<Vec3D> PathEntity::getPosition(Entity *) {
    if (finished || target == nullptr) {
        return nullptr;
    }

    return Vec3D::createVector(target->xCoord, target->yCoord, target->zCoord);
}

void PathEntity::incrementPathIndex() {
    finished = true;
}

bool PathEntity::isFinished() {
    return finished;
}
