/*
 * =====================================================================================
 *
 *       Filename:  Player.cpp
 *
 *    Description:
 *
 *        Created:  22/01/2019 23:46:58
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Network.hpp"
#include "Player.hpp"

// FIXME: The following code is a patch for block GUI and will probably be removed

void Player::serialize(sf::Packet &packet) {
	int i = 0;
	for (auto &it : m_inventory.items()) {
		packet << it.item().name() << it.amount()
			<< u8(i % m_inventory.width()) << u8(i / m_inventory.width());
		++i;
	}
}

void Player::deserialize(sf::Packet &packet) {
	std::string name;
	u16 amount;
	u8 x, y;
	while (!packet.endOfPacket()) {
		packet >> name >> amount >> x >> y;
		m_inventory.setStack(x, y, name, amount);
	}
}

