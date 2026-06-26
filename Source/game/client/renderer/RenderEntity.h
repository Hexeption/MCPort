//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_RENDERENTITY_H
#define MCPORT_RENDERENTITY_H

#include "Render.h"

class Entity;

class RenderEntity : public Render {
public:
    void doRender(Entity &entity, double x, double y, double z, float yaw, float partialTicks) override;
};

#endif //MCPORT_RENDERENTITY_H
