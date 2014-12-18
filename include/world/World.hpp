/*
 * =====================================================================================
 *
 *       Filename:  World.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/12/2014 15:28:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
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
		~World();
		
		void draw(Shader &shader, const glm::mat4 &pv);
		
		Chunk *getChunk(s16 x, s16 z) { return m_chunks[x + z * m_width].get(); }
		
	private:
		s32 m_width;
		s32 m_depth;
		
		std::vector<std::unique_ptr<Chunk>> m_chunks;
};

#endif // WORLD_HPP_
