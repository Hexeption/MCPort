//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_EFFECTRENDERER_H
#define MCPORT_EFFECTRENDERER_H
#include "RenderEngine.h"
#include "game/block/Block.h"
#include "../../entity/fx/EntityFX.h"
#include "game/world/World.h"


class EffectRenderer {
protected:
    World *worldObj = nullptr;

private:
    std::array<std::vector<std::unique_ptr<EntityFX> >, 4> fxLayers;
    RenderEngine &renderEngine;
    Random rand = Random();

public:
    EffectRenderer(World *world, RenderEngine &renderEngine);

    EffectRenderer(World &world, RenderEngine &renderEngine);

    void addEffect(std::unique_ptr<EntityFX> effect);

    void addEffect(EntityFX *effect);

    void updateEffects();

    void renderParticles(Entity &entity, float partialTicks);

    void renderLitParticles(Entity &entity, float partialTicks);

    void clearEffects(World *world);

    void clearEffects(World &world);

    void addBlockDestroyEffects(int_t x, int_t y, int_t z);

    void addBlockHitEffects(int_t x, int_t y, int_t z, int_t face);

    jstring getStatistics() const;
};


#endif //MCPORT_EFFECTRENDERER_H
