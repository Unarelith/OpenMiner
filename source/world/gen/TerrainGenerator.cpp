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
#include <glm/gtc/noise.hpp>

#include "Chunk.hpp"
#include "TerrainGenerator.hpp"

void TerrainGenerator::generate(Chunk &chunk) const {
	if(chunk.isGenerated()) return;

	for(u8 z = 0 ; z < Chunk::depth ; z++) {
		for(u8 x = 0 ; x < Chunk::width ; x++) {
			float n = noise2d((x + chunk.x() * Chunk::width) / 256.0, (z + chunk.z() * Chunk::depth) / 256.0, 5, 0.5) * 4;
			float h = 10 + n * 2;

			for(u8 y = 0 ; y < Chunk::height ; y++) {
				if(y + chunk.y() * Chunk::height < h) {
					chunk.addBlock(glm::vec3{x + chunk.x() * Chunk::width, y + chunk.y() * Chunk::height, z + chunk.z() * Chunk::depth}, 1);
				} else {
					chunk.addBlock(glm::vec3{x + chunk.x() * Chunk::width, y + chunk.y() * Chunk::height, z + chunk.z() * Chunk::depth}, 0);
				}
			}
		}
	}

	chunk.setGenerated(true);
	chunk.setChanged(true);

	if (chunk.getSurroundingChunk(0) && chunk.getSurroundingChunk(0)->isGenerated()) chunk.getSurroundingChunk(0)->setChanged(true);
	if (chunk.getSurroundingChunk(1) && chunk.getSurroundingChunk(1)->isGenerated()) chunk.getSurroundingChunk(1)->setChanged(true);
	if (chunk.getSurroundingChunk(2) && chunk.getSurroundingChunk(2)->isGenerated()) chunk.getSurroundingChunk(2)->setChanged(true);
	if (chunk.getSurroundingChunk(3) && chunk.getSurroundingChunk(3)->isGenerated()) chunk.getSurroundingChunk(3)->setChanged(true);
}

float TerrainGenerator::noise2d(float x, float y, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * glm::simplex(glm::vec2(x, y) * scale);
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
		sum += strength * fabs(glm::simplex(glm::vec3(x, y, z) * scale));
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}


