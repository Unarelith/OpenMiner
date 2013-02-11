#include <string>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "types.h"
#include "map.h"
#include "cube.h"

Cube::Cube(float x, float y, float z, GLuint texture, Map *map) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture = texture;
	
	m_map = map;
}

void Cube::draw(float x, float y, float z) {
	m_x += x;
	m_y += y;
	m_z += z;
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	glBegin(GL_QUADS);
	
	glColor3ub(255, 255, 255); // Front Right
		glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
		glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
		glTexCoord2d(0, 1); glVertex3d(m_x, m_y + 1, m_z);
		glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
	
	glColor3ub(255, 255, 255); // Front left
		glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
		glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
		glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y + 1, m_z);
		glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
		
	glColor3ub(255, 255, 255); // Back left
		glTexCoord2d(1, 0); glVertex3d(m_x, m_y, m_z + 1);
		glTexCoord2d(0, 0); glVertex3d(m_x, m_y, m_z);
		glTexCoord2d(0, 1); glVertex3d(m_x + 1, m_y, m_z);
		glTexCoord2d(1, 1); glVertex3d(m_x + 1, m_y, m_z + 1);
	
	glColor3ub(255, 255, 255); // Back right
		glTexCoord2d(1, 0); glVertex3d(m_x, m_y + 1, m_z + 1);
		glTexCoord2d(0, 0); glVertex3d(m_x, m_y + 1, m_z);
		glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
		glTexCoord2d(1, 1); glVertex3d(m_x, m_y, m_z + 1);

	glColor3ub(255, 255, 255); // Bottom
		glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z);
		glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z);
		glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z);
		glTexCoord2d(1, 1); glVertex3d(m_x, m_y, m_z);

	glColor3ub(255, 255, 255); // Top
		glTexCoord2d(0, 1); glVertex3d(m_x, m_y, m_z + 1);
		glTexCoord2d(0, 0); glVertex3d(m_x + 1, m_y, m_z + 1);
		glTexCoord2d(1, 0); glVertex3d(m_x + 1, m_y + 1, m_z + 1);
		glTexCoord2d(1, 1); glVertex3d(m_x, m_y + 1, m_z + 1);
		
	glEnd();
}

