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
#include "chunk.h"
#include "game.h"

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

u16 _width = 16;
u16 _depth = 16;
u16 _height = 16;

u16 *_map = (u16*)malloc(_width * _depth * _height * sizeof(u16));

Map *m_map = new Map{_width, _depth, _height, _map};

Chunk::Chunk(int x, int y, int z, Textures textures) {
	srand(time(NULL));
	
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture = textures["stone"];
	
	for(s32 z = 0 ; z < m_map->height ; z++) {
		for(s32 y = 0 ; y < m_map->depth ; y++) {
			for(s32 x = 0 ; x < m_map->width ; x++) {
				if(z == 0) {
					m_map->map[MAP_POS(x, y, z)] = 3;
				}
				else if(z < m_map->height - 2) {
					if(rand()%3 >= 2) m_map->map[MAP_POS(x, y, z)] = 2;
				} else {
					if(rand()%3 >= 2) m_map->map[MAP_POS(x, y, z)] = 1;
				}
			}
		}
	}
	
	for(s32 z = 0 ; z < m_map->height ; z++) {
		for(s32 y = 0 ; y < m_map->depth ; y++) {
			for(s32 x = 0 ; x < m_map->width ; x++) {
				switch(m_map->map[MAP_POS(x, y, z)]) {
					case 1:
						m_cubes.push_back(new Cube(x, y, z, textures["grass"], m_map));
						break;
					case 2:
						m_cubes.push_back(new Cube(x, y, z, textures["stone"], m_map));
						break;
					case 3:
						m_cubes.push_back(new Cube(x, y, z, textures["bedrock"], m_map));
						break;
				}
			}
		}
	}
}

Chunk::~Chunk() {
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		delete (*it);
	}
	
	m_cubes.clear();
	
	delete m_map;
	free(_map);
}

void Chunk::draw() {
	glPushMatrix();
	
	glTranslatef(m_x, m_y, m_z);
	
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		(*it)->draw();
	}
	
	glPopMatrix();
}

void Chunk::deleteCube(Cube *cube) {
	for(std::vector<Cube*>::iterator it = m_cubes.begin(); it != m_cubes.end() ; it++) {
		if((*it)->x() == cube->x() && (*it)->y() == cube->y() && (*it)->z() == cube->z()) {
			if(cube->selected()) {
				m_map->map[MAP_POS(cube->x(), cube->y(), cube->z())] = 0;
				delete (*it);
				m_cubes.erase(it);
				break;
			}
		}
	}
}

void Chunk::addCube(Cube *selectedCube) {
	if(selectedCube->selectedFace() == 0) {
		if((MAP_POS(selectedCube->x(), selectedCube->y() + 1, selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y() + 1, selectedCube->z()) < m_map->width * m_map->depth * m_map->height))
			m_map->map[MAP_POS(selectedCube->x(), selectedCube->y() + 1, selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y() + 1, selectedCube->z(), m_texture, m_map));
	}
	else if(selectedCube->selectedFace() == 1) {
		if((MAP_POS(selectedCube->x() + 1, selectedCube->y(), selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x() + 1, selectedCube->y(), selectedCube->z()) < m_map->width * m_map->depth * m_map->height))
			m_map->map[MAP_POS(selectedCube->x() + 1, selectedCube->y(), selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x() + 1, selectedCube->y(), selectedCube->z(), m_texture, m_map));
	}
	else if(selectedCube->selectedFace() == 2) {
		if((MAP_POS(selectedCube->x() - 1, selectedCube->y(), selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x() - 1, selectedCube->y(), selectedCube->z()) < m_map->width * m_map->depth * m_map->height))
			m_map->map[MAP_POS(selectedCube->x() - 1, selectedCube->y(), selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x() - 1, selectedCube->y(), selectedCube->z(), m_texture, m_map));
	}
	else if(selectedCube->selectedFace() == 3) {
		if((MAP_POS(selectedCube->x(), selectedCube->y() - 1, selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y() - 1, selectedCube->z()) < m_map->width * m_map->depth * m_map->height))
			m_map->map[MAP_POS(selectedCube->x(), selectedCube->y() - 1, selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y() - 1, selectedCube->z(), m_texture, m_map));
	}
	else if(selectedCube->selectedFace() == 4) {
		if((MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1) < m_map->width * m_map->depth * m_map->height))
			m_map->map[MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1)] = 1;
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1, m_texture, m_map));
	}
	else if(selectedCube->selectedFace() == 5) {
		if((MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1) < m_map->width * m_map->depth * m_map->height))
			m_map->map[MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1)] = 1;
		if(selectedCube->z() - 1 >= 0) m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1, m_texture, m_map));
	}
}

