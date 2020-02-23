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
#include "EngineConfig.hpp"
#include "Registry.hpp"
#include "ServerChunk.hpp"
#include "TerrainGenerator.hpp"
#include "World.hpp"

#include <glm/gtc/noise.hpp>
#include "FastNoise.hpp"

TerrainGenerator::TerrainGenerator() {
}

void TerrainGenerator::generate(ServerChunk &chunk) const {
	fastNoiseGeneration(chunk);
}

void TerrainGenerator::setBlocksFromLuaTable(const sol::table &table) {
	m_dirtBlockID      = Registry::getInstance().getBlockFromStringID(table["dirt"].get<std::string>()).id();
	m_grassBlockID     = Registry::getInstance().getBlockFromStringID(table["grass"].get<std::string>()).id();
	m_stoneBlockID     = Registry::getInstance().getBlockFromStringID(table["stone"].get<std::string>()).id();
	m_logBlockID       = Registry::getInstance().getBlockFromStringID(table["log"].get<std::string>()).id();
	m_leavesBlockID    = Registry::getInstance().getBlockFromStringID(table["leaves"].get<std::string>()).id();
	m_flowerBlockID    = Registry::getInstance().getBlockFromStringID(table["flower"].get<std::string>()).id();
	m_waterBlockID     = Registry::getInstance().getBlockFromStringID(table["water"].get<std::string>()).id();
	m_sandBlockID      = Registry::getInstance().getBlockFromStringID(table["sand"].get<std::string>()).id();
	m_tallgrassBlockID = Registry::getInstance().getBlockFromStringID(table["tallgrass"].get<std::string>()).id();
}

void TerrainGenerator::fastNoiseGeneration(ServerChunk &chunk) const {
	FastNoise noise;
	noise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
	noise.SetFrequency(1 / 256.0f);
	noise.SetFractalOctaves(4);

	srand(chunk.x() + chunk.y() + chunk.z() + 1337);
	Chunk *topChunk = chunk.getSurroundingChunk(Chunk::Top);
	for(int y = 0 ; y < CHUNK_DEPTH ; y++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			// Land height
			double n = noise.GetNoise(-x - chunk.x() * CHUNK_WIDTH, y + chunk.y() * CHUNK_DEPTH);
			double h = 10 + n * 20;

			// double n = noise2d((x + chunk.x() * CHUNK_WIDTH) / 256.0, (y + chunk.y() * CHUNK_DEPTH) / 256.0, 4, 0.5) * 4;
			// double h = 10 + n * 2;

			// Land blocks
			for(int z = 0 ; z < CHUNK_HEIGHT ; z++) {
				// Are we above "ground" level?
				if(z + chunk.z() * CHUNK_HEIGHT > h) {
					// If we are not yet up to sea level, fill with water blocks
					if(z + chunk.z() * CHUNK_HEIGHT < SEALEVEL) {
						chunk.setBlockRaw(x, y, z, m_waterBlockID);
					}
					// Otherwise we are in the air, so try to make a tree
					else if(chunk.getBlock(x, y, z - 1) == m_grassBlockID && (rand() % 64) == 0 && n < 4) {
						// Trunk
						int h = (rand() & 3) + 3;
						for(int i = 0 ; i < h ; i++) {
							chunk.setBlockRaw(x, y, z + i, m_logBlockID);
						}

						// Leaves
						for(int iz = -3 ; iz <= 3 ; iz++) {
							for(int iy = -3 ; iy <= 3 ; iy++) {
								for(int ix = -3 ; ix <= 3 ; ix++) {
									if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !chunk.getBlock(x + ix, y + iy, z + h + iz)) {
										chunk.setBlockRaw(x + ix, y + iy, z + h + iz, m_leavesBlockID);

										// FIXME: This is a temporary fix for the second part of #41
										chunk.lightmap().setSunlight(x + ix, y + iy, z + h + iz, 0);
									}
								}
							}
						}
					}
					// Or tallgrass
					else if(chunk.getBlock(x, y, z - 1) == m_grassBlockID && (rand() % 32) == 0) {
						chunk.setBlockRaw(x, y, z, m_tallgrassBlockID);
					}
					// Or a flower
					else if(chunk.getBlock(x, y, z - 1) == m_grassBlockID && (rand() & 0xff) == 0) {
						chunk.setBlockRaw(x, y, z, m_flowerBlockID);
					}
					// If we are on the top block of the chunk, add sunlight
					else if (z == CHUNK_HEIGHT - 1) {
						chunk.lightmap().addSunlight(x, y, z, 15);
					}
				}
				else {
					if (z + chunk.z() * CHUNK_HEIGHT >= h - 1 && z + chunk.z() * CHUNK_HEIGHT > SEALEVEL - 1)
						chunk.setBlockRaw(x, y, z, m_grassBlockID);
					else if (z + chunk.z() * CHUNK_HEIGHT <= SEALEVEL - 1 && h < SEALEVEL && z + chunk.z() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_sandBlockID);
					else if (z + chunk.z() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_dirtBlockID);
					else
						chunk.setBlockRaw(x, y, z, m_stoneBlockID);

					// Caves
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

float TerrainGenerator::noise2d(double x, double y, int octaves, float persistence) {
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

float TerrainGenerator::noise3d_abs(double x, double y, double z, int octaves, float persistence) {
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

