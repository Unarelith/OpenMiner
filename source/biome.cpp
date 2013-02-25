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

#include "types.h"
#include "player.h"
#include "map.h"
#include "cube.h"
#include "biome.h"
#include "scene.h"

using namespace std;

/*u16 width = 5;
u16 depth = 5;
u16 height = 5;

// Map is displayed from bottom
u16 map[5 * 5 * 5] = {
	1, 0, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	
	0, 1, 1, 1, 1,
	1, 1, 0, 1, 0,
	1, 1, 1, 1, 1,
	1, 0, 1, 0, 1,
	1, 1, 0, 1, 1,
	
	0, 0, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
};*/

u16 _width = 3;
u16 _depth = 3;
u16 _height = 2;

u16 _map[3 * 3 * 2] = {
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	
	0, 0, 0,
	0, 1, 0,
	0, 0, 0
};

Map *m_map = new Map{_width, _depth, _height, _map};

Biome::Biome(int x, int y, int z, GLuint texture) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	for(s32 z = 0 ; z < m_map->height ; z++) {
		for(s32 y = 0 ; y < m_map->depth ; y++) {
			for(s32 x = 0 ; x < m_map->width ; x++) {
				if(m_map->map[MAP_POS(x, y, z)] == 1) {
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
	Scene::testCubes(m_cubes);
	
	glPushMatrix();
	
	glTranslatef(m_x, m_y, m_z);
	
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		(*it)->draw();
	}
	
	glPopMatrix();
}

void Biome::deleteCube(Cube *cube) {
	for(std::vector<Cube*>::iterator it = m_cubes.begin(); it != m_cubes.end() ; it++) {
		if((*it)->x() == cube->x() && (*it)->y() == cube->y() && (*it)->z() == cube->z()) {
			m_map->map[MAP_POS(cube->x(), cube->y(), cube->z())] = 0;
			delete (*it);
			m_cubes.erase(it);
			break;
		}
	}
}

