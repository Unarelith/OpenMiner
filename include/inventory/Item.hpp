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

#include <string>

#include "IntTypes.hpp"
#include "ItemType.hpp"

class Item {
	public:
		Item(u32 id, u32 textureID, const std::string &name);

		const std::string &name() const { return m_name; }

		u32 id() const { return m_id; }
		u32 textureID() const { return m_textureID; }

		bool isBlock() const { return m_isBlock; }

	protected:
		bool m_isBlock = false;

	private:
		std::string m_name;

		u32 m_id = 0;
		u32 m_textureID = 0;
};

#endif // ITEM_HPP_
