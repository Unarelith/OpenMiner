/*
 * =====================================================================================
 *
 *       Filename:  Block.cpp
 *
 *    Description:
 *
 *        Created:  29/12/2014 04:56:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <iostream>

#include "Block.hpp"
#include "Player.hpp"
#include "World.hpp"

Block::Block(u32 id, u32 textureID, const std::string &name, const std::string &label) {
	m_id = id;
	m_textureID = textureID;

	m_name = name;
	m_label = label;

	m_itemDrop = name;
	m_itemDropAmount = 1;
}

void Block::onTick(const glm::ivec3 &pos, Player &player, Chunk &chunk, World &world) const {
	try {
		if (m_onTick && m_onTickEnabled) {
			m_onTick(pos, player, chunk, world);
		}
	}
	catch (const sol::error &e) {
		m_onTickEnabled = false;
		std::cerr << e.what() << std::endl;
		std::cerr << "Block stopped ticking at (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	}
}

bool Block::onBlockActivated(const glm::ivec3 &pos, Player &player, World &world) const {
	try {
		if (m_onBlockActivated) {
			m_onBlockActivated(pos, player, world);
			return true;
		}
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}

	return false;
}

glm::vec4 Block::getTexCoords(int face, u16 blockData) const {
	// 0 -> right
	// 1 -> left
	// 2 -> bottom
	// 3 -> top
	// 4 -> back
	// 5 -> front

	u32 textureID = m_textureID;

	// Grass blocks has dirt sides and bottom
	if(m_id == 3) {
		if (face == 2) textureID = 37;
		if (face == 0 || face == 1 || face == 4 || face == 5) textureID = 224;
	}

	// Wood blocks have rings on top and bottom
	if (m_id == 5 && (face == 3 || face == 2))
		textureID = 278;

	// Workbench
	if (m_id == BlockType::Workbench) {
		if (face == 0 || face == 5) textureID = m_textureID + 1;
		if (face == 3 || face == 2) textureID = m_textureID + 2;
	}

	// Furnace
	if (m_id == BlockType::Furnace) {
		if (face == 1 || face == 4 || face == 5) textureID = 166;
		if (face == 3 || face == 2) textureID = 167;

		if (face == 0 && blockData)
			textureID = 165;
	}

	return getTexCoordsFromID(textureID);
}

glm::vec4 Block::getTexCoordsFromID(int textureID) const {
	// FIXME: HARDCODED VALUES
	const u16 textureWidth = 256;
	const u16 textureHeight = 464;

	float textureX = textureID % (textureWidth / 16) * 16.0f / textureWidth;
	float textureY = textureID / (textureWidth / 16) * 16.0f / textureHeight;

	return glm::vec4(textureX,
	                 textureY,
	                 textureX + 16.0f / textureWidth,
	                 textureY + 16.0f / textureHeight);
}

