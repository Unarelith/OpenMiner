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
#ifndef NETWORKUTILS_HPP_
#define NETWORKUTILS_HPP_

#include <SFML/Network/Packet.hpp>

//======================================================================================
// std::vector
//======================================================================================
#include <vector>

template<typename T>
sf::Packet &operator<<(sf::Packet &packet, const std::vector<T> &vec) {
	packet << (unsigned int)vec.size();
	for (auto &it : vec)
		packet << it;
	return packet;
}

template<typename T>
sf::Packet &operator>>(sf::Packet &packet, std::vector<T> &vec) {
	unsigned int size;
	packet >> size;

	for (unsigned int i = 0 ; i < size ; ++i) {
		T v;
		packet >> v;
		vec.emplace_back(v);
	}

	return packet;
}

//======================================================================================
// gk::Rect
//======================================================================================
#include <gk/core/Rect.hpp>

template<typename T>
sf::Packet &operator<<(sf::Packet &packet, const gk::Rect<T> &rect) {
	packet << rect.x << rect.y << rect.sizeX << rect.sizeY;
	return packet;
}

template<typename T>
sf::Packet &operator>>(sf::Packet &packet, gk::Rect<T> &rect) {
	packet >> rect.x >> rect.y >> rect.sizeX >> rect.sizeY;
	return packet;
}

//======================================================================================
// gk::Vector3
//======================================================================================
#include <gk/core/Vector3.hpp>

template<typename T>
sf::Packet &operator<<(sf::Packet &packet, const gk::Vector3<T> &vec) {
	packet << vec.x << vec.y << vec.z;
	return packet;
}

template<typename T>
sf::Packet &operator>>(sf::Packet &packet, gk::Vector3<T> &vec) {
	packet >> vec.x >> vec.y >> vec.z;
	return packet;
}

#endif // NETWORKUTILS_HPP_
