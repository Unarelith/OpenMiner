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
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "config.h"
#include "types.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "player.h"
#include "game.h"

using namespace std;

Cube::Cube(int x, int y, int z, GLuint texture) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture = texture;
	
	m_selected = false;
	m_selectedFace = -1;
}

void Cube::draw() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	glBegin(GL_QUADS);
	
	// Front right
	if((m_y >= Game::map->depth() - 1) || (Game::map->map()[MAP_POS(m_x, m_y + 1, m_z)] == 0)) {
		if(!m_selected) glColor3ub(255, 255, 255);
		else if(m_selectedFace == 0) glColor3ub(255, 0, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y + 1, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	}
	
	// Front left
	if((m_x >= Game::map->width() - 1) || (Game::map->map()[MAP_POS(m_x + 1, m_y, m_z)] == 0)) {
		if(!m_selected) glColor3ub(255, 255, 255);
		else if(m_selectedFace == 1) glColor3ub(255, 0, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
	}
	
	// Back left
	if((m_y <= 0) || (Game::map->map()[MAP_POS(m_x, m_y - 1, m_z)] == 0)) {
		if(!m_selected) glColor3ub(255, 255, 255);
		else if(m_selectedFace == 3) glColor3ub(255, 0, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y, m_z + 1);
	}
	
	// Back right
	if((m_x <= 0) || (Game::map->map()[MAP_POS(m_x - 1, m_y, m_z)] == 0)) {
		if(!m_selected) glColor3ub(255, 255, 255);
		else if(m_selectedFace == 2) glColor3ub(255, 0, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x, m_y + 1, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x, m_y + 1, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y, m_z + 1);
	}
	
	// Bottom
	if((m_z <= 0) || (Game::map->map()[MAP_POS(m_x, m_y, m_z - 1)] == 0)) {
		if(!m_selected) glColor3ub(255, 255, 255);
		else if(m_selectedFace == 5) glColor3ub(255, 0, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z);
	}
	
	// Top
	if((m_z >= Game::map->height() - 1) || (Game::map->map()[MAP_POS(m_x, m_y, m_z + 1)] == 0)) {
		if(!m_selected) glColor3ub(255, 255, 255);
		else if(m_selectedFace == 4) glColor3ub(255, 0, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	}
	
	glEnd();
}

