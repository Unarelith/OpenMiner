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
#include "config.h"
#include "player.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "game.h"

using namespace std;

Chunk::Chunk(int x, int y, int z, Textures textures) {
	srand(time(NULL));
	
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture = textures["stone"];
}

Chunk::~Chunk() {
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		delete (*it);
	}
	
	m_cubes.clear();
}

void Chunk::draw() {
	for(std::vector<Cube*>::iterator it = m_cubes.begin() ; it != m_cubes.end() ; it++) {
		(*it)->draw();
	}
}

void Chunk::deleteCube(Cube *cube) {
	for(std::vector<Cube*>::iterator it = m_cubes.begin(); it != m_cubes.end() ; it++) {
		if((*it)->x() == cube->x() && (*it)->y() == cube->y() && (*it)->z() == cube->z()) {
			if(cube->selected()) {
				Game::map->map()[MAP_POS(cube->x(), cube->y(), cube->z())] = 0;
				delete (*it);
				m_cubes.erase(it);
				break;
			}
		}
	}
}

void Chunk::addCube(Cube *selectedCube) {
	if(selectedCube->selectedFace() == -1) {
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y(), selectedCube->z(), selectedCube->texture()));
	}
	else if(selectedCube->selectedFace() == 0) {
		if((MAP_POS(selectedCube->x(), selectedCube->y() + 1, selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y() + 1, selectedCube->z()) < Game::map->width() * Game::map->depth() * Game::map->height()))
			Game::map->map()[MAP_POS(selectedCube->x(), selectedCube->y() + 1, selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y() + 1, selectedCube->z(), m_texture));
	}
	else if(selectedCube->selectedFace() == 1) {
		if((MAP_POS(selectedCube->x() + 1, selectedCube->y(), selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x() + 1, selectedCube->y(), selectedCube->z()) < Game::map->width() * Game::map->depth() * Game::map->height()))
			Game::map->map()[MAP_POS(selectedCube->x() + 1, selectedCube->y(), selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x() + 1, selectedCube->y(), selectedCube->z(), m_texture));
	}
	else if(selectedCube->selectedFace() == 2) {
		if((MAP_POS(selectedCube->x() - 1, selectedCube->y(), selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x() - 1, selectedCube->y(), selectedCube->z()) < Game::map->width() * Game::map->depth() * Game::map->height()))
			Game::map->map()[MAP_POS(selectedCube->x() - 1, selectedCube->y(), selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x() - 1, selectedCube->y(), selectedCube->z(), m_texture));
	}
	else if(selectedCube->selectedFace() == 3) {
		if((MAP_POS(selectedCube->x(), selectedCube->y() - 1, selectedCube->z()) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y() - 1, selectedCube->z()) < Game::map->width() * Game::map->depth() * Game::map->height()))
			Game::map->map()[MAP_POS(selectedCube->x(), selectedCube->y() - 1, selectedCube->z())] = 1;
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y() - 1, selectedCube->z(), m_texture));
	}
	else if(selectedCube->selectedFace() == 4) {
		if((MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1) < Game::map->width() * Game::map->depth() * Game::map->height()))
			Game::map->map()[MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1)] = 1;
		m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y(), selectedCube->z() + 1, m_texture));
	}
	else if(selectedCube->selectedFace() == 5) {
		if((MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1) >= 0) && (MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1) < Game::map->width() * Game::map->depth() * Game::map->height()))
			Game::map->map()[MAP_POS(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1)] = 1;
		if(selectedCube->z() - 1 >= 0) m_cubes.push_back(new Cube(selectedCube->x(), selectedCube->y(), selectedCube->z() - 1, m_texture));
	}
}

