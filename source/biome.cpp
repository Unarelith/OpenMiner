#include <iostream>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "types.h"
#include "map.h"
#include "cube.h"
#include "biome.h"

using namespace std;

u16 width = 3;
u16 depth = 3;
u16 height = 2;

u16 map[3 * 3 * 2] = {
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	
	0, 0, 0,
	0, 1, 0,
	0, 0, 0
};

Map *m_map = new Map{width, depth, height, map};

Biome::Biome(float x, float y, float z, GLuint texture) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	
	for(u16 z = 0 ; z < 2 ; z++) {
		for(u16 y = 0 ; y < 3 ; y++) {
			for(u16 x = 0 ; x < 3 ; x++) {
				if(map[x + (y * m_map->depth) + (z * m_map->width * m_map->depth)] == 1) {
					m_cubes.push_back(new Cube(x, y, z, texture, m_map));
				}
			}
		}
	}
}

Biome::~Biome() {
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		delete (*it);
	}
	
	m_cubes.clear();
	
	delete m_map;
}

void Biome::draw() {
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		(*it)->draw(m_x, m_y, m_z);
	}
}
