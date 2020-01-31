/*
 * =====================================================================================
 *
 *       Filename:  BlockData.hpp
 *
 *    Description:
 *
 *        Created:  29/06/2018 06:52:08
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKDATA_HPP_
#define BLOCKDATA_HPP_

#include "Inventory.hpp"

struct BlockData {
	BlockData(const gk::Vector3i &pos, int width, int height) : inventory(width, height) {
		inventory.setInBlock(true);
		inventory.setBlockPos(pos);
	}

	Inventory inventory;

	u32 data = 0;

	bool useAltTiles = false;
};

#endif // BLOCKDATA_HPP_
