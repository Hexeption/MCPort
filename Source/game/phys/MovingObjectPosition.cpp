//
// Created by Keir Davis on 25/06/2026.
//

#include "MovingObjectPosition.h"

MovingObjectPosition::MovingObjectPosition(const int blockX, const int blockY, const int blockZ, const int sideHit,
                                           std::unique_ptr<Vec3D> hitVec) : typeOfHit(0), blockX(blockX),
                                                                            blockY(blockY), blockZ(blockZ),
                                                                            sideHit(sideHit),
                                                                            hitVec(std::move(hitVec)) {
}

MovingObjectPosition::MovingObjectPosition(const Entity &entityHit) : typeOfHit(1),
                                                                      hitVec(std::make_unique<Vec3D>(
                                                                          entityHit.posX, entityHit.posY,
                                                                          entityHit.posZ)),
                                                                      entityHit(std::make_unique<Entity>(entityHit)) {
}
