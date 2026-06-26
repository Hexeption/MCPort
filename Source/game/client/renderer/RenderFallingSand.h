//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_RENDERFALLINGSAND_H
#define MCPORT_RENDERFALLINGSAND_H

#include "Render.h"
#include "RenderBlocks.h"

class EntityFallingSand;

class RenderFallingSand : public Render {
private:
    RenderBlocks sandRenderBlocks;

public:
    RenderFallingSand();

    void doRenderFallingSand(EntityFallingSand &entity, double x, double y, double z);

    void doRender(Entity &entity, double x, double y, double z, float yaw, float partialTicks) override;
};

#endif //MCPORT_RENDERFALLINGSAND_H
