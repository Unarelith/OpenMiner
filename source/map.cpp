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
#include <GL/gl.h>
#include <GL/glu.h>

#include "types.h"
#include "config.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "player.h"
#include "game.h"

using namespace std;

Chunk *Map::currentChunk = NULL;
Chunk *Map::selectedChunk = NULL;
Cube *Map::selectedCube = NULL;

Map::Map(u16 width, u16 depth, u16 height, Textures textures) {
	m_width = width;
	m_depth = depth;
	m_height = height;
	
	m_textures = textures;
	
	m_map = (u16*)malloc(m_width * m_depth * m_height * sizeof(u16));
	
	for(s32 z = 0 ; z < m_height ; z++) {
		for(s32 y = 0 ; y < m_depth ; y++) {
			for(s32 x = 0 ; x < m_width ; x++) {
				m_map[_MAP_POS(x, y, z)] = 0;
				if(z == 0) {
					m_map[_MAP_POS(x, y, z)] = 3;
				}
				else if(z < m_height - 2) {
					if(rand()%3 >= 2) m_map[_MAP_POS(x, y, z)] = 2;
				} else {
					if(rand()%3 >= 2) m_map[_MAP_POS(x, y, z)] = 1;
				}
			}
		}
	}
	
	for(u16 z = 0 ; z < m_height >> 3 ; z++) {
		for(u16 y = 0 ; y < m_depth >> 3 ; y++) {
			for(u16 x = 0 ; x < m_width >> 3 ; x++) {
				m_chunks.push_back(new Chunk(x << 3, y << 3, z << 3, m_textures));
			}
		}
	}
	
	for (int zz = 0 ; zz < m_height >> 3 ; zz++) {
		for (int yy = 0 ; yy < m_depth >> 3 ; yy++) {
			for (int xx = 0 ; xx < m_width >> 3 ; xx++) {
				int chunkIndex = xx + (yy * (m_width >> 3)) + (zz * (m_width >> 3) * (m_depth >> 3));
				if(xx - 1 >= 0) m_chunks[chunkIndex]->setSurroundingChunk(0, m_chunks[chunkIndex - 1]);
				if(xx + 1 < m_width >> 3) m_chunks[chunkIndex]->setSurroundingChunk(1, m_chunks[chunkIndex + 1]);
				if(yy - 1 >= 0) m_chunks[chunkIndex]->setSurroundingChunk(2, m_chunks[chunkIndex - (m_width >> 3)]);
				if(yy + 1 < m_depth >> 3) m_chunks[chunkIndex]->setSurroundingChunk(3, m_chunks[chunkIndex + (m_width >> 3)]);
				if(zz + 1 < m_height >> 3) m_chunks[chunkIndex]->setSurroundingChunk(4, m_chunks[chunkIndex + ((m_width >> 3) * (m_depth >> 3))]);
				if(zz - 1 >= 0) m_chunks[chunkIndex]->setSurroundingChunk(5, m_chunks[chunkIndex - ((m_width >> 3) * (m_depth >> 3))]);
			}
		}
	}
	
	for(vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		for(s32 z = (*it)->z() ; z < (*it)->z() + CHUNK_HEIGHT ; z++) {
			for(s32 y = (*it)->y() ; y < (*it)->y() + CHUNK_DEPTH ; y++) {
				for(s32 x = (*it)->x() ; x < (*it)->x() + CHUNK_WIDTH ; x++) {
					switch(m_map[_MAP_POS(x, y, z)]) {
						case 1:
							(*it)->addCube(new Cube(x, y, z, m_textures["grass"], 1));
							break;
						case 2:
							(*it)->addCube(new Cube(x, y, z, m_textures["stone"], 2));
							break;
						case 3:
							(*it)->addCube(new Cube(x, y, z, m_textures["bedrock"], 3));
							break;
					}
				}
			}
		}
		(*it)->refreshVBO();
	}
	
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	selectedCube = new Cube(-1, -1, -1, m_textures["dirt"], 0);
}

Map::~Map() {
	free(m_map);
	
	for(vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		delete (*it);
	}
	m_chunks.clear();
	
	delete selectedCube;
}

void Map::draw() {
	glBindTexture(GL_TEXTURE_2D, m_textures["stone"]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	for(vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		glPushMatrix();
		glTranslatef(float((*it)->x()), float((*it)->y()), float((*it)->z()));
		(*it)->render();
		glPopMatrix();
	}
	
	testCubes();
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if ((currentChunk != NULL) && (selectedCube->x() != -1) && (selectedCube->selectedFace() >= 0) && (selectedCube->selectedFace() < 6)) {
		float cubeCoords[6 * 12] = {
			1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, // FR | 0
			1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, // FL | 1
			0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, // BR | 2
			0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, // BL | 3
			0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, // T  | 4
			1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0  // B  | 5
		};
		
		int xx = selectedCube->x();
		int yy = selectedCube->y();
		int zz = selectedCube->z();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor4ub(255, 255, 255, 64);
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0, -1.0);
		
		glBegin(GL_QUADS);
		glVertex3f(xx + cubeCoords[(selectedCube->selectedFace() * 12) + 0], yy + cubeCoords[(selectedCube->selectedFace() * 12) + 1], zz + cubeCoords[(selectedCube->selectedFace() * 12) + 2]);
		glVertex3f(xx + cubeCoords[(selectedCube->selectedFace() * 12) + 3], yy + cubeCoords[(selectedCube->selectedFace() * 12) + 4], zz + cubeCoords[(selectedCube->selectedFace() * 12) + 5]);
		glVertex3f(xx + cubeCoords[(selectedCube->selectedFace() * 12) + 6], yy + cubeCoords[(selectedCube->selectedFace() * 12) + 7], zz + cubeCoords[(selectedCube->selectedFace() * 12) + 8]);
		glVertex3f(xx + cubeCoords[(selectedCube->selectedFace() * 12) + 9], yy + cubeCoords[(selectedCube->selectedFace() * 12) + 10], zz + cubeCoords[(selectedCube->selectedFace() * 12) + 11]);
		glEnd();
		
		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_ALPHA_TEST);
		glColor4ub(255, 255, 255, 255);
	}
}

Chunk *Map::findNearestChunk(float x, float y, float z) {
	float distance = FAR;
	Chunk *chunk = NULL;
	
	for(vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		vect3D center;
		
		center.x = (*it)->x() + CHUNK_WIDTH / 2;
		center.y = (*it)->y() + CHUNK_DEPTH / 2;
		center.z = (*it)->z() + CHUNK_HEIGHT / 2;
		
		float d = sqrt(pow(center.x - x, 2) + pow(center.y - y, 2) + pow(center.z - z, 2));
		
		if(d < distance) {
			distance = d;
			chunk = (*it);
		}
	}
	
	return chunk;
}

bool Map::intersectionLinePlane(vect3D normal, vect3D planePoint, vect3D lineOrigPoint, vect3D directionVector, float *distance) {
	float p1 = directionVector.x * normal.x + directionVector.y * normal.y + directionVector.z * normal.z; // First point to be tested
	
	if(p1 == 0) return false; // Degenerate case
	
	vect3D u; // planePoint - lineOrigPoint
	
	u.x = planePoint.x - lineOrigPoint.x;
	u.y = planePoint.y - lineOrigPoint.y;
	u.z = planePoint.z - lineOrigPoint.z;
	
	float p2 = u.x * normal.x + u.y * normal.y + u.z * normal.z; // Second point to be tested
	
	float k = p2 / p1;
	
	if((k < 0) || (k > 5)) return false;
	
	vect3D i; // Intersection point
	
	i.x = lineOrigPoint.x + k * directionVector.x;
	i.y = lineOrigPoint.y + k * directionVector.y;
	i.z = lineOrigPoint.z + k * directionVector.z;
	
	vect3D v;
	
	v.x = i.x - planePoint.x;
	v.y = i.y - planePoint.y;
	v.z = i.z - planePoint.z;
	
	float size = 0.5;
	
	if(v.x >= -size && v.x <= size && v.y >= -size && v.y <= size && v.z >= -size && v.z <= size) {
		if(distance != NULL) *distance = k;
		return true;
	} else {
		return false;
	}
}

// Front right = 0 | Front left = 1
// Back right = 2 | Back left = 3
// Top = 4 | Bottom = 5
bool Map::intersectionLineCube(int cubeX, int cubeY, int cubeZ, vect3D lineOrigPoint, vect3D directionVector, float *distance, s8 *face) {
	vect3D planePoint;
	vect3D normal;
	
	float k = 0;
	float smallestK = FAR;
	s8 nearestFace = -1;
	bool result = false;
	
	// Front right	
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY + 1;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = 0;
	normal.y = 1;
	normal.z = 0;
	
	result = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector, &k);
	if(result && (smallestK > k)) {
		smallestK = k;
		nearestFace = 0;
	}
	
	// Front left
	planePoint.x = cubeX + 1;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = 1;
	normal.y = 0;
	normal.z = 0;
	
	result = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector, &k);
	if(result && (smallestK > k)) {
		smallestK = k;
		nearestFace = 1;
	}
	
	// Back left
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = 0;
	normal.y = -1;
	normal.z = 0;
	
	result = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector, &k);
	if(result && (smallestK > k)) {
		smallestK = k;
		nearestFace = 3;
	}
	
	// Back right
	planePoint.x = cubeX;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = -1;
	normal.y = 0;
	normal.z = 0;
	
	result = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector, &k);
	if(result && (smallestK > k)) {
		smallestK = k;
		nearestFace = 2;
	}
	
	// Bottom
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ;
	
	normal.x = 0;
	normal.y = 0;
	normal.z = -1;
	
	result = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector, &k);
	if(result && (smallestK > k)) {
		smallestK = k;
		nearestFace = 5;
	}
	
	// Top
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ + 1;
	
	normal.x = 0;
	normal.y = 0;
	normal.z = 1;
	
	result = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector, &k);
	if(result && (smallestK > k)) {
		smallestK = k;
		nearestFace = 4;
	}
	
	if(nearestFace < 0) {
		return false;
	} else {
		if(distance != NULL) *distance = smallestK;
		if(face != NULL) *face = nearestFace;
		return true;
	}
}

void Map::testCubes() {
	vect3D linePoint;
	
	linePoint.x = Game::player->x();
	linePoint.y = Game::player->y();
	linePoint.z = Game::player->z();
	
	vect3D directionVector;
	
	directionVector.x = Game::player->pointTargetedx() - Game::player->x();
	directionVector.y = Game::player->pointTargetedy() - Game::player->y();
	directionVector.z = Game::player->pointTargetedz() - Game::player->z();
	
	float distance = FAR;
	Cube *cube = NULL;
	int face = -1;
	Chunk *chunk = NULL;
	vector<Cube*> cubes;
	for(unsigned short i = 0 ; i < 7 ; i++) {
		if(i == 6) cubes = currentChunk->cubes();
		else {
			if(currentChunk->surroundingChunks()[i] == NULL) continue;
			cubes = currentChunk->surroundingChunks()[i]->cubes();
		}
		
		for(std::vector<Cube*>::iterator it = cubes.begin() ; it != cubes.end() ; it++) {
			(*it)->setSelected(false, -1);
			
			float d = -1;
			s8 f = -1;
			
			bool result = intersectionLineCube((*it)->x(), (*it)->y(), (*it)->z(), linePoint, directionVector, &d, &f);
			
			if(result && (d < distance) && (d < 5)) {
				distance = d;
				cube = (*it);
				face = f;
				if(i == 6) chunk = currentChunk;
				else chunk = currentChunk->surroundingChunks()[i];
			}
		}
	}
	
	if(cube != NULL) {
		selectedCube = cube;
		cube->setSelected(true, face);
		face = -1;
	} else {
		selectedCube->setSelected(false, -1);
	}
	
	if(chunk != NULL) {
		selectedChunk = chunk;
	} else {
		selectedChunk = NULL;
	}
}

