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
	m_hitbox = FloatBox{-0.3125, -0.3125, 0, 0.625, 0.625, 1.75};
}

// Note: This function returns an angle4
u8 Player::getDirection() const {
	return int(floorf(m_viewAngleH / 90.f + 0.5f)) & 3;
}

// Note: This function returns an angle4
u8 Player::getOppositeDirection() const {
	return getDirection() ^ 2;
}

Vector3i Player::getCurrentChunk() const {
	return {
		(static_cast<s32>(m_x) & -CHUNK_WIDTH)  / CHUNK_WIDTH,
		(static_cast<s32>(m_y) & -CHUNK_DEPTH)  / CHUNK_DEPTH,
		(static_cast<s32>(m_z) & -CHUNK_HEIGHT) / CHUNK_HEIGHT
	};
}

void Player::serialize(sf::Packet &packet) const {
	packet << m_x << m_y << m_z << m_dimension << m_viewAngleH << m_viewAngleV << m_viewAngleRoll << m_inventory << m_heldItemSlot;
}

void Player::deserialize(sf::Packet &packet) {
	packet >> m_x >> m_y >> m_z >> m_dimension >> m_viewAngleH >> m_viewAngleV >> m_viewAngleRoll >> m_inventory >> m_heldItemSlot;
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Player::initUsertype(sol::state &lua) {
	lua.new_usertype<Player>("Player",
		"name", &Player::name,

		"x", &Player::x,
		"y", &Player::y,
		"z", &Player::z,
		"set_position", &Player::setPosition,

		"dimension", &Player::dimension,
		"set_dimension", &Player::setDimension,

		"client_id", &Player::clientID,

		"inventory", &Player::inventory,

		"held_item_stack", &Player::heldItemStack
	);
}
