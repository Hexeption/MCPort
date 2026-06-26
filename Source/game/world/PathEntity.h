//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_PATHENTITY_H
#define MCPORT_PATHENTITY_H

#include <memory>

class Entity;
class Vec3D;

class PathEntity {
private:
    std::unique_ptr<Vec3D> target;
    bool finished = false;

public:
    explicit PathEntity(std::unique_ptr<Vec3D> target);

    std::unique_ptr<Vec3D> getPosition(Entity *entity);
    void incrementPathIndex();
    bool isFinished();
};

#endif //MCPORT_PATHENTITY_H
