/*
 * =====================================================================================
 *
 *       Filename:  Player.hpp
 *
 *    Description:
 *
 *        Created:  22/01/2019 23:46:45
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Inventory.hpp"
#include "ISerializable.hpp"

class Player : public ISerializable {
	public:
		void serialize(sf::Packet &packet) override;
		void deserialize(sf::Packet &packet) override;

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		Inventory &inventory() { return m_inventory; }

		u16 clientID() const { return m_clientID; }

		void setPosition(s32 x, s32 y, s32 z) { m_x = x; m_y = y; m_z = z; }
		void setClientID(u16 clientID) { m_clientID = clientID; }

	protected:
		s32 m_x = 0;
		s32 m_y = 0;
		s32 m_z = 0;

		u16 m_clientID = 0;

		Inventory m_inventory{9, 4};
};

#endif // PLAYER_HPP_
