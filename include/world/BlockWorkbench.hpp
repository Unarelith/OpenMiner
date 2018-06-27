/*
 * =====================================================================================
 *
 *       Filename:  BlockWorkbench.hpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 04:43:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKWORKBENCH_HPP_
#define BLOCKWORKBENCH_HPP_

#include "Block.hpp"

class BlockWorkbench : public Block {
	public:
		BlockWorkbench();

		bool onBlockActivated(const glm::ivec3 &position, Player &player, World &world) const override;
};

#endif // BLOCKWORKBENCH_HPP_
