//
// Created by Keir Davis on 25/06/2026.
//

#include "EffectRenderer.h"

#include "game/client/MathHelper.h"
#include "game/entity/fx/EntityDiggingFX.h"
#include "glad/glad.h"

EffectRenderer::EffectRenderer(World *world, RenderEngine &renderEngine)
    : worldObj(world), renderEngine(renderEngine) {
}

EffectRenderer::EffectRenderer(World &world, RenderEngine &renderEngine)
    : EffectRenderer(&world, renderEngine) {
}

void EffectRenderer::addEffect(std::unique_ptr<EntityFX> effect) {
    if (effect == nullptr) {
        return;
    }

    const int_t layer = effect->getFXLayer();
    if (layer < 0 || layer >= static_cast<int_t>(fxLayers.size())) {
        return;
    }

    fxLayers[layer].push_back(std::move(effect));
}

void EffectRenderer::addEffect(EntityFX *effect) {
    addEffect(std::unique_ptr<EntityFX>(effect));
}

void EffectRenderer::updateEffects() {
    for (auto &layer: fxLayers) {
        for (auto it = layer.begin(); it != layer.end();) {
            (*it)->onUpdate();
            if ((*it)->isDead) {
                it = layer.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void EffectRenderer::renderParticles(Entity &entity, float partialTicks) {
    constexpr float degreesToRadians = 3.14159265358979323846f / 180.0f;

    const float cosYaw = MathHelper::cos(entity.rotationYaw * degreesToRadians);
    const float sinYaw = MathHelper::sin(entity.rotationYaw * degreesToRadians);
    const float rotationXY = -sinYaw * MathHelper::sin(entity.rotationPitch * degreesToRadians);
    const float rotationXZ = cosYaw * MathHelper::sin(entity.rotationPitch * degreesToRadians);
    const float cosPitch = MathHelper::cos(entity.rotationPitch * degreesToRadians);

    EntityFX::interpPosX = entity.lastTickPosX + (entity.posX - entity.lastTickPosX) * static_cast<double>(
                               partialTicks);
    EntityFX::interpPosY = entity.lastTickPosY + (entity.posY - entity.lastTickPosY) * static_cast<double>(
                               partialTicks);
    EntityFX::interpPosZ = entity.lastTickPosZ + (entity.posZ - entity.lastTickPosZ) * static_cast<double>(
                               partialTicks);

    for (int_t layer = 0; layer < 3; ++layer) {
        if (fxLayers[layer].empty()) {
            continue;
        }

        int_t texture = 0;
        if (layer == 0) {
            texture = renderEngine.getTexture(u"/particles.png");
        } else if (layer == 1) {
            texture = renderEngine.getTexture(u"/terrain.png");
        } else if (layer == 2) {
            texture = renderEngine.getTexture(u"/gui/items.png");
        }

        glBindTexture(GL_TEXTURE_2D, texture);
        Tessellator &tessellator = Tessellator::instance;
        tessellator.startDrawingQuads();

        for (const auto &effect: fxLayers[layer]) {
            effect->renderParticle(tessellator, partialTicks, cosYaw, cosPitch, sinYaw, rotationXY, rotationXZ);
        }

        tessellator.draw();
    }
}

void EffectRenderer::renderLitParticles(Entity &entity, float partialTicks) {
    constexpr int_t layer = 3;
    if (fxLayers[layer].empty()) {
        return;
    }

    Tessellator &tessellator = Tessellator::instance;
    for (const auto &effect: fxLayers[layer]) {
        effect->renderParticle(tessellator, partialTicks, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void EffectRenderer::clearEffects(World *world) {
    worldObj = world;
    for (auto &layer: fxLayers) {
        layer.clear();
    }
}

void EffectRenderer::clearEffects(World &world) {
    clearEffects(&world);
}

void EffectRenderer::addBlockDestroyEffects(int_t x, int_t y, int_t z) {
    if (worldObj == nullptr) {
        return;
    }

    const int_t blockId = worldObj->getBlockId(x, y, z);
    if (blockId == 0) {
        return;
    }

    Block *block = Block::blocksList[blockId];
    constexpr int_t count = 4;

    for (int_t dx = 0; dx < count; ++dx) {
        for (int_t dy = 0; dy < count; ++dy) {
            for (int_t dz = 0; dz < count; ++dz) {
                const double px = static_cast<double>(x) + (static_cast<double>(dx) + 0.5) / static_cast<double>(count);
                const double py = static_cast<double>(y) + (static_cast<double>(dy) + 0.5) / static_cast<double>(count);
                const double pz = static_cast<double>(z) + (static_cast<double>(dz) + 0.5) / static_cast<double>(count);

                addEffect(new EntityDiggingFX(*worldObj, px, py, pz, px - static_cast<double>(x) - 0.5,
                                              py - static_cast<double>(y) - 0.5, pz - static_cast<double>(z) - 0.5,
                                              block));
            }
        }
    }
}

void EffectRenderer::addBlockHitEffects(int_t x, int_t y, int_t z, int_t face) {
    if (worldObj == nullptr) {
        return;
    }

    const int_t blockId = worldObj->getBlockId(x, y, z);
    if (blockId == 0) {
        return;
    }

    Block *block = Block::blocksList[blockId];
    constexpr float offset = 0.1f;

    double px = static_cast<double>(x) + rand.nextDouble() * (
                    block->maxX - block->minX - static_cast<double>(offset * 2.0f)) + static_cast<double>(offset) +
                block->minX;
    double py = static_cast<double>(y) + rand.nextDouble() * (
                    block->maxY - block->minY - static_cast<double>(offset * 2.0f)) + static_cast<double>(offset) +
                block->minY;
    double pz = static_cast<double>(z) + rand.nextDouble() * (
                    block->maxZ - block->minZ - static_cast<double>(offset * 2.0f)) + static_cast<double>(offset) +
                block->minZ;

    if (face == 0) {
        py = static_cast<double>(y) + block->minY - static_cast<double>(offset);
    } else if (face == 1) {
        py = static_cast<double>(y) + block->maxY + static_cast<double>(offset);
    } else if (face == 2) {
        pz = static_cast<double>(z) + block->minZ - static_cast<double>(offset);
    } else if (face == 3) {
        pz = static_cast<double>(z) + block->maxZ + static_cast<double>(offset);
    } else if (face == 4) {
        px = static_cast<double>(x) + block->minX - static_cast<double>(offset);
    } else if (face == 5) {
        px = static_cast<double>(x) + block->maxX + static_cast<double>(offset);
    }

    auto effect = std::make_unique<EntityDiggingFX>(*worldObj, px, py, pz, 0.0, 0.0, 0.0, block);
    effect->multiplyVelocity(0.2f).multipleParticleScaleBy(0.6f);
    addEffect(std::move(effect));
}

jstring EffectRenderer::getStatistics() const {
    return String::toString(static_cast<ulong_t>(fxLayers[0].size() + fxLayers[1].size() + fxLayers[2].size()));
}
