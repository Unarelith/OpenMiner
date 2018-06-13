/*
 * =====================================================================================
 *
 *       Filename:  Block.hpp
 *
 *    Description:
 *
 *        Created:  29/12/2014 04:56:02
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <glm/glm.hpp>

#include "Types.hpp"

class Block {
	public:
		Block(const glm::vec3 &pos, u32 id);

		glm::vec4 getTexCoords();

		const glm::vec3 &pos() const { return m_pos; }

		u32 id() const { return m_id; }

	private:
		glm::vec3 m_pos;

		u32 m_id;
};

#endif // BLOCK_HPP_
