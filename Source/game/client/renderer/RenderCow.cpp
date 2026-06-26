//
// Created by Keir Davis on 26/06/2026.
//

#include "RenderCow.h"

#include "game/entity/EntityCow.h"

RenderCow::RenderCow(ModelBase *model, const float shadowSizeIn)
    : RenderLiving(model, shadowSizeIn) {
}

void RenderCow::renderCow(EntityCow &entity, double x, double y, double z,
                           float yaw, float partialTicks) {
    RenderLiving::doRenderLiving(entity, x, y, z, yaw, partialTicks);
}

void RenderCow::doRenderLiving(EntityLiving &entity, double x, double y, double z,
                                float yaw, float partialTicks) {
    renderCow(dynamic_cast<EntityCow &>(entity), x, y, z, yaw, partialTicks);
}

void RenderCow::doRender(Entity &entity, double x, double y, double z,
                          float yaw, float partialTicks) {
    renderCow(dynamic_cast<EntityCow &>(entity), x, y, z, yaw, partialTicks);
}
