/*
 * =====================================================================================
 *
 *       Filename:  World.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/12/2014 15:28:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "World.hpp"

World::World() {
	m_width = 16;
	m_depth = 16;
	
	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
			m_chunks.push_back(std::unique_ptr<Chunk>(new Chunk(x, 0, z)));
		}
	}
	
	for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
		for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
			if(x > -m_width / 2)	 getChunk(x, z)->setLeft(getChunk(x - 1, z));
			if(x <  m_width / 2 - 1) getChunk(x, z)->setRight(getChunk(x + 1, z));
			if(z > -m_depth / 2)	 getChunk(x, z)->setFront(getChunk(x, z - 1));
			if(z <  m_depth / 2 - 1) getChunk(x, z)->setBack(getChunk(x, z + 1));
		}
	}
}

World::~World() {
}

void World::draw(Shader &shader, const glm::mat4 &pv) {
	float ud = 1000.0;
	s32 ux = 0;
	s32 uz = 0;
	
	for(auto &it : m_chunks) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(it->x() * Chunk::width, it->y() * Chunk::height, it->z() * Chunk::depth));
		glm::mat4 mvp = pv * model;
		
		// Is this chunk on the screen?
		glm::vec4 center = mvp * glm::vec4(Chunk::width / 2, Chunk::height / 2, Chunk::depth / 2, 1);
		
		float d = glm::length(center);
		center.x /= center.w;
		center.y /= center.w;
		
		// If it is behind the camera, don't bother drawing it
		if(center.z < -Chunk::height / 2) {
			continue;
		}
		
		// It it is outside the screen, don't bother drawing it
		if(fabsf(center.x) > 1 + fabsf(Chunk::height * 2 / center.w) || fabsf(center.y) > 1 + fabsf(Chunk::height * 2 / center.w)) {
			continue;
		}
		
		if(!it->initialized()) {
			if(d < ud) {
				ud = d;
				ux = it->x();
				uz = it->z();
			}
			
			continue;
		}
		
		shader.setUniform("u_modelMatrix", model);
		shader.setUniform("u_viewProjectionMatrix", pv);
		
		it->draw(shader);
	}
	
	if(ud < 1000) {
		getChunk(ux, uz)->generate();
		getChunk(ux, uz)->setInitialized(true);
		
		if(getChunk(ux, uz)->left())  getChunk(ux, uz)->left()->generate();
		if(getChunk(ux, uz)->right()) getChunk(ux, uz)->right()->generate();
		if(getChunk(ux, uz)->front()) getChunk(ux, uz)->front()->generate();
		if(getChunk(ux, uz)->back())  getChunk(ux, uz)->back()->generate();
	}
}

//#include "Debug.hpp"

Chunk *World::getChunk(s32 x, s32 z) {
	//DEBUG("(", x, z, ") => (", x + m_width / 2, z + m_depth / 2, ")");
	
	x += m_width / 2;
	z += m_depth / 2;
	
	return m_chunks[x + z * m_width].get();
}

