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
#include "Block.hpp"

Block::Block(u32 id) {
	m_id = id;
}

#include "Chunk.hpp"

glm::vec4 Block::getTexCoords(int face) const {
	u32 id = m_id;

	// 0 -> right
	// 1 -> left
	// 2 -> bottom
	// 3 -> top
	// 4 -> back
	// 5 -> front

	// Grass blocks has dirt sides and bottom
	if(id == 3) {
		if (face == 2) id = 1;
		if (face == 0 || face == 1 || face == 4 || face == 5) id = 2;
	}

	// Wood blocks have rings on top and bottom
	if (id == 5 && (face == 3 || face == 2))
		id = 12;

	// FIXME: HARDCODED VALUES
	const u16 textureWidth = 256;
	const u16 textureHeight = 32;

	float textureX = id % (textureWidth / 16) * 16.0f / textureWidth;
	float textureY = id / (textureWidth / 16) * 16.0f / textureHeight;

	return glm::vec4(textureX,
	                 textureY,
	                 textureX + 16.0f / textureWidth,
	                 textureY + 16.0f / textureHeight);
}

