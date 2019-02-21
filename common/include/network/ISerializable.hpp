/*
 * =====================================================================================
 *
 *       Filename:  ISerializable.hpp
 *
 *    Description:
 *
 *        Created:  22/01/2019 23:49:32
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ISERIALIZABLE_HPP_
#define ISERIALIZABLE_HPP_

namespace sf { class Packet; }

class ISerializable {
	public:
		virtual ~ISerializable() = default;

		virtual void serialize(sf::Packet &packet) = 0;
		virtual void deserialize(sf::Packet &packet) = 0;
};

#endif // ISERIALIZABLE_HPP_
