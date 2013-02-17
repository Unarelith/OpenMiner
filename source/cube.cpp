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
	
	if((m_y >= m_map->depth - 1) || (m_map->map[MAP_POS(m_x, m_y + 1, m_z)] == 0)) {
		glColor3ub(255, 255, 255); // Front Right
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y + 1, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	}
	
	if((m_x >= m_map->width - 1) || (m_map->map[MAP_POS(m_x + 1, m_y, m_z)] == 0)) {
		glColor3ub(255, 255, 255); // Front left
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
	}
	
	if((m_y <= 0) || (m_map->map[MAP_POS(m_x, m_y - 1, m_z)] == 0)) {
		glColor3ub(255, 255, 255); // Back left
			glTexCoord2d(1, 0); glVertex3d(m_x, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y, m_z + 1);
	}
	
	if((m_x <= 0) || (m_map->map[MAP_POS(m_x - 1, m_y, m_z)] == 0)) {
		glColor3ub(255, 255, 255); // Back right
			glTexCoord2d(1, 0); glVertex3d(m_x, m_y + 1, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x, m_y + 1, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y, m_z + 1);
	}
	
	if((m_z <= 0) || (m_map->map[MAP_POS(m_x, m_y, m_z - 1)] == 0)) {
		glColor3ub(255, 255, 255); // Bottom
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z);
	}
	
	if((m_z >= m_map->height - 1) || (m_map->map[MAP_POS(m_x, m_y, m_z + 1)] == 0)) {
		glColor3ub(255, 255, 255); // Top
			glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z + 1);
			glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
			glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
			glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	}
	
	glEnd();
}

