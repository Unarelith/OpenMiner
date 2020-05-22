/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <glm/glm.hpp>

#include <sol/sol.hpp>

#include <gk/core/Box.hpp>
#include <gk/core/IntTypes.hpp>
#include <gk/graphics/Color.hpp>

#include "ItemStack.hpp"
#include "ISerializable.hpp"
#include "TilesDef.hpp"

class Chunk;
class Player;
class World;

enum class BlockDrawType : u8 {
	Solid   = 0,
	XShape  = 1,
	Leaves  = 2,
	Liquid  = 3,
	Glass   = 4,
	Cactus  = 5,
	BoundingBox = 6, // FIXME: Temporary
};

class Block : public ISerializable {
	public:
		Block() = default;
		Block(u32 id, const TilesDef &tiles, const std::string &stringID, const std::string &label);
		virtual ~Block() = default;

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		u16 id() const { return m_id & 0xffff; }
		u16 data() const { return (m_id >> 16) & 0xffff; }
		const TilesDef &tiles() const { return m_tiles; }

		const std::string &stringID() const { return m_stringID; }
		const std::string &label() const { return m_label; }
		void setLabel(const std::string &label) { m_label = label; }

		std::string modName() const { return m_stringID.substr(0, m_stringID.find_first_of(":")); }

		bool isOpaque() const { return m_id != 0 && m_isOpaque && m_drawType != BlockDrawType::XShape; }
		void setOpaque(bool isOpaque) { m_isOpaque = isOpaque; }

		ItemStack getItemDrop() const { return ItemStack{m_itemDrop, m_itemDropAmount}; };
		void setItemDrop(const std::string &itemDrop, u16 itemDropAmount = 1) { m_itemDrop = itemDrop; m_itemDropAmount = itemDropAmount; }

		u8 harvestRequirements() const { return m_harvestRequirements; }
		void setHarvestRequirements(u8 harvestRequirements) { m_harvestRequirements = harvestRequirements; }

		float hardness() const { return m_hardness; }
		void setHardness(float hardness) { m_hardness = hardness; }

		float timeToBreak(u8 harvestCapability, float miningSpeed) const {
			return ((m_harvestRequirements & harvestCapability) == m_harvestRequirements) ? 1.5 * m_hardness / miningSpeed : 5 * m_hardness;
		}

		const gk::FloatBox &boundingBox() const { return m_boundingBox; }
		void setBoundingBox(const gk::FloatBox &boundingBox) { m_boundingBox = boundingBox; }

		BlockDrawType drawType() const { return m_drawType; }
		void setDrawType(BlockDrawType drawType) { m_drawType = drawType; }

		bool canUpdate() const { return m_canUpdate; }
		bool canBeActivated() const { return m_canBeActivated; }

		bool isLightSource() const { return m_isLightSource; }
		void setLightSource(bool isLightSource) { m_isLightSource = isLightSource; }

		const gk::Color &colorMultiplier() const { return m_colorMultiplier; }
		void setColorMultiplier(const gk::Color &colorMultiplier) { m_colorMultiplier = colorMultiplier; }

		bool isRotatable() const { return m_isRotatable; }
		void setRotatable(bool isRotatable) { m_isRotatable = isRotatable; }

		const std::string &inventoryImage() const { return m_inventoryImage; }
		void setInventoryImage(const std::string &inventoryImage) { m_inventoryImage = inventoryImage; }

		void addGroup(const std::string &name, u16 value) { m_groups.emplace(name, value); }
		bool hasGroup(const std::string &name) const { return m_groups.find(name) != m_groups.end(); }

		u16 getGroupValue(const std::string &name) const {
			auto it = m_groups.find(name);
			if (it == m_groups.end())
				return 0;

			return it->second;
		}

		static void initUsertype(sol::state &lua);

	protected:
		glm::vec4 getTexCoordsFromID(int textureID) const;

		bool m_canUpdate = false;
		bool m_canBeActivated = false;

	private:
		u32 m_id = 0;
		TilesDef m_tiles;

		std::string m_stringID;
		std::string m_label;

		std::string m_itemDrop;
		u16 m_itemDropAmount;

		u8 m_harvestRequirements = 0;
		float m_hardness = 1.0f;

		gk::FloatBox m_boundingBox{0, 0, 0, 1, 1, 1};

		BlockDrawType m_drawType = BlockDrawType::Solid;

		bool m_isOpaque = true;

		bool m_isLightSource = false;

		gk::Color m_colorMultiplier = gk::Color::White;

		bool m_isRotatable = false;

		std::string m_inventoryImage;

		std::unordered_map<std::string, u16> m_groups;
};

#endif // BLOCK_HPP_
