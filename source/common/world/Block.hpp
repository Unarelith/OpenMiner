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

#include <deque>

#include <glm/glm.hpp>

#include <sol/sol.hpp>

#include <gk/core/Box.hpp>
#include <gk/core/IntTypes.hpp>
#include <gk/core/ISerializable.hpp>
#include <gk/graphics/Color.hpp>

#include "BlockParam.hpp"
#include "BlockPlacementConstraints.hpp"
#include "BlockState.hpp"
#include "ItemStack.hpp"
#include "TilesDef.hpp"

class Chunk;
class Player;
class World;

class Block : public gk::ISerializable {
	public:
		Block() = default;
		Block(u32 id, const std::string &stringID);
		virtual ~Block() = default;

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		u16 id() const { return m_id; }

		const std::string &stringID() const { return m_stringID; }
		std::string modName() const { return m_stringID.substr(0, m_stringID.find_first_of(":")); }

		const TilesDef &tiles(u16 state) const;

		bool canUpdate() const { return m_canUpdate; }
		bool canBeActivated() const { return m_canBeActivated; }

		bool isRotatable() const { return m_isRotatable; }
		void setRotatable(bool isRotatable) { m_isRotatable = isRotatable; }

		void addGroup(const std::string &name, u16 value) { m_groups.emplace(name, value); }
		bool hasGroup(const std::string &name) const { return m_groups.find(name) != m_groups.end(); }

		u16 getGroupValue(const std::string &name) const {
			auto it = m_groups.find(name);
			if (it == m_groups.end())
				return 0;

			return it->second;
		}

		BlockState &addState();
		BlockState &getState(u16 id);
		const BlockState &getState(u16 id) const;
		const std::deque<BlockState> &states() const { return m_states; }

		const BlockParam &param() const { return m_param; }
		BlockParam &param() { return m_param; }

		u8 customParamBits() const { return m_customParamBits; }
		void setCustomParamBits(u8 customParamBits) { m_customParamBits = customParamBits; }

		BlockPlacementConstraints &placementConstraints() { return m_placementConstraints; }
		const BlockPlacementConstraints &placementConstraints() const { return m_placementConstraints; }

		static void initUsertype(sol::state &lua);

	protected:
		glm::vec4 getTexCoordsFromID(int textureID) const;

		bool m_canUpdate = false;
		bool m_canBeActivated = false;

	private:
		u16 m_id = 0;
		std::string m_stringID;

		bool m_isRotatable = false;

		std::unordered_map<std::string, u16> m_groups;

		std::deque<BlockState> m_states;

		BlockParam m_param{*this};

		u8 m_customParamBits = 0;

		BlockPlacementConstraints m_placementConstraints;
};

#endif // BLOCK_HPP_
