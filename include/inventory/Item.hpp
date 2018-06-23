/*
 * =====================================================================================
 *
 *       Filename:  Item.hpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:29:41
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "Types.hpp"

class Item {
	public:
		Item(u32 id);

		u32 id() const { return m_id; }

	private:
		u32 m_id = 0;
};

#endif // ITEM_HPP_
