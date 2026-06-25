//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityFX.h"

#include "game/client/MathHelper.h"
#include "java/Math.h"

double EntityFX::interpPosX = 0.0;
double EntityFX::interpPosY = 0.0;
double EntityFX::interpPosZ = 0.0;

EntityFX::EntityFX(World &worldObj, double x, double y, double z, double mx, double my, double mz)
    : Entity(worldObj) {
    setSize(0.2f, 0.2f);
    yOffset = height / 2.0f;
    setPosition(x, y, z);

    particleRed = particleGreen = particleBlue = 1.0f;

    motionX = mx + static_cast<double>((Math::random() * 2.0f - 1.0f) * 0.4f);
    motionY = my + static_cast<double>((Math::random() * 2.0f - 1.0f) * 0.4f);
    motionZ = mz + static_cast<double>((Math::random() * 2.0f - 1.0f) * 0.4f);

    const float speed = MathHelper::sqrt_double(motionX * motionX + motionY * motionY + motionZ * motionZ);
    const float randomSpeed = (Math::random() + Math::random() + 1.0f) * 0.15f;

    if (speed != 0.0f) {
        motionX = motionX / static_cast<double>(speed) * static_cast<double>(randomSpeed) * 0.4;
        motionY = motionY / static_cast<double>(speed) * static_cast<double>(randomSpeed) * 0.4 + 0.1;
        motionZ = motionZ / static_cast<double>(speed) * static_cast<double>(randomSpeed) * 0.4;
    }

    particleTextureJitterX = Math::random() * 3.0f;
    particleTextureJitterY = Math::random() * 3.0f;
    particleScale = (Math::random() * 0.5f + 0.5f) * 2.0f;
    particleMaxAge = static_cast<int_t>(4.0f / (Math::random() * 0.9f + 0.1f));
    particleAge = 0;
}

EntityFX &EntityFX::multiplyVelocity(float multiplier) {
    motionX *= static_cast<double>(multiplier);
    motionY = (motionY - 0.1) * static_cast<double>(multiplier) + 0.1;
    motionZ *= static_cast<double>(multiplier);
    return *this;
}

EntityFX &EntityFX::multipleParticleScaleBy(float scale) {
    setSize(0.2f * scale, 0.2f * scale);
    particleScale *= scale;
    return *this;
}

void EntityFX::onUpdate() {
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;

    if (particleAge++ >= particleMaxAge) {
        isDead = true;
    }

    motionY -= 0.04 * static_cast<double>(particleGravity);
    moveEntity(motionX, motionY, motionZ);
    motionX *= 0.98;
    motionY *= 0.98;
    motionZ *= 0.98;

    if (onGround) {
        motionX *= 0.7;
        motionZ *= 0.7;
    }
}

void EntityFX::renderParticle(Tessellator &tessellator, float partialTicks, float rotationX, float rotationYZ,
                              float rotationZ, float rotationXY, float rotationXZ) {
    const float u0 = static_cast<float>(particleTextureIndex % 16) / 16.0f;
    const float u1 = u0 + 0.999f / 16.0f;
    const float v0 = static_cast<float>(particleTextureIndex / 16) / 16.0f;
    const float v1 = v0 + 0.999f / 16.0f;
    const float scale = 0.1f * particleScale;

    const float x = static_cast<float>(prevPosX + (posX - prevPosX) * static_cast<double>(partialTicks) - interpPosX);
    const float y = static_cast<float>(prevPosY + (posY - prevPosY) * static_cast<double>(partialTicks) - interpPosY);
    const float z = static_cast<float>(prevPosZ + (posZ - prevPosZ) * static_cast<double>(partialTicks) - interpPosZ);
    const float brightness = getBrightness(partialTicks);

    tessellator.setColorOpaque_F(particleRed * brightness, particleGreen * brightness, particleBlue * brightness);
    tessellator.addVertexWithUV(x - rotationX * scale - rotationXY * scale, y - rotationYZ * scale,
                                z - rotationZ * scale - rotationXZ * scale, u0, v1);
    tessellator.addVertexWithUV(x - rotationX * scale + rotationXY * scale, y + rotationYZ * scale,
                                z - rotationZ * scale + rotationXZ * scale, u0, v0);
    tessellator.addVertexWithUV(x + rotationX * scale + rotationXY * scale, y + rotationYZ * scale,
                                z + rotationZ * scale + rotationXZ * scale, u1, v0);
    tessellator.addVertexWithUV(x + rotationX * scale - rotationXY * scale, y - rotationYZ * scale,
                                z + rotationZ * scale - rotationXZ * scale, u1, v1);
}

int_t EntityFX::getFXLayer() {
    return 0;
}

void EntityFX::writeEntityToNBT(NBTTagCompound compound) {
}

void EntityFX::readEntityFromNBT(NBTTagCompound compound) {
}
