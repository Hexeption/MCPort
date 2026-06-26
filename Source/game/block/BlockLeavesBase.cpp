//
// Created by Keir Davis on 26/06/2026.
//

#include "BlockLeavesBase.h"

#include "game/world/IBlockAccess.h"

BlockLeavesBase::BlockLeavesBase(int_t blockID, int_t blockIndexInTexture, Material *material,
                                 bool graphicsLevel) : Block(blockID, blockIndexInTexture, material),
                                                       graphicsLevel(graphicsLevel) {
}

bool BlockLeavesBase::isOpaqueCube() const {
	return false;
}

bool BlockLeavesBase::shouldSideBeRendered(IBlockAccess &blockAccess, int_t x, int_t y, int_t z, int_t side) const {
	const int_t blockId = blockAccess.getBlockId(x, y, z);

	if (!graphicsLevel && blockId == blockID) {
		return false;
	}

	return Block::shouldSideBeRendered(blockAccess, x, y, z, side);
}
