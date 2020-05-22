/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "Network.hpp"
#include "Player.hpp"

Player::Player() {
	m_hitbox = gk::FloatBox{-0.3125, -0.3125, 0, 0.625, 0.625, 1.75};
}

void Player::serialize(sf::Packet &packet) const {
	packet << m_x << m_y << m_z << m_dimension << m_inventory;
}

void Player::deserialize(sf::Packet &packet) {
	packet >> m_x >> m_y >> m_z >> m_dimension >> m_inventory;
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Player::initUsertype(sol::state &lua) {
	lua.new_usertype<Player>("Player",
		"inventory", &Player::inventory,

		"x", &Player::x,
		"y", &Player::y,
		"z", &Player::z,
		"set_position", &Player::setPosition,

		"dimension", &Player::dimension,
		"set_dimension", &Player::setDimension,

		"client_id", &Player::clientID
	);
}

