//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_BLOCKFLUID_H
#define MCPORT_BLOCKFLUID_H

#include "Block.h"

class Entity;
class Vec3D;

class BlockFluid : public Block {
public:
    BlockFluid(int_t blockID, Material *material);

    static float getFluidHeightPercent(int_t metadata);
    static double getFlowDirection(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, Material *material);

    int_t getRenderType() const override;

    int_t getBlockTextureFromSide(int_t side) const override;

    int_t getBlockTextureFromSideAndMetadata(int_t side, int_t metadata) const override;

    bool shouldSideBeRendered(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const override;

    bool canCollideCheck(int_t metadata, bool ignoreLiquid) const override;

    float getBlockBrightness(IBlockAccess &blockAccess, int_t x, int_t y, int_t z) const override;

    int_t getRenderBlockPass() const override;

    void velocityToAddToEntity(World &world, int_t x, int_t y, int_t z, Entity &entity,
                               Vec3D &velocity) const override;

    int_t tickRate() const override;

protected:
    int_t fluidType = 1;
};

#endif //MCPORT_BLOCKFLUID_H
