/*
 * =====================================================================================
 *
 *       Filename:  BlockWater.hpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 23:19:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKWATER_HPP_
#define BLOCKWATER_HPP_

#include "Block.hpp"

class BlockWater : public Block {
	public:
		BlockWater();

		void onNeighbourUpdate(const glm::ivec3 &blockPosition, const glm::ivec3 &thisBlockPosition, Chunk &chunk) const override;
		void onTick(const glm::ivec3 &blockPosition, Player &player, Chunk &chunk, World &world) const override;
};

#endif // BLOCKWATER_HPP_
