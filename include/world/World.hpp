/*
 * =====================================================================================
 *
 *       Filename:  World.hpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 15:28:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <memory>

#include "Chunk.hpp"
#include "TerrainGenerator.hpp"

class Camera;

class World {
	public:
		World();

		void draw(Camera &camera, Shader &shader, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix);

		Chunk *getChunk(s32 x, s32 z);

		void addSelectedBlock() { if(!selectedChunk || !selectedBlock) return; selectedChunk->setBlock(selectedBlock->pos(), 1); selectedChunk->update(); }
		void removeSelectedBlock() { if(!selectedChunk || !selectedBlock) return; selectedChunk->setBlock(selectedBlock->pos(), 0); selectedChunk->update(); }

		bool intersectionLinePlane(const glm::vec3 &normal, const glm::vec3 &planePoint, const glm::vec3 &lineOrigPoint, const glm::vec3 &directionVector, float *distance);
		bool intersectionLineBlock(int blockX, int blockY, int blockZ, const glm::vec3 &lineOrigPoint, const glm::vec3 &directionVector, float *distance, s8 *face);
		void testBlocks(Camera &camera, float maxDistance);

		// Render distance in chunks
		static const u16 renderDistance = 8;

		// FIXME: MOVE THIS QUICKLY
		static Block *selectedBlock;
		static Chunk *selectedChunk;

	private:
		s32 m_width;
		s32 m_depth;

		Texture m_texture;

		std::vector<std::unique_ptr<Chunk>> m_chunks;

		TerrainGenerator m_terrainGenerator;
};

#endif // WORLD_HPP_
