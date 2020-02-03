/*
 * =====================================================================================
 *
 *       Filename:  TerrainGenerator.cpp
 *
 *    Description:
 *
 *        Created:  12/06/2018 22:47:04
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "Registry.hpp"
#include "ServerChunk.hpp"
#include "TerrainGenerator.hpp"
#include "World.hpp"

#include <glm/gtc/noise.hpp>
#include "FastNoise.hpp"
#include "SimplexNoise.hpp"

void TerrainGenerator::generate(ServerChunk &chunk) const {
	// lightTestGeneration(chunk);
	// basicGeneration(chunk);
	testCraftGeneration(chunk);
	// simplexGeneration(chunk);
	// fastNoiseGeneration(chunk);
}

void TerrainGenerator::setBlocksFromLuaTable(const sol::table &table) {
	m_dirtBlockID   = Registry::getInstance().getBlockFromStringID(table["dirt"].get<std::string>()).id();
	m_grassBlockID  = Registry::getInstance().getBlockFromStringID(table["grass"].get<std::string>()).id();
	m_stoneBlockID  = Registry::getInstance().getBlockFromStringID(table["stone"].get<std::string>()).id();
	m_logBlockID    = Registry::getInstance().getBlockFromStringID(table["log"].get<std::string>()).id();
	m_leavesBlockID = Registry::getInstance().getBlockFromStringID(table["leaves"].get<std::string>()).id();
	m_flowerBlockID = Registry::getInstance().getBlockFromStringID(table["flower"].get<std::string>()).id();
	m_waterBlockID  = Registry::getInstance().getBlockFromStringID(table["water"].get<std::string>()).id();
	m_sandBlockID   = Registry::getInstance().getBlockFromStringID(table["sand"].get<std::string>()).id();
}

void TerrainGenerator::lightTestGeneration(ServerChunk &chunk) const {
	for(u8 y = 0 ; y < CHUNK_HEIGHT ; y++) {
		for(u8 z = 0 ; z < CHUNK_DEPTH ; z++) {
			for(u8 x = 0 ; x < CHUNK_WIDTH ; x++) {
				if (y == 4)
					chunk.setBlockRaw(x, y, z, m_stoneBlockID);
				else if (y == 10 && (z > 4 || chunk.z() != 0))
					chunk.setBlockRaw(x, y, z, m_stoneBlockID);
			}
		}
	}

	if (chunk.y() == 0) {
		for(u8 z = 0 ; z < CHUNK_DEPTH ; z++) {
			for(u8 x = 0 ; x < CHUNK_WIDTH ; x++) {
				chunk.lightmap().addSunlight(x, 31, z, 15);
			}
		}
	}

	// Chunk *topChunk = chunk.getSurroundingChunk(Chunk::Top);
	// if (topChunk) {
	// 	for(u8 z = 0 ; z < CHUNK_DEPTH ; z++) {
	// 		for(u8 x = 0 ; x < CHUNK_WIDTH ; x++) {
	// 			int sunlightLevel = topChunk->lightmap().getSunlight(x, 0, z);
	// 			if (sunlightLevel) {
	// 				chunk.lightmap().addSunlight(x, CHUNK_HEIGHT - 1, z, sunlightLevel);
	// 			}
	// 		}
	// 	}
	// }
}

void TerrainGenerator::basicGeneration(ServerChunk &chunk) const {
	for(u8 z = 0 ; z < CHUNK_DEPTH ; z++) {
		for(u8 x = 0 ; x < CHUNK_WIDTH ; x++) {
			float n = noise2d((x + chunk.x() * CHUNK_WIDTH) / 256.0, (z + chunk.z() * CHUNK_DEPTH) / 256.0, 5, 0.5) * 4;
			float h = 10 + n * 2;

			for(u8 y = 0 ; y < CHUNK_HEIGHT ; y++) {
				if(y + chunk.y() * CHUNK_HEIGHT < h) {
					chunk.setBlockRaw(x, y, z, 1);
				}
				else if (y == CHUNK_HEIGHT - 1) {
					chunk.lightmap().addSunlight(x, y, z, 15);
				}
			}
		}
	}
}

void TerrainGenerator::testCraftGeneration(ServerChunk &chunk) const {
	srand(1337);
	Chunk *topChunk = chunk.getSurroundingChunk(Chunk::Top);
	for(int z = 0 ; z < CHUNK_DEPTH ; z++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			// Land height
			float n = noise2d((x + chunk.x() * CHUNK_WIDTH) / 256.0, (z + chunk.z() * CHUNK_DEPTH) / 256.0, 4, 0.5) * 4;
			float h = 10 + n * 2;

			// Land blocks
			for(int y = 0 ; y < CHUNK_HEIGHT ; y++) {
				// Are we above "ground" level?
				if(y + chunk.y() * CHUNK_HEIGHT > h) {
					// If we are not yet up to sea level, fill with water blocks
					if(y + chunk.y() * CHUNK_HEIGHT < SEALEVEL) {
						chunk.setBlockRaw(x, y, z, m_waterBlockID);
					}
					// Otherwise we are in the air, so try to make a tree
					else if(chunk.getBlock(x, y - 1, z) == m_grassBlockID && (rand() % 256) == 0 && n < 4) {
						// Trunk
						h = (rand() & 0x3) + 3;
						for(int i = 0 ; i < h ; i++) {
							chunk.setBlockRaw(x, y + i, z, m_logBlockID);
						}

						// Leaves
						for(int ix = -3 ; ix <= 3 ; ix++) {
							for(int iy = -3 ; iy <= 3 ; iy++) {
								for(int iz = -3 ; iz <= 3 ; iz++) {
									if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !chunk.getBlock(x + ix, y + h + iy, z + iz)) {
										chunk.setBlockRaw(x + ix, y + h + iy, z + iz, m_leavesBlockID);
									}
								}
							}
						}
					}
					// Or a flower
					else if(chunk.getBlock(x, y - 1, z) == m_grassBlockID && (rand() & 0xff) == 0) {
						chunk.setBlockRaw(x, y, z, m_flowerBlockID);
					}
					// If we are on the top block of the chunk, add sunlight
					else if (y == CHUNK_HEIGHT - 1) {
						chunk.lightmap().addSunlight(x, y, z, 15);
					}
				}
				else {
					// // Random value used to determine land type
					// float r = noise3d_abs((x + chunk.x() * CHUNK_WIDTH) / 256.0, (y + chunk.y() * CHUNK_HEIGHT) / 256.0 + 16, (z + chunk.z() * CHUNK_DEPTH) / 256.0, 5, 0.5) * 4;
                    //
					// // Sand layer
					// if(n * 4 + r * 5 < 4) {
					// 	chunk.setBlockRaw(x, y, z, m_sandBlockID);
					// }
					// // Dirt layer, but use grass blocks for the top
					// else if(n + r * 5 < 6 * 8 && n * 10 + r * 5 > 30) {
					// 	chunk.setBlockRaw(x, y, z, (h < SEALEVEL - 5 || y + chunk.y() * CHUNK_HEIGHT < h - 1) ? m_dirtBlockID : m_grassBlockID);
					// }
					// else {
					// 	chunk.setBlockRaw(x, y, z, m_stoneBlockID);
					// }
					if (y + chunk.y() * CHUNK_HEIGHT >= h - 1 && y + chunk.y() * CHUNK_HEIGHT > SEALEVEL - 1)
						chunk.setBlockRaw(x, y, z, m_grassBlockID);
					else if (y + chunk.y() * CHUNK_HEIGHT <= SEALEVEL - 1 && h < SEALEVEL && y + chunk.y() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_sandBlockID);
					else if (y + chunk.y() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_dirtBlockID);
					else
						chunk.setBlockRaw(x, y, z, m_stoneBlockID);

					// Caves
					float n2 = noise2d((x + chunk.x() * CHUNK_WIDTH) / 256.0, (z + chunk.z() * CHUNK_DEPTH) / 256.0, 8, 0.3) * 4;
					float r2 = noise3d_abs((x + chunk.x() * CHUNK_WIDTH) / 512.0f, (y + chunk.y() * CHUNK_HEIGHT) / 512.0f, (z + chunk.z() * CHUNK_DEPTH) / 512.0f, 4, 0.1);
					float r3 = noise3d_abs((x + chunk.x() * CHUNK_WIDTH) / 512.0f, (y + chunk.y() * CHUNK_HEIGHT) / 128.0f, (z + chunk.z() * CHUNK_DEPTH) / 512.0f, 4, 1);
					float r4 = n2 * 5 + r2 * r3 * 20;
					if (r4 > 6 && r4 < 8 && h > SEALEVEL) {
						chunk.setBlockRaw(x, y - 1, z, 0);
						chunk.setBlockRaw(x, y, z, 0);
						chunk.setBlockRaw(x, y + 1, z, 0);
					}
				}

				if (topChunk && topChunk->isInitialized()) {
					int sunlightLevel = topChunk->lightmap().getSunlight(x, 0, z);
					if (sunlightLevel) {
						chunk.lightmap().addSunlight(x, CHUNK_HEIGHT - 1, z, sunlightLevel);
					}
				}
			}
		}
	}
}

void TerrainGenerator::simplexGeneration(ServerChunk &chunk) const {
	srand(1337);
	Chunk *topChunk = chunk.getSurroundingChunk(Chunk::Top);
	for(int z = 0 ; z < CHUNK_DEPTH ; z++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			// Land height
			float n = newNoise2d((x + chunk.x() * CHUNK_WIDTH) / 256.0, (z + chunk.z() * CHUNK_DEPTH) / 256.0, 4, 0.5) * 4;
			float h = 10 + n * 2;

			// Land blocks
			for(int y = 0 ; y < CHUNK_HEIGHT ; y++) {
				// Are we above "ground" level?
				if(y + chunk.y() * CHUNK_HEIGHT > h) {
					// If we are not yet up to sea level, fill with water blocks
					if(y + chunk.y() * CHUNK_HEIGHT < SEALEVEL) {
						chunk.setBlockRaw(x, y, z, m_waterBlockID);
					}
					// Otherwise we are in the air, so try to make a tree
					else if(chunk.getBlock(x, y - 1, z) == m_grassBlockID && (rand() % 256) == 0 && n < 4) {
						// Trunk
						h = (rand() & 0x3) + 3;
						for(int i = 0 ; i < h ; i++) {
							chunk.setBlockRaw(x, y + i, z, m_logBlockID);
						}

						// Leaves
						for(int ix = -3 ; ix <= 3 ; ix++) {
							for(int iy = -3 ; iy <= 3 ; iy++) {
								for(int iz = -3 ; iz <= 3 ; iz++) {
									if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !chunk.getBlock(x + ix, y + h + iy, z + iz)) {
										chunk.setBlockRaw(x + ix, y + h + iy, z + iz, m_leavesBlockID);
									}
								}
							}
						}
					}
					// Or a flower
					else if(chunk.getBlock(x, y - 1, z) == m_grassBlockID && (rand() & 0xff) == 0) {
						chunk.setBlockRaw(x, y, z, m_flowerBlockID);
					}
					// If we are on the top block of the chunk, add sunlight
					else if (y == CHUNK_HEIGHT - 1) {
						chunk.lightmap().addSunlight(x, y, z, 15);
					}
				}
				else {
					if (y + chunk.y() * CHUNK_HEIGHT >= h - 1 && y + chunk.y() * CHUNK_HEIGHT > SEALEVEL - 1)
						chunk.setBlockRaw(x, y, z, m_grassBlockID);
					else if (y + chunk.y() * CHUNK_HEIGHT <= SEALEVEL - 1 && h < SEALEVEL && y + chunk.y() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_sandBlockID);
					else if (y + chunk.y() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_dirtBlockID);
					else
						chunk.setBlockRaw(x, y, z, m_stoneBlockID);
				}

				if (topChunk && topChunk->isInitialized()) {
					int sunlightLevel = topChunk->lightmap().getSunlight(x, 0, z);
					if (sunlightLevel) {
						chunk.lightmap().addSunlight(x, CHUNK_HEIGHT - 1, z, sunlightLevel);
					}
				}
			}
		}
	}
}

void TerrainGenerator::fastNoiseGeneration(ServerChunk &chunk) const {
	FastNoise noise;
	noise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
	noise.SetFrequency(1);
	noise.SetFractalOctaves(4);

	srand(1337);
	Chunk *topChunk = chunk.getSurroundingChunk(Chunk::Top);
	for(int z = 0 ; z < CHUNK_DEPTH ; z++) {
		for(int x = 0 ; x < CHUNK_WIDTH ; x++) {
			// Land height
			float d = 256.0f;
			float n = noise.GetNoise((x + chunk.x() * CHUNK_WIDTH) / d, (z + chunk.z() * CHUNK_DEPTH) / d);
			float h = 10 + n * 20;

			// Land blocks
			for(int y = 0 ; y < CHUNK_HEIGHT ; y++) {
				// Are we above "ground" level?
				if(y + chunk.y() * CHUNK_HEIGHT > h) {
					// If we are not yet up to sea level, fill with water blocks
					if(y + chunk.y() * CHUNK_HEIGHT < SEALEVEL) {
						chunk.setBlockRaw(x, y, z, m_waterBlockID);
					}
					// Otherwise we are in the air, so try to make a tree
					else if(chunk.getBlock(x, y - 1, z) == m_grassBlockID && (rand() % 256) == 0 && n < 1) {
						// Trunk
						h = (rand() & 0x3) + 3;
						for(int i = 0 ; i < h ; i++) {
							chunk.setBlockRaw(x, y + i, z, m_logBlockID);
						}

						// Leaves
						for(int ix = -3 ; ix <= 3 ; ix++) {
							for(int iy = -3 ; iy <= 3 ; iy++) {
								for(int iz = -3 ; iz <= 3 ; iz++) {
									if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !chunk.getBlock(x + ix, y + h + iy, z + iz)) {
										chunk.setBlockRaw(x + ix, y + h + iy, z + iz, m_leavesBlockID);
									}
								}
							}
						}
					}
					// Or a flower
					else if(chunk.getBlock(x, y - 1, z) == m_grassBlockID && (rand() & 0xff) == 0) {
						chunk.setBlockRaw(x, y, z, m_flowerBlockID);
					}
					// If we are on the top block of the chunk, add sunlight
					else if (y == CHUNK_HEIGHT - 1) {
						chunk.lightmap().addSunlight(x, y, z, 15);
					}
				}
				else {
					if (y + chunk.y() * CHUNK_HEIGHT >= h - 1 && y + chunk.y() * CHUNK_HEIGHT > SEALEVEL - 1)
						chunk.setBlockRaw(x, y, z, m_grassBlockID);
					else if (y + chunk.y() * CHUNK_HEIGHT <= SEALEVEL - 1 && h < SEALEVEL && y + chunk.y() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_sandBlockID);
					else if (y + chunk.y() * CHUNK_HEIGHT > h - 3)
						chunk.setBlockRaw(x, y, z, m_dirtBlockID);
					else
						chunk.setBlockRaw(x, y, z, m_stoneBlockID);
				}

				if (topChunk && topChunk->isInitialized()) {
					int sunlightLevel = topChunk->lightmap().getSunlight(x, 0, z);
					if (sunlightLevel) {
						chunk.lightmap().addSunlight(x, CHUNK_HEIGHT - 1, z, sunlightLevel);
					}
				}
			}
		}
	}
}

float TerrainGenerator::noise2d(float x, float y, int octaves, float persistence) {
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

float TerrainGenerator::noise3d_abs(float x, float y, float z, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * fabs(glm::simplex(glm::vec3{x, y, z} * scale));
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

float TerrainGenerator::newNoise2d(float x, float y, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * SimplexNoise::noise(x * scale, y * scale);
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

float TerrainGenerator::newNoise3d_abs(float x, float y, float z, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * fabs(SimplexNoise::noise(x * scale, y * scale, z * scale));
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

