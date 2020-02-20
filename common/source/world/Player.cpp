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
#include "Network.hpp"
#include "Player.hpp"

Player::Player() {
	m_hitbox = gk::FloatBox{-0.2, -0.2, -1.8, 0.4, 0.4, 1.8};
}

void Player::serialize(sf::Packet &packet) const {
	packet << m_x << m_y << m_z << m_inventory;
}

void Player::deserialize(sf::Packet &packet) {
	packet >> m_x >> m_y >> m_z >> m_inventory;
}

