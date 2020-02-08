/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef TERRAINGENERATOR_HPP_
#define TERRAINGENERATOR_HPP_

class ServerChunk;

class TerrainGenerator {
	public:
		void generate(ServerChunk &chunk) const;

		void setBlocksFromLuaTable(const sol::table &table);

	private:
		void lightTestGeneration(ServerChunk &chunk) const;
		void basicGeneration(ServerChunk &chunk) const;
		void testCraftGeneration(ServerChunk &chunk) const;
		void simplexGeneration(ServerChunk &chunk) const;
		void fastNoiseGeneration(ServerChunk &chunk) const;

		static float noise2d(float x, float y, int octaves, float persistence);
		static float noise3d_abs(float x, float y, float z, int octaves, float persistence);

		static float newNoise2d(float x, float y, int octaves, float persistence);
		static float newNoise3d_abs(float x, float y, float z, int octaves, float persistence);

		u16 m_dirtBlockID = 0;
		u16 m_grassBlockID = 0;
		u16 m_stoneBlockID = 0;
		u16 m_logBlockID = 0;
		u16 m_leavesBlockID = 0;
		u16 m_flowerBlockID = 0;
		u16 m_waterBlockID = 0;
		u16 m_sandBlockID = 0;
};

#endif // TERRAINGENERATOR_HPP_
