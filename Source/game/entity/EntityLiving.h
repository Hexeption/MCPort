//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYLIVING_H
#define MCPORT_ENTITYLIVING_H

#include "Entity.h"
#include "java/Type.h"

class EntityLiving : public Entity {
public:
    float moveStrafing = 0.0f;
    float moveForward = 0.0f;
    bool isJumping = false;
    float moveSpeed = 0.7f;
    float cameraPitch = 0.0f;
    int_t health = 10;

    explicit EntityLiving(World &world);

    void onUpdate() override;

    float getEyeHeight() const override;

    virtual void onLivingUpdate();

    void moveEntityWithHeading(float strafe, float forward);

protected:
    virtual void jump();
};

#endif //MCPORT_ENTITYLIVING_H
