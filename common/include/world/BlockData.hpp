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
	BlockData(int width, int height) : inventory(width, height) {}

	Inventory inventory;

	u64 data = 0;
};

#endif // BLOCKDATA_HPP_
