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

#include <random.hpp>

#include <sol/sol.hpp>

#include "Heightmap.hpp"
#include "IntTypes.hpp"
#include "TerrainBiomeSampler.hpp"

using Random_t = effolkronium::random_local;

class Biome;
class Dimension;
class ServerChunk;

class TerrainGenerator {
	public:
		TerrainGenerator(Heightmap &heightmap, const Dimension &dimension, s32 seed);

		void generate(ServerChunk &chunk);

		void setSeed(s32 seed) { m_caveNoise.SetSeed(seed); }

	private:
		void fastNoiseGeneration(ServerChunk &chunk);

		bool tryPlaceTree(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const;
		bool tryPlaceFlora(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const;
		bool tryPlacePortal(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const;

		void generateOres(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const;
		void generateCavesOld(ServerChunk &chunk, s8 x, s8 y, s8 z, int h, HeightmapChunk &heightmap) const;
		void generateCaves(ServerChunk &chunk, int x, int y, int z);

		void randomWalkOrePlace(ServerChunk &chunk, int x, int y, int z, Random_t &rand, u16 oreBlock, u16 deepBlock, int size) const;
		void oreFloodFill(ServerChunk &chunk, int x, int y, int z, u16 toReplace, u16 replaceWith, int depth, Random_t &rand) const;

		static float noise2d(double x, double y, int octaves, float persistence);
		static float noise3d_abs(double x, double y, double z, int octaves, float persistence);

		TerrainBiomeSampler m_biomeSampler;

		Heightmap &m_heightmap;

		FastNoiseLite m_caveNoise;
};

#endif // TERRAINGENERATOR_HPP_
