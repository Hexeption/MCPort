//
// Created by Keir Davis on 26/06/2026.
//

#include "RenderEntity.h"

#include <glad/glad.h>

#include "game/entity/Entity.h"

void RenderEntity::doRender(Entity &entity, double x, double y, double z, float, float) {
    glPushMatrix();
    renderOffsetAABB(
        entity.boundingBox,
        x - entity.lastTickPosX,
        y - entity.lastTickPosY,
        z - entity.lastTickPosZ
    );
    glPopMatrix();
}
