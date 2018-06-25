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

void BlockWater::onBlockDestroy(const glm::ivec3 &blockPosition, const glm::ivec3 &thisBlockPosition, World &world) const {
	glm::ivec3 left{thisBlockPosition.x - 1, thisBlockPosition.y, thisBlockPosition.z};
	glm::ivec3 right{thisBlockPosition.x + 1, thisBlockPosition.y, thisBlockPosition.z};
	glm::ivec3 front{thisBlockPosition.x, thisBlockPosition.y, thisBlockPosition.z - 1};
	glm::ivec3 back{thisBlockPosition.x, thisBlockPosition.y, thisBlockPosition.z + 1};

	if (blockPosition == left || blockPosition == right || blockPosition == front || blockPosition == back)
		world.setBlock(blockPosition.x, blockPosition.y, blockPosition.z, 8);
}

void BlockWater::onTick(const glm::ivec3 &blockPosition, World &world) const {
	if (world.getBlock(blockPosition.x - 1, blockPosition.y, blockPosition.z) == 0) {
		world.setBlock(blockPosition.x - 1, blockPosition.y, blockPosition.z, 8);
	}
	if (world.getBlock(blockPosition.x, blockPosition.y - 1, blockPosition.z) == 0) {
		world.setBlock(blockPosition.x, blockPosition.y - 1, blockPosition.z, 8);
	}
	if (world.getBlock(blockPosition.x, blockPosition.y, blockPosition.z - 1) == 0) {
		world.setBlock(blockPosition.x, blockPosition.y, blockPosition.z - 1, 8);
	}
	if (world.getBlock(blockPosition.x + 1, blockPosition.y, blockPosition.z) == 0) {
		world.setBlock(blockPosition.x + 1, blockPosition.y, blockPosition.z, 8);
	}
	if (world.getBlock(blockPosition.x, blockPosition.y, blockPosition.z + 1) == 0) {
		world.setBlock(blockPosition.x, blockPosition.y, blockPosition.z + 1, 8);
	}
}

