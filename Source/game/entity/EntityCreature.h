//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ENTITYCREATURE_H
#define MCPORT_ENTITYCREATURE_H

#include <memory>

#include "EntityLiving.h"
#include "game/world/PathEntity.h"

class EntityCreature : public EntityLiving {
public:
    Entity *entityToAttack = nullptr;

protected:
    bool hasAttacked = false;
    std::unique_ptr<PathEntity> pathToEntity;

public:
    explicit EntityCreature(World &world);

    void updateEntityActionState() override;

    virtual float getBlockPathWeight(int_t x, int_t y, int_t z) const;

    bool getCanSpawnHere() const override;

protected:
    virtual Entity *findPlayerToAttack();

    virtual void attackEntity(Entity &target, float distance);
};

#endif //MCPORT_ENTITYCREATURE_H
