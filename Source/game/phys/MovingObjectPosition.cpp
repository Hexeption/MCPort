//
// Created by Keir Davis on 25/06/2026.
//

#include "MovingObjectPosition.h"

#include "game/entity/Entity.h"

MovingObjectPosition::MovingObjectPosition(const int blockX, const int blockY, const int blockZ, const int sideHit,
                                           std::unique_ptr<Vec3D> hitVec) : typeOfHit(0), blockX(blockX),
                                                                            blockY(blockY), blockZ(blockZ),
                                                                            sideHit(sideHit),
                                                                            hitVec(std::move(hitVec)) {
}

MovingObjectPosition::MovingObjectPosition(Entity *entity) : typeOfHit(1),
                                                              hitVec(entity
                                                                  ? std::make_unique<Vec3D>(entity->posX, entity->posY, entity->posZ)
                                                                  : nullptr),
                                                              entityHit(entity) {
}
