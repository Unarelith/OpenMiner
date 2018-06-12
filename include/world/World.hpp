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

class World {
	public:
		World();

		void draw(Shader &shader, const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix);

		Chunk *getChunk(s32 x, s32 z);

		// Render distance in chunks
		static const u16 renderDistance = 8;

	private:
		s32 m_width;
		s32 m_depth;

		Texture m_texture;

		std::vector<std::unique_ptr<Chunk>> m_chunks;
};

#endif // WORLD_HPP_
