//
// Created by Keir Davis on 24/06/2026.
//

#include "Block.h"

#include "game/world/IBlockAccess.h"
#include "game/world/World.h"

struct BlockMaterialBootstrap {
    BlockMaterialBootstrap() {
        Material::init();
    }
};

static BlockMaterialBootstrap blockMaterialBootstrap;

std::array<Block *, 256> Block::blocksList = {};
std::array<int_t, 256> Block::lightOpacity = [] {
    std::array<int_t, 256> values{};
    values.fill(0);
    return values;
}();
std::array<int_t, 256> Block::lightValue = {};
Block *Block::stone = new Block(1, 1, Material::rock);
Block *Block::grass = new Block(2, 0, 2, 3, Material::grass);
Block *Block::dirt = new Block(3, 2, Material::grass);
Block *Block::sand = new Block(12, 18, Material::sand);

Block::Block(const int_t blockID, const int_t blockIndexInTexture, Material *material) : blockID(blockID),
    material(material), blockIndexInTexture(blockIndexInTexture) {
    blockTextures.fill(blockIndexInTexture);
    if (blockID >= 0 && blockID < static_cast<int_t>(blocksList.size())) {
        blocksList[blockID] = this;
        lightOpacity[blockID] = isOpaqueCube() ? 255 : 0;
    }
}

Block::Block(const int_t blockID, const int_t topTexture, const int_t bottomTexture, const int_t sideTexture,
             Material *material) : Block(blockID, sideTexture, material) {
    blockTextures[0] = bottomTexture;
    blockTextures[1] = topTexture;
}

int_t Block::getRenderType() const {
    return 0;
}

void Block::setBlockBoundsForItemRender() {
}

int_t Block::getBlockTextureFromSide(const int_t side) const {
    if (side >= 0 && side < static_cast<int_t>(blockTextures.size())) {
        return blockTextures[side];
    }
    return blockIndexInTexture;
}

bool Block::isOpaqueCube() const {
    return material != nullptr && material->isSolid();
}

float Block::getBlockBrightness(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z) const {
    return blockAccess.getBrightness(x, y, z);
}

bool Block::shouldSideBeRendered(IBlockAccess &blockAccess, const int_t x, const int_t y, const int_t z,
                                 int_t) const {
    return !blockAccess.isBlockNormalCube(x, y, z);
}

void Block::updateTick(World &world, int_t x, int_t y, int_t z, Random &random) {
}

void Block::randomDisplayTick(World &world, int_t x, int_t y, int_t z, Random &random) {
}

bool Block::canPlaceBlockAt(World &world, const int_t x, const int_t y, const int_t z) const {
    const int_t blockId = world.getBlockId(x, y, z);
    return blockId == 0 || (blockId >= 0 && blockId < static_cast<int_t>(blocksList.size()) &&
                            blocksList[blockId] != nullptr && blocksList[blockId]->material != nullptr &&
                            blocksList[blockId]->material->getIsLiquid());
}

int_t Block::tickRate() const {
    return 10;
}
