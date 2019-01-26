/*
 * =====================================================================================
 *
 *       Filename:  Inventory.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:03:48
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Inventory.hpp"
#include "Network.hpp"

void Inventory::setStack(u16 x, u16 y, const std::string &name, u16 amount) {
	m_items.at(x + y * m_width) = ItemStack(name, amount);
	m_hasChanged = true;
}

void Inventory::addStack(const std::string &name, u16 amount) {
	for (std::size_t i = 0 ; i < m_items.size() ; ++i) {
		if (m_items[i].item().id() == 0) {
			m_items[i] = ItemStack(name, amount);
			m_hasChanged = true;
			break;
		}
		else if (m_items[i].item().name() == name) {
			m_items[i] = ItemStack(name, m_items[i].amount() + amount);
			m_hasChanged = true;
			break;
		}
	}
}

sf::Packet &operator<<(sf::Packet &packet, Inventory &inventory) {
	packet << u8(inventory.inBlock())
		<< s32(inventory.blockPos().x) << s32(inventory.blockPos().y) << s32(inventory.blockPos().z);

	int i = 0;
	for (auto &it : inventory.items()) {
		packet << it.item().name() << it.amount()
			<< u8(i % inventory.width()) << u8(i / inventory.width());
		++i;
	}
	return packet;
}

sf::Packet &operator>>(sf::Packet &packet, Inventory &inventory) {
	u8 inBlock;
	s32 bx, by, bz;
	packet >> inBlock >> bx >> by >> bz;
	inventory.setInBlock(inBlock);
	inventory.setBlockPos({bx, by, bz});

	std::string name;
	u16 amount;
	u8 x, y;
	while (!packet.endOfPacket()) {
		packet >> name >> amount >> x >> y;
		inventory.setStack(x, y, name, amount);
	}
	return packet;
}

