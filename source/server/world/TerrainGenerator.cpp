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
#include "EngineConfig.hpp"
#include "Registry.hpp"
#include "ServerChunk.hpp"
#include "TerrainGenerator.hpp"
#include "World.hpp"

#include <glm/gtc/noise.hpp>
#include "FastNoise.hpp"

void TerrainGenerator::generate(ServerChunk &chunk) const {
	fastNoiseGeneration(chunk);
}

void TerrainGenerator::fastNoiseGeneration(ServerChunk &chunk) const {
	HeightmapChunk &heightmap = m_heightmap.getOrCreateChunk(chunk.x(), chunk.y());

	Random_t rand;
	rand.seed(chunk.x() + chunk.y() * CHUNK_WIDTH + chunk.z() * CHUNK_WIDTH * CHUNK_HEIGHT + 1337);

	Chunk *topChunk = chunk.getSurroundingChunk(Chunk::Top);
	for(int y = 0 ; y < CHUNK_DEPTH ; y++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			u16 biomeIndex = m_biomeSampler.getBiomeIndexAt(x + chunk.x() * CHUNK_WIDTH, y + chunk.y() * CHUNK_DEPTH);
			const Biome &biome = Registry::getInstance().getBiome(biomeIndex);

			// Land height
			double h = heightmap.landHeightAt(x, y);

			// Land blocks
			for(int z = 0 ; z < CHUNK_HEIGHT ; z++) {
				// Are we above "ground" level?
				if(z + chunk.z() * CHUNK_HEIGHT > h) {
					// If we are not yet up to sea level, fill with water blocks
					if (z + chunk.z() * CHUNK_HEIGHT < SEALEVEL) {
						chunk.setBlockRaw(x, y, z, biome.getLiquidBlockID());
					}
					// Otherwise we are in the air
					else if (chunk.getBlock(x, y, z) == 0 && z == CHUNK_HEIGHT - 1) {
						// Add sunlight at the top of the chunk if possible
						chunk.lightmap().addSunlight(x, y, z, 15);
					}
				}
				else {
					bool isGeneratingTopBlock = z + chunk.z() * CHUNK_HEIGHT >= h - 1 && z + chunk.z() * CHUNK_HEIGHT > SEALEVEL - 1;
					if (isGeneratingTopBlock)
						chunk.setBlockRaw(x, y, z, biome.getTopBlockID());
					else if (z + chunk.z() * CHUNK_HEIGHT <= SEALEVEL - 1 && h < SEALEVEL && z + chunk.z() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, biome.getBeachBlockID());
					else if (z + chunk.z() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, biome.getGroundBlockID());
					else
						chunk.setBlockRaw(x, y, z, biome.getDeepBlockID());

					// Caves
					generateCaves(chunk, x, y, z, h);

					// Populate ores.
					generateOres(chunk, x, y, z, biome, rand);

					// Generate trees, flora and portals
					if (isGeneratingTopBlock && chunk.getBlock(x, y, z)) {
						// Try to place a tree
						bool placedTree = tryPlaceTree(chunk, x, y, z + 1, biome, rand);

						// Otherwise try to place flora.
						bool placedFlora = false;
						if (!placedTree)
							placedFlora = tryPlaceFlora(chunk, x, y, z + 1, biome, rand);

						// Or a portal
						if (!placedTree && !placedFlora)
							tryPlacePortal(chunk, x, y, z + 1, biome, rand);
					}
				}

				if (topChunk && topChunk->isInitialized()) {
					int sunlightLevel = topChunk->lightmap().getSunlight(x, y, 0);
					if (sunlightLevel) {
						chunk.lightmap().addSunlight(x, y, CHUNK_HEIGHT - 1, sunlightLevel);
					}
				}
			}
		}
	}
}

inline bool TerrainGenerator::tryPlaceTree(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const {
	if (chunk.getBlock(x, y, z - 1) == biome.getTopBlockID()) {
		for (const PlacementEntry::Tree &treePlacement : biome.getTrees()) {
			if (!rand.get<bool>(treePlacement.probability))
				continue;

			const Tree &tree = Registry::getInstance().getTree(treePlacement.treeID);

			// Trunk
			int h = rand.get(tree.trunkMinHeight(), tree.trunkMaxHeight());
			for (int i = 0; i < h; i++) {
				chunk.setBlockRaw(x, y, z + i, tree.getLogBlockID());
			}

			// Leaves
			if (tree.hasLeaves()) {
				for (int iz = -3; iz <= 3; iz++) {
					for (int iy = -3; iy <= 3; iy++) {
						for (int ix = -3; ix <= 3; ix++) {
							u16 block = chunk.getBlock(x + ix, y + iy, z + h + iz);
							if (ix * ix + iy * iy + iz * iz < 8 + rand.get(0, 1) && !block) {
								chunk.setBlockRaw(x + ix, y + iy, z + h + iz, tree.getLeavesBlockID());

								// FIXME: This is a temporary fix for the second part of #41
								chunk.lightmap().setSunlight(x + ix, y + iy, z + h + iz, 0);
							}
						}
					}
				}
			}

			return true;
		}
	}

	return false;
}

inline bool TerrainGenerator::tryPlaceFlora(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const {
	for (const PlacementEntry::Flora &flora : biome.getFlora()) {
		if (chunk.getBlock(x, y, z - 1) != flora.spawnsOnBlockID)
			continue;

		if (!rand.get<bool>(flora.probability))
			continue;

		chunk.setBlockRaw(x, y, z, flora.blockID);

		return true;
	}

	return false;
}

inline bool TerrainGenerator::tryPlacePortal(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const {
	// FIXME: This is a temporary portal generation
	//        This code should be replaced by a proper "feature" implementation
	//        which will also allow making stuff like villages easier
	if (chunk.getBlock(x, y, z - 1) == biome.getTopBlockID() && rand.get<bool>(0.0002)) {
		for (int ix = 0 ; ix < 4 ; ++ix) {
			for (int iz = 0 ; iz < 5 ; ++iz) {
				if (ix == 0 || iz == 0 || ix == 3 || iz == 4)
					chunk.setBlockRaw(x + ix, y, z + iz, biome.getPortalFrameBlockID());
				else
					chunk.setBlockRaw(x + ix, y, z + iz, biome.getPortalBlockID());
			}
		}

		return true;
	}

	return false;
}

inline void TerrainGenerator::generateOres(ServerChunk &chunk, int x, int y, int z, const Biome &biome, Random_t &rand) const {
	// TODO: Like trees, ores should be able to seamlessly cross chunk boundaries.
	// This could be achieved either by setting up a generation pipeline with stages,
	// processing neighboring chunks' ores every time, or generating them with noise.
	for (const PlacementEntry::Ore &ore : biome.getOres()) {
		if (!rand.get<bool>(ore.probability))
			continue;

		if (ore.genType == PlacementEntry::Ore::Gen::RandomWalk)
			randomWalkOrePlace(chunk, x, y, z, rand, ore.blockID, biome.getDeepBlockID(), ore.size);
		else if (ore.genType == PlacementEntry::Ore::Gen::FloodFill)
			oreFloodFill(chunk, x, y, z, biome.getDeepBlockID(), ore.blockID, ore.size, rand);

		break;
	}
}

inline void TerrainGenerator::randomWalkOrePlace(ServerChunk &chunk, int x, int y, int z, Random_t &rand, u16 oreBlock, u16 deepBlock, int size) const {
	if (size <= 0 || chunk.getBlock(x, y, z) != deepBlock)
		return;
	chunk.setBlockRaw(x, y, z, oreBlock);
	int direction = rand.get(1, 6);
	switch(direction) {
		case 1:
			randomWalkOrePlace(chunk, x + 1, y, z, rand, oreBlock, deepBlock, size - 1);
			break;
		case 2:
			randomWalkOrePlace(chunk, x - 1, y, z, rand, oreBlock, deepBlock, size - 1);
			break;
		case 3:
			randomWalkOrePlace(chunk, x, y + 1, z, rand, oreBlock, deepBlock, size - 1);
			break;
		case 4:
			randomWalkOrePlace(chunk, x, y - 1, z, rand, oreBlock, deepBlock, size - 1);
			break;
		case 5:
			randomWalkOrePlace(chunk, x, y, z + 1, rand, oreBlock, deepBlock, size - 1);
			break;
		case 6:
			randomWalkOrePlace(chunk, x, y, z - 1, rand, oreBlock, deepBlock, size - 1);
			break;
	}
}

void TerrainGenerator::oreFloodFill(ServerChunk &chunk, double x, double y, double z, u16 toReplace, u16 replaceWith, int depth, Random_t &rand) const {
	if (depth < 0) return;
	if (chunk.getBlock(x, y, z) == replaceWith) return;
	if (chunk.getBlock(x, y, z) == toReplace)
		chunk.setBlockRaw(x, y, z, replaceWith);

	oreFloodFill(chunk, x + 1, y, z, toReplace, replaceWith, depth - 1, rand);
	oreFloodFill(chunk, x - 1, y, z, toReplace, replaceWith, depth - 1, rand);
	oreFloodFill(chunk, x, y + 1, z, toReplace, replaceWith, depth - 1, rand);
	oreFloodFill(chunk, x, y - 1, z, toReplace, replaceWith, depth - 1, rand);
	oreFloodFill(chunk, x, y, z + 1, toReplace, replaceWith, depth - 1, rand);
	oreFloodFill(chunk, x, y, z - 1, toReplace, replaceWith, depth - 1, rand);

	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x + 1, y + 1, z + 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x + 1, y + 1, z - 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x + 1, y - 1, z + 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x + 1, y - 1, z - 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x - 1, y + 1, z + 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x - 1, y + 1, z - 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x - 1, y - 1, z + 1, toReplace, replaceWith, depth - 1, rand);
	if (rand.get<bool>(1.f / 15.f))
		oreFloodFill(chunk, x - 1, y - 1, z - 1, toReplace, replaceWith, depth - 1, rand);
}

inline void TerrainGenerator::generateCaves(ServerChunk &chunk, int x, int y, int z, int h) const {
	float n2 = noise2d(-(x + chunk.x() * CHUNK_WIDTH) / 256.0, (y + chunk.y() * CHUNK_DEPTH) / 256.0, 8, 0.3) * 4;
	float r2 = noise3d_abs(-(x + chunk.x() * CHUNK_WIDTH) / 512.0f, (z + chunk.z() * CHUNK_HEIGHT) / 512.0f, (y + chunk.y() * CHUNK_DEPTH) / 512.0f, 4, 0.1);
	float r3 = noise3d_abs(-(x + chunk.x() * CHUNK_WIDTH) / 512.0f, (z + chunk.z() * CHUNK_HEIGHT) / 128.0f, (y + chunk.y() * CHUNK_DEPTH) / 512.0f, 4, 1);
	float r4 = n2 * 5 + r2 * r3 * 20;
	if (r4 > 6 && r4 < 8 && h > SEALEVEL) {
		chunk.setBlockRaw(x, y, z - 1, 0);
		chunk.setBlockRaw(x, y, z, 0);
		chunk.setBlockRaw(x, y, z + 1, 0);
	}
}

inline float TerrainGenerator::noise2d(double x, double y, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * glm::simplex(glm::vec2{x, y} * scale);
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

inline float TerrainGenerator::noise3d_abs(double x, double y, double z, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * fabsf(glm::simplex(glm::vec3{x, y, z} * scale));
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

