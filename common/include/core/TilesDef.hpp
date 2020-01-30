/*
 * =====================================================================================
 *
 *       Filename:  TilesDef.hpp
 *
 *    Description:
 *
 *        Created:  30/01/2020 16:37:29
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TILESDEF_HPP_
#define TILESDEF_HPP_

#include <string>
#include <vector>

#include <SFML/Network/Packet.hpp>

#include <gk/core/IntTypes.hpp>

#include "ISerializable.hpp"

struct TilesDef : public ISerializable {
	TilesDef() = default;
	TilesDef(const std::vector<std::string> &textureFilenames_)
		: textureFilenames(textureFilenames_) {}

	const std::string &get(u8 id) const { return textureFilenames.at(id); }

	void serialize(sf::Packet &packet) override {
		packet << (u8)textureFilenames.size();
		for (std::string &it : textureFilenames)
			packet << it;
	}

	void deserialize(sf::Packet &packet) override {
		u8 textureFilenamesSize = 0;
		packet >> textureFilenamesSize;
		for (u8 i = 0 ; i < textureFilenamesSize ; ++i) {
			std::string str;
			packet >> str;
			textureFilenames.emplace_back(str);
		}
	}

	std::vector<std::string> textureFilenames;
};

#endif // TILESDEF_HPP_
