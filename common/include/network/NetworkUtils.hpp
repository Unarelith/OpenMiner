/*
 * =====================================================================================
 *
 *       Filename:  NetworkUtils.hpp
 *
 *    Description:
 *
 *        Created:  31/01/2020 14:41:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
