//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYDIGGINGFX_H
#define MCPORT_ENTITYDIGGINGFX_H
#include "EntityFX.h"
#include "game/block/Block.h"


class EntityDiggingFX : public EntityFX {
public:
    EntityDiggingFX(World &world, double x, double y, double z, double motionX, double motionY, double motionZ,
                    Block *block);

    int_t getFXLayer() override;

    void renderParticle(Tessellator &tessellator, float partialTicks, float rotationX, float rotationYZ,
                        float rotationZ, float rotationXY, float rotationXZ) override;
};


#endif //MCPORT_ENTITYDIGGINGFX_H
