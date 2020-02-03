/*
 * =====================================================================================
 *
 *       Filename:  TerrainGenerator.hpp
 *
 *    Description:
 *
 *        Created:  12/06/2018 22:35:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
