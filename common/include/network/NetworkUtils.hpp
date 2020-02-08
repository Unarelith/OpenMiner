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
#ifndef NETWORKUTILS_HPP_
#define NETWORKUTILS_HPP_

#include <vector>

#include <SFML/Network/Packet.hpp>

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

#endif // NETWORKUTILS_HPP_
