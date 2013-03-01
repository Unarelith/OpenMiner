/*---------------------------------------------------------------------------------
	
	KubKraft
	Copyright (C) 2012 Quent42340 <quent42340@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
---------------------------------------------------------------------------------*/
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "config.h"
#include "types.h"
#include "player.h"
#include "map.h"
#include "cube.h"
#include "chunk.h"
#include "biome.h"
#include "scene.h"

Chunk *Biome::currentChunk = NULL;

Biome::Biome(int x, int y, int z, Textures textures) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_textures = textures;
	
	m_chunks.push_back(new Chunk(m_x, m_y, m_z, m_textures));
	m_chunks.push_back(new Chunk(m_x + 16, m_y, m_z, m_textures));
	m_chunks.push_back(new Chunk(m_x, m_y + 16, m_z, m_textures));
	m_chunks.push_back(new Chunk(m_x + 16, m_y + 16, m_z, m_textures));
	
	currentChunk = findNearestChunk(Scene::player->x(), Scene::player->y(), Scene::player->z());
}

Biome::~Biome() {
	m_chunks.clear();
	
	delete currentChunk;
}

void Biome::draw() {
	for(std::vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		(*it)->draw();
	}
}

void Biome::updateChunks() {
	currentChunk = findNearestChunk(Scene::player->x(), Scene::player->y(), Scene::player->z());
}

Chunk *Biome::findNearestChunk(float x, float y, float z) {
	float distance = FAR;
	Chunk *chunk = NULL;
	
	for(std::vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		vect3D center;
		
		// 8 here is 16, so chunk size, divided by 2
		center.x = (*it)->x() + 8;
		center.y = (*it)->y() + 8;
		center.z = (*it)->z() + 8;
		
		float d = sqrt(pow(center.x - x, 2) + pow(center.y - y, 2) + pow(center.z - z, 2));
		
		if(d < distance) {
			distance = d;
			chunk = (*it);
		}
	}
	
	return chunk;
}

