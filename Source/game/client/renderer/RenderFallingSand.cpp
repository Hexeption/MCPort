//
// Created by Keir Davis on 26/06/2026.
//

#include "RenderFallingSand.h"

#include <glad/glad.h>

#include "RenderManager.h"
#include "game/block/Block.h"
#include "game/client/MathHelper.h"
#include "game/entity/EntityFallingSand.h"
#include "game/world/World.h"

RenderFallingSand::RenderFallingSand() {
    shadowSize = 0.5f;
}

void RenderFallingSand::doRenderFallingSand(EntityFallingSand &entity, const double x, const double y, const double z) {
    Block *block = Block::blocksList[entity.blockID];
    if (block == nullptr) {
        return;
    }

    loadTexture(u"/terrain.png");

    glPushMatrix();
    glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

    glDisable(GL_LIGHTING);

    sandRenderBlocks.renderBlockFallingSand(
        block,
        entity.getWorld(),
        MathHelper::floor_double(entity.posX),
        MathHelper::floor_double(entity.posY),
        MathHelper::floor_double(entity.posZ)
    );

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void RenderFallingSand::doRender(Entity &entity, const double x, const double y, const double z,
                                 const float yaw, const float partialTicks) {
    doRenderFallingSand(static_cast<EntityFallingSand &>(entity), x, y, z);
}
