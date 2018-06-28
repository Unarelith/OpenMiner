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

#include "BlockType.hpp"
#include "IntTypes.hpp"

class Chunk;
class Player;
class World;

class Block {
	public:
		Block(u32 id, u32 textureID);
		virtual ~Block() = default;

		virtual void onTick(const glm::ivec3 &, Player &, World &) const {}
		virtual bool onBlockActivated(const glm::ivec3 &, Player &, World &) const { return false; }
		virtual void onBlockDestroy(const glm::ivec3 &, const glm::ivec3 &, Player &, World &) const {}

		virtual glm::vec4 getTexCoords(int face) const;

		u16 id() const { return m_id & 0xffff; }
		u16 data() const { return (m_id >> 16) & 0xffff; }
		u32 textureID() const { return m_textureID; }

		s8 selectedFace() const { return m_selectedFace; }
		void setSelected(bool isSelected, s8 face) { m_isSelected = isSelected; m_selectedFace = face; }

		bool isOpaque() const { return m_id != 0 && m_id != 4 && m_id != 8 && m_id != 9; }

		bool canUpdate() const { return m_canUpdate; }

	protected:
		bool m_canUpdate = false;

	private:
		u32 m_id;
		u32 m_textureID;

		bool m_isSelected = false;
		s8 m_selectedFace = -1;
};

#endif // BLOCK_HPP_
