/*
 * =====================================================================================
 *
 *       Filename:  ISerializable.cpp
 *
 *    Description:
 *
 *        Created:  31/01/2020 14:36:28
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ISerializable.hpp"

sf::Packet &operator<<(sf::Packet &packet, const ISerializable &s) {
	s.serialize(packet);
	return packet;
}

sf::Packet &operator>>(sf::Packet &packet, ISerializable &s) {
	s.deserialize(packet);
	return packet;
}

