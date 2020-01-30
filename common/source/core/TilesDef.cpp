/*
 * =====================================================================================
 *
 *       Filename:  TilesDef.cpp
 *
 *    Description:
 *
 *        Created:  30/01/2020 17:27:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/Exception.hpp>

#include "TilesDef.hpp"

const std::string &TilesDef::getTextureForFace(u8 face) const {
	u8 size = textureFilenames.size();
	if (size == 0)
		throw EXCEPTION("Trying to get texture from empty tiles definition");

	if (size == 1)
		face = BlockFace::Top;
	else if (size == 2)
		face = (face == BlockFace::Top || face == BlockFace::Bottom) ? 0 : 1;
	else if (size == 3 && face >= size)
		face = BlockFace::Left;
	else if (size == 4 && face >= size)
		face = BlockFace::Right;
	else if (size == 5 && face >= size)
		face = BlockFace::Front;

	return textureFilenames.at(face);
}

void TilesDef::serialize(sf::Packet &packet) {
	packet << (u8)textureFilenames.size();
	for (std::string &it : textureFilenames)
		packet << it;
}

void TilesDef::deserialize(sf::Packet &packet) {
	u8 textureFilenamesSize = 0;
	packet >> textureFilenamesSize;
	for (u8 i = 0 ; i < textureFilenamesSize ; ++i) {
		std::string str;
		packet >> str;
		textureFilenames.emplace_back(str);
	}
}

