/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <gk/core/Box.hpp>

#include "Inventory.hpp"
#include "ISerializable.hpp"

class Player : public ISerializable {
	public:
		Player();

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		s32 x() const { return m_x; }
		s32 y() const { return m_y; }
		s32 z() const { return m_z; }

		Inventory &inventory() { return m_inventory; }

		u16 clientID() const { return m_clientID; }

		void setPosition(s32 x, s32 y, s32 z) { m_x = x; m_y = y; m_z = z; }
		void setClientID(u16 clientID) { m_clientID = clientID; }

		const gk::FloatBox &hitbox() const { return m_hitbox; }

	protected:
		s32 m_x = 0;
		s32 m_y = 0;
		s32 m_z = 0;

		u16 m_clientID = 0;

		Inventory m_inventory{9, 4};

		gk::FloatBox m_hitbox;
};

#endif // PLAYER_HPP_
