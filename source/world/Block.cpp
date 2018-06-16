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

Block::Block(const glm::vec3 &pos, u32 id) : m_pos(pos) {
	m_id = id;
}

glm::vec4 Block::getTexCoords() {
	// FIXME: HARDCODED VALUES
	const u16 textureWidth = 256;
	const u16 textureHeight = 16;

	float textureX = m_id % (textureWidth / 16) * 16.0f / textureWidth;
	float textureY = m_id / (textureWidth / 16) * 16.0f / textureHeight;

	return glm::vec4(textureX,
	                 textureY,
	                 textureX + 16.0f / textureWidth,
	                 textureY + 16.0f / textureHeight);
}

