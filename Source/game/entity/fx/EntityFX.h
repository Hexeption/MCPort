//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYFX_H
#define MCPORT_ENTITYFX_H
#include "game/client/renderer/Tessellator.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/entity/Entity.h"

class EntityFX : public Entity {
protected:
    int_t particleTextureIndex = 0;
    float particleTextureJitterX = 0.0f;
    float particleTextureJitterY = 0.0f;
    int_t particleAge = 0;
    int_t particleMaxAge = 0;
    float particleScale = 0.0f;
    float particleGravity = 0.0f;
    float particleRed = 1.0f;
    float particleGreen = 1.0f;
    float particleBlue = 1.0f;

public:
    static double interpPosX;
    static double interpPosY;
    static double interpPosZ;

    EntityFX(World &worldObj, double x, double y, double z, double motionX, double motionY, double motionZ);

    EntityFX &multiplyVelocity(float multiplier);

    EntityFX &multipleParticleScaleBy(float scale);

    void onUpdate() override;

    virtual void renderParticle(Tessellator &tessellator, float partialTicks, float rotationX, float rotationYZ,
                                float rotationZ, float rotationXY, float rotationXZ);

    virtual int_t getFXLayer();

    virtual void writeEntityToNBT(NBTTagCompound compound);

    virtual void readEntityFromNBT(NBTTagCompound compound);
};


#endif //MCPORT_ENTITYFX_H
