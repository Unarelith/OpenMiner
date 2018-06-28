/*
 * =====================================================================================
 *
 *       Filename:  BlockWater.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 23:25:13
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "BlockWater.hpp"
#include "World.hpp"

BlockWater::BlockWater() : Block(BlockType::Water, 457) {
	m_canUpdate = true;
}

void BlockWater::onNeighbourUpdate(const glm::ivec3 &blockPosition, const glm::ivec3 &thisBlockPosition, Chunk &chunk) const {
	if (chunk.getBlock(blockPosition.x, blockPosition.y, blockPosition.z) == 0) {
		chunk.setBlock(blockPosition.x, blockPosition.y, blockPosition.z, 8);
	}
	else if (blockPosition == thisBlockPosition && chunk.isGenerated()) {
		if (chunk.getBlock(blockPosition.x - 1, blockPosition.y, blockPosition.z) == 0) {
			chunk.setBlock(blockPosition.x - 1, blockPosition.y, blockPosition.z, 8);
		}
		if (chunk.getBlock(blockPosition.x, blockPosition.y - 1, blockPosition.z) == 0) {
			chunk.setBlock(blockPosition.x, blockPosition.y - 1, blockPosition.z, 8);
		}
		if (chunk.getBlock(blockPosition.x, blockPosition.y, blockPosition.z - 1) == 0) {
			chunk.setBlock(blockPosition.x, blockPosition.y, blockPosition.z - 1, 8);
		}
		if (chunk.getBlock(blockPosition.x + 1, blockPosition.y, blockPosition.z) == 0) {
			chunk.setBlock(blockPosition.x + 1, blockPosition.y, blockPosition.z, 8);
		}
		if (chunk.getBlock(blockPosition.x, blockPosition.y, blockPosition.z + 1) == 0) {
			chunk.setBlock(blockPosition.x, blockPosition.y, blockPosition.z + 1, 8);
		}
	}
}

void BlockWater::onTick(const glm::ivec3 &blockPosition, Player &, Chunk &chunk, World &world) const {
	// if (world.getBlock(blockPosition.x - 1, blockPosition.y, blockPosition.z) == 0) {
	// 	world.setBlock(blockPosition.x - 1, blockPosition.y, blockPosition.z, 8);
	// }
	// if (world.getBlock(blockPosition.x, blockPosition.y - 1, blockPosition.z) == 0) {
	// 	world.setBlock(blockPosition.x, blockPosition.y - 1, blockPosition.z, 8);
	// }
	// if (world.getBlock(blockPosition.x, blockPosition.y, blockPosition.z - 1) == 0) {
	// 	world.setBlock(blockPosition.x, blockPosition.y, blockPosition.z - 1, 8);
	// }
	// if (world.getBlock(blockPosition.x + 1, blockPosition.y, blockPosition.z) == 0) {
	// 	world.setBlock(blockPosition.x + 1, blockPosition.y, blockPosition.z, 8);
	// }
	// if (world.getBlock(blockPosition.x, blockPosition.y, blockPosition.z + 1) == 0) {
	// 	world.setBlock(blockPosition.x, blockPosition.y, blockPosition.z + 1, 8);
	// }
}

