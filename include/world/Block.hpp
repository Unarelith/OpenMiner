/*
 * =====================================================================================
 *
 *       Filename:  Block.hpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  29/12/2014 04:56:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <glm/glm.hpp>

#include "Types.hpp"

class Block {
	public:
		Block(u32 id);

		glm::vec4 getTexCoords();

		u32 id() const { return m_id; }

	private:
		u32 m_id;
};

#endif // BLOCK_HPP_
