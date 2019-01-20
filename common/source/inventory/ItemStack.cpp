/*
 * =====================================================================================
 *
 *       Filename:  ItemStack.cpp
 *
 *    Description:
 *
 *        Created:  28/12/2018 20:00:00
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ItemStack.hpp"
#include "Registry.hpp"

const Item &ItemStack::item() const {
	return Registry::getInstance().getItem(m_name);
}

sf::Packet &operator<<(sf::Packet &packet, const ItemStack &itemStack) {
	return packet << itemStack.item().name() << itemStack.amount();
}

sf::Packet &operator>>(sf::Packet &packet, ItemStack &itemStack) {
	std::string name;
	u16 amount;
	packet >> name >> amount;
	itemStack.setItem(name);
	itemStack.setAmount(amount);
	return packet;
}

