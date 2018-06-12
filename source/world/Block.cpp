/*
 * =====================================================================================
 *
 *       Filename:  Block.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  29/12/2014 04:56:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#include "Block.hpp"

Block::Block(u32 id) {
	m_id = id;
}

glm::vec4 Block::getTexCoords() {
	const u16 textureWidth = 256;
	const u16 textureHeight = 256;

	float textureX = m_id % (textureWidth / 16) * 16.0f / textureWidth;
	float textureY = m_id / (textureWidth / 16) * 16.0f / textureHeight;

	return glm::vec4(textureX,
	                 textureY,
	                 textureX + 16.0f / textureWidth,
					 textureY + 16.0f / textureHeight);
}

