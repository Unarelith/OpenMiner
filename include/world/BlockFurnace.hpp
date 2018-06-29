/*
 * =====================================================================================
 *
 *       Filename:  BlockFurnace.hpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 09:59:24
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKFURNACE_HPP_
#define BLOCKFURNACE_HPP_

#include "Block.hpp"

class BlockFurnace : public Block {
	public:
		BlockFurnace();

		bool onBlockActivated(const glm::ivec3 &blockPosition, Player &player, World &world) const override;
		void onTick(const glm::ivec3 &blockPosition, Player &player, Chunk &chunk, World &world) const override;
};

#endif // BLOCKFURNACE_HPP_
