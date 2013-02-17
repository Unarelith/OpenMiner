#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "types.h"
#include "map.h"
#include "cube.h"

using namespace std;

Cube::Cube(int x, int y, int z, GLuint texture, Map *map) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture = texture;
	
	m_map = map;
}

void Cube::draw() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	glBegin(GL_QUADS);
	
	// Front right
	if((m_y >= m_map->depth - 1) || (m_map->map[MAP_POS(m_x, m_y + 1, m_z)] == 0)) {
		if(m_selected) glColor3ub(255, 255, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y + 1, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	}
	
	// Front left
	if((m_x >= m_map->width - 1) || (m_map->map[MAP_POS(m_x + 1, m_y, m_z)] == 0)) {
		if(m_selected) glColor3ub(255, 255, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
	}
	
	// Back left
	if((m_y <= 0) || (m_map->map[MAP_POS(m_x, m_y - 1, m_z)] == 0)) {
		if(m_selected) glColor3ub(255, 255, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y, m_z + 1);
	}
	
	// Back right
	if((m_x <= 0) || (m_map->map[MAP_POS(m_x - 1, m_y, m_z)] == 0)) {
		if(m_selected) glColor3ub(255, 255, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x, m_y + 1, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x, m_y + 1, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y, m_z + 1);
	}
	
	// Bottom
	if((m_z <= 0) || (m_map->map[MAP_POS(m_x, m_y, m_z - 1)] == 0)) {
		if(m_selected) glColor3ub(255, 255, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z);
	}
	
	// Top
	if((m_z >= m_map->height - 1) || (m_map->map[MAP_POS(m_x, m_y, m_z + 1)] == 0)) {
		if(m_selected) glColor3ub(255, 255, 255);
		else glColor3ub(255, 0, 0);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	}
	
	glEnd();
}

