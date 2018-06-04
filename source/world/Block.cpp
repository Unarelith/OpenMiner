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

Block::Block(u16 id) {
	m_id = id;
}

Block::~Block() {
}

glm::vec4 Block::getTexCoords() {
	u16 textureWidth = 256;
	u16 textureHeight = 256;

	float textureX = m_id % (textureWidth / 16) * 16.0f / textureWidth;
	float textureY = m_id / (textureWidth / 16) * 16.0f / textureHeight;

	return glm::vec4(textureX,
	                 textureY,
	                 textureX + 16.0f / textureWidth,
					 textureY + 16.0f / textureHeight);
}

