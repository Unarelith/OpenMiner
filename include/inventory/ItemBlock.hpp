/*
 * =====================================================================================
 *
 *       Filename:  ItemBlock.hpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 17:28:04
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEMBLOCK_HPP_
#define ITEMBLOCK_HPP_

#include "BlockType.hpp"
#include "Item.hpp"

class ItemBlock : public Item {
	public:
		ItemBlock(u32 id, u32 blockID, const std::string &name);
};

#endif // ITEMBLOCK_HPP_
