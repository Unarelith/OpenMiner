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

class Biome : public ISerializable {
public:
	Biome() = default;
	Biome(u32 id, const std::string &stringID, const std::string &label);
	~Biome() = default;

	void serialize(sf::Packet &packet) const override;
	void deserialize(sf::Packet &packet) override;

	const std::string &stringID() const { return m_stringID; }
	const std::string &label() const { return m_label; }
	void setLabel(const std::string &label) { m_label = label; }

	std::vector<double> getParams() { return m_params; }
	const u16 getTopBlock() const { return m_topBlock; }
	const u16 getGroundBlock() const { return m_groundBlock; }
	const u16 getBeachBlock() const { return m_beachBlock; }
	const u16 getLiquidBlock() const { return m_liquidBlock; }

	void setParams(std::vector<double> value) { m_params = value; }
	void setTopBlock(u16 value) { m_topBlock = value; }
	void setGroundBlock(u16 value) { m_groundBlock = value; }
	void setBeachBlock(u16 value) { m_beachBlock = value; }
	void setLiquidBlock(u16 value) { m_liquidBlock = value; }

private:
	u32 m_id;
	std::string m_stringID;
	std::string m_label;

	// TODO something to distinguish the worldtype of biome
	std::vector<double> m_params;
	u16 m_topBlock;
	u16 m_groundBlock;
	u16 m_beachBlock;
	u16 m_liquidBlock;

};

#endif // BIOME_HPP_