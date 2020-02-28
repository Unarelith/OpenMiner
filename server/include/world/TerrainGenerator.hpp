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
#ifndef TERRAINGENERATOR_HPP_
#define TERRAINGENERATOR_HPP_

#include <gk/core/IntTypes.hpp>

#include <sol.hpp>

class ServerChunk;

class TerrainGenerator {
	public:
		TerrainGenerator();

		void generate(ServerChunk &chunk) const;

		void setBlocksFromLuaTable(const sol::table &table);

	private:
		void fastNoiseGeneration(ServerChunk &chunk) const;

		void oreFloodFill(ServerChunk &chunk, double x, double y, double z, u16 toReplace, u16 replaceWith, int depth) const;
		static float noise2d(double x, double y, int octaves, float persistence);
		static float noise3d_abs(double x, double y, double z, int octaves, float persistence);

		u16 m_dirtBlockID = 0;
		u16 m_grassBlockID = 0;
		u16 m_stoneBlockID = 0;
		u16 m_logBlockID = 0;
		u16 m_leavesBlockID = 0;
		u16 m_flowerBlockID = 0;
		u16 m_waterBlockID = 0;
		u16 m_sandBlockID = 0;
		u16 m_tallgrassBlockID = 0;
		u16 m_oreIronBlockID = 0;
};

#endif // TERRAINGENERATOR_HPP_
