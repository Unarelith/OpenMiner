/*
 * =====================================================================================
 *
 *       Filename:  ItemStack.hpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 01:23:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEMSTACK_HPP_
#define ITEMSTACK_HPP_

#include "Item.hpp"

namespace sf { class Packet; }

class ItemStack {
	public:
		ItemStack() = default;
		ItemStack(const std::string &name, u16 amount = 1)
			: m_name(name), m_amount(amount) {}

		const Item &item() const;
		void setItem(const std::string &name) { m_name = name; }

		u16 amount() const { return m_amount; }
		void setAmount(u16 amount) { m_amount = amount; }

	private:
		std::string m_name;

		u16 m_amount = 0;
};

sf::Packet &operator<<(sf::Packet &packet, const ItemStack &itemStack);
sf::Packet &operator>>(sf::Packet &packet, ItemStack &itemStack);

#endif // ITEMSTACK_HPP_
