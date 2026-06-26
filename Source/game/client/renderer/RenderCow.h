//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_RENDERCOW_H
#define MCPORT_RENDERCOW_H

#include "RenderLiving.h"

class EntityCow;

class RenderCow : public RenderLiving {
public:
    RenderCow(ModelBase *model, float shadowSize);

    void renderCow(EntityCow &entity, double x, double y, double z,
                   float yaw, float partialTicks);

    void doRenderLiving(EntityLiving &entity, double x, double y, double z,
                        float yaw, float partialTicks) override;

    void doRender(Entity &entity, double x, double y, double z,
                  float yaw, float partialTicks) override;
};

#endif //MCPORT_RENDERCOW_H
