/*
 * =====================================================================================
 *
 *       Filename:  Block.hpp
 *
 *    Description:
 *
 *        Created:  29/12/2014 04:56:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <glm/glm.hpp>

#include "Types.hpp"

class Chunk;

class Block {
	public:
		Block(u32 id);

		bool onClickEvent(Chunk *chunk);

		glm::vec4 getTexCoords(int face) const;

		u32 id() const { return m_id; }
		void setId(u32 id) { m_id = id; }

		s8 selectedFace() const { return m_selectedFace; }
		void setSelected(bool isSelected, s8 face) { m_isSelected = isSelected; m_selectedFace = face; }

		bool isOpaque() const { return m_id != 0 && m_id != 8 && m_id != 9; }

	private:
		u32 m_id;

		bool m_isSelected = false;
		s8 m_selectedFace = -1;
};

#endif // BLOCK_HPP_
