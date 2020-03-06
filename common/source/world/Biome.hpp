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
#ifndef BIOME_HPP_
#define BIOME_HPP_

#include <string>
#include <vector>

#include <gk/core/IntTypes.hpp>

#include "ISerializable.hpp"
#include "PlacementEntry.hpp"

class Biome : public ISerializable {
	public:
		Biome() = default;
		Biome(u16 id, const std::string &stringID, const std::string &label);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		u16 id() const { return m_id; }
		const std::string &stringID() const { return m_stringID; }
		const std::string &label() const { return m_label; }
		void setLabel(const std::string &label) { m_label = label; }

		const std::vector<double> &getParams() const { return m_params; }

		u16 getTopBlockID() const { return m_topBlockID; }
		u16 getGroundBlockID() const { return m_groundBlockID; }
		u16 getDeepBlockID() const { return m_deepBlockID; }
		u16 getBeachBlockID() const { return m_beachBlockID; }
		u16 getLiquidBlockID() const { return m_liquidBlockID; }
		const std::vector<PlacementEntry::Flora> &getFlora() const { return m_flora; }
		const std::vector<PlacementEntry::Ore> &getOres() const { return m_ores; }
		const std::vector<PlacementEntry::Tree> &getTrees() const { return m_trees; }

		void setParams(std::vector<double> &value) { m_params = value; }

		void setTopBlockID(u16 value) { m_topBlockID = value; }
		void setGroundBlockID(u16 value) { m_groundBlockID = value; }
		void setDeepBlockID(u16 value) { m_deepBlockID = value; }
		void setBeachBlockID(u16 value) { m_beachBlockID = value; }
		void setLiquidBlockID(u16 value) { m_liquidBlockID = value; }
		void setFlora(std::vector<PlacementEntry::Flora> &value) { m_flora = value; }
		void setOres(std::vector<PlacementEntry::Ore> &value) { m_ores = value; }
		void setTrees(std::vector<PlacementEntry::Tree> &value) { m_trees = value; }

		PlacementEntry::Flora &addFlora() {
			m_flora.emplace_back();
			return m_flora.back();
		}

		PlacementEntry::Ore &addOre() {
			m_ores.emplace_back();
			return m_ores.back();
		}

		PlacementEntry::Tree &addTree() {
			m_trees.emplace_back();
			return m_trees.back();
		}

	private:
		u16 m_id;
		std::string m_stringID;
		std::string m_label;

		// TODO something to distinguish the worldtype of biome
		std::vector<double> m_params;
		u16 m_topBlockID;
		u16 m_groundBlockID;
		u16 m_deepBlockID;
		u16 m_beachBlockID;
		u16 m_liquidBlockID;
		std::vector<PlacementEntry::Flora> m_flora;
		std::vector<PlacementEntry::Ore> m_ores;
		std::vector<PlacementEntry::Tree> m_trees;
};

#endif // BIOME_HPP_
