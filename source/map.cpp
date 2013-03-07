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

Chunk *Map::currentChunk;
Cube *Map::selectedCube;

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
	
	for(u16 z = 0 ; z < m_height >> 4 ; z++) {
		for(u16 y = 0 ; y < m_depth >> 4 ; y++) {
			for(u16 x = 0 ; x < m_width >> 4 ; x++) {
				m_chunks.push_back(new Chunk(x << 4, y << 4, z << 4, m_textures));
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
	}
	
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	selectedCube = new Cube(-1, -1, -1, m_textures["dirt"], 0);
}

Map::~Map() {
	free(m_map);
	
	m_chunks.clear();
	
	delete currentChunk;
	delete selectedCube;
}

void Map::draw() {
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	for(vector<Chunk*>::iterator it = m_chunks.begin() ; it != m_chunks.end() ; it++) {
		// (*it)->draw();
		(*it)->render();
	}
	
	testCubes(currentChunk->cubes());
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

void Map::testCubes(std::vector<Cube*> cubes) {
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
	for(std::vector<Cube*>::iterator it = cubes.begin() ; it != cubes.end() ; it++) {
		(*it)->setSelected(false, -1);
		
		float d = -1;
		s8 f = -1;
		
		bool result = intersectionLineCube((*it)->x(), (*it)->y(), (*it)->z(), linePoint, directionVector, &d, &f);
		
		if(result && (d < distance) && (d < 5)) {
			distance = d;
			cube = (*it);
			face = f;
		}
	}
	
	if(cube != NULL) {
		selectedCube = cube;
		cube->setSelected(true, face);
		face = -1;
	} else {
		selectedCube->setSelected(false, -1);
	}
}

