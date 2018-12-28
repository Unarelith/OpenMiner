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
#include "Box.hpp"
#include "IntTypes.hpp"
#include "ItemStack.hpp"

class Chunk;
class Player;
class World;

class Block {
	public:
		Block(u32 id, u32 textureID, const std::string &name, const std::string &label);
		virtual ~Block() = default;

		virtual void onTick(const glm::ivec3 &, Player &, Chunk &, World &) const {}
		virtual bool onBlockActivated(const glm::ivec3 &, Player &, World &) const { return false; }
		virtual void onNeighbourUpdate(const glm::ivec3 &, const glm::ivec3 &, Chunk &) const {}

		virtual glm::vec4 getTexCoords(int face, u16 blockData) const;

		u16 id() const { return m_id & 0xffff; }
		u16 data() const { return (m_id >> 16) & 0xffff; }
		u32 textureID() const { return m_textureID; }

		const std::string &name() const { return m_name; }
		const std::string &label() const { return m_label; }
		void setLabel(const std::string &label) { m_label = label; }

		s8 selectedFace() const { return m_selectedFace; }
		void setSelected(bool isSelected, s8 face) { m_isSelected = isSelected; m_selectedFace = face; }

		bool isOpaque() const { return m_id != 0 && m_id != 4 && m_id != 8 && m_id != 9 && m_id != 16; }

		bool canUpdate() const { return m_canUpdate; }

		ItemStack getItemDrop() const { return ItemStack{m_itemDrop, m_itemDropAmount}; };
		void setItemDrop(u16 itemDrop, u16 itemDropAmount = 1) { m_itemDrop = itemDrop; m_itemDropAmount = itemDropAmount; }

		u8 harvestRequirements() const { return m_harvestRequirements; }
		void setHarvestRequirements(u8 harvestRequirements) { m_harvestRequirements = harvestRequirements; }

		float hardness() const { return m_hardness; }
		void setHardness(float hardness) { m_hardness = hardness; }

		float timeToBreak(u8 harvestCapability, float miningSpeed) const {
			return ((m_harvestRequirements & harvestCapability) == m_harvestRequirements) ? 1.5 * m_hardness / miningSpeed : 5 * m_hardness;
		}

		const FloatBox &boundingBox() const { return m_boundingBox; }
		void setBoundingBox(const FloatBox &boundingBox) { m_boundingBox = boundingBox; }

	protected:
		glm::vec4 getTexCoordsFromID(int textureID) const;

		bool m_canUpdate = false;

	private:
		u32 m_id;
		u32 m_textureID;

		std::string m_name;
		std::string m_label;

		bool m_isSelected = false;
		s8 m_selectedFace = -1;

		u16 m_itemDrop;
		u16 m_itemDropAmount;

		u8 m_harvestRequirements = 0;
		float m_hardness = 1.0f;

		FloatBox m_boundingBox{0, 0, 0, 1, 1, 1};
};

#endif // BLOCK_HPP_
