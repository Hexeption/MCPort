//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityDiggingFX.h"

#include <iostream>

EntityDiggingFX::EntityDiggingFX(World &world, double x, double y, double z, double motionX, double motionY,
                                 double motionZ, Block *block)
    : EntityFX(world, x, y, z, motionX, motionY, motionZ) {
    if (block != nullptr) {
        particleTextureIndex = block->blockIndexInTexture;
        particleGravity = block->blockParticleGravity;
    }

    particleRed = particleGreen = particleBlue = 0.6f;
    particleScale /= 2.0f;
}

int_t EntityDiggingFX::getFXLayer() {
    return 1;
}

void EntityDiggingFX::renderParticle(Tessellator &tessellator, float partialTicks, float rotationX,
                                     float rotationYZ, float rotationZ, float rotationXY, float rotationXZ) {
    const float u0 = (static_cast<float>(particleTextureIndex % 16) + particleTextureJitterX / 4.0f) / 16.0f;
    const float u1 = u0 + 0.999f / 64.0f;
    const float v0 = (static_cast<float>(particleTextureIndex / 16) + particleTextureJitterY / 4.0f) / 16.0f;
    const float v1 = v0 + 0.999f / 64.0f;
    const float scale = 0.1f * particleScale;

    const float px = static_cast<float>(prevPosX + (posX - prevPosX) * static_cast<double>(partialTicks));
    const float py = static_cast<float>(prevPosY + (posY - prevPosY) * static_cast<double>(partialTicks));
    const float pz = static_cast<float>(prevPosZ + (posZ - prevPosZ) * static_cast<double>(partialTicks));
    const float brightness = getBrightness(partialTicks);

    tessellator.setColorOpaque_F(brightness * particleRed, brightness * particleGreen, brightness * particleBlue);
    tessellator.addVertexWithUV(px - rotationX * scale - rotationXY * scale, py - rotationYZ * scale,
                                pz - rotationZ * scale - rotationXZ * scale, u0, v1);
    tessellator.addVertexWithUV(px - rotationX * scale + rotationXY * scale, py + rotationYZ * scale,
                                pz - rotationZ * scale + rotationXZ * scale, u0, v0);
    tessellator.addVertexWithUV(px + rotationX * scale + rotationXY * scale, py + rotationYZ * scale,
                                pz + rotationZ * scale + rotationXZ * scale, u1, v0);
    tessellator.addVertexWithUV(px + rotationX * scale - rotationXY * scale, py - rotationYZ * scale,
                                pz + rotationZ * scale - rotationXZ * scale, u1, v1);
}
