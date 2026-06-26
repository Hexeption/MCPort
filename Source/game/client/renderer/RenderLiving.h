//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_RENDERLIVING_H
#define MCPORT_RENDERLIVING_H

#include "Render.h"
#include "game/client/renderer/model/ModelBase.h"

class EntityLiving;

class RenderLiving : public Render {
public:
    ModelBase *mainModel = nullptr;
    ModelBase *renderPassModel = nullptr;

    RenderLiving(ModelBase *model, float shadowSize);

    void setRenderPassModel(ModelBase *model);

    virtual void doRenderLiving(EntityLiving &entity, double x, double y, double z,
                               float yaw, float partialTicks);

    void doRender(Entity &entity, double x, double y, double z,
                  float yaw, float partialTicks) override;

protected:
    virtual float renderSwingProgress(EntityLiving &entity, float partialTicks);

    virtual float handleRotationFloat(EntityLiving &entity, float partialTicks);

    virtual void renderEquippedItems(EntityLiving &entity, float partialTicks);

    virtual bool shouldRenderPass(EntityLiving &entity, int pass);

    virtual float getDeathMaxRotation(EntityLiving &entity);

    virtual int getColorMultiplier(EntityLiving &entity, float brightness, float partialTicks);

    virtual void preRenderCallback(EntityLiving &entity, float partialTicks);
};

#endif //MCPORT_RENDERLIVING_H
