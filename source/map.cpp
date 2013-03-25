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
#include <unordered_map>
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

#include "genutils.h"

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
			}
		}
	}
	
	for (int yy = 0; yy < m_depth >> 3; yy++) {
		for (int xx = 0; xx < m_width >> 3; xx++) {
			for (int yC = 0; yC < CHUNK_DEPTH; yC++) {
				for (int xC = 0; xC < CHUNK_WIDTH; xC++) {
					int x = xx * CHUNK_WIDTH + xC;
					int y = yy * CHUNK_DEPTH + yC;
					float perlin = get2DPerlinNoiseValue(x, y, 64) * 3
								 + get2DPerlinNoiseValue(x, y, 32) * 0
								 + get2DPerlinNoiseValue(x, y, 16) * 0.5
								 + get2DPerlinNoiseValue(x, y, 8) * 0.1;
					int heightValue = int((perlin * float(CHUNK_HEIGHT)) + float(m_height / 2));
					
					if (heightValue < 0) heightValue = 0;
					else if (heightValue >= m_height) heightValue = m_height - 1;
					
					for(int zz = 0 ; zz < heightValue ; zz++) {
						int dirtHeight = (1.0 - rand()%10 / 100 - 0.20) * heightValue;
						if(zz < dirtHeight) m_map[_MAP_POS(xC + (xx << 3), yC + (yy << 3), zz)] = 1;
						else if(zz > dirtHeight && zz < dirtHeight + 3) m_map[_MAP_POS(xC + (xx << 3), yC + (yy << 3), zz)] = rand()%2 + 1;
						else m_map[_MAP_POS(xC + (xx << 3), yC + (yy << 3), zz)] = 2;
					}
				}
			}
		}
	}
	
	m_chunks = new Chunk*[(m_width >> 3) * (m_height >> 3) * (m_depth >> 3)];
	
	for(u16 z = 0 ; z < m_height >> 3 ; z++) {
		for(u16 y = 0 ; y < m_depth >> 3 ; y++) {
			for(u16 x = 0 ; x < m_width >> 3 ; x++) {
				m_chunks[CHUNK_POS(x, y, z)] = new Chunk(x << 3, y << 3, z << 3, m_textures);
			}
		}
	}
				
	for(u16 z = 0 ; z < m_height >> 3 ; z++) {
		for(u16 y = 0 ; y < m_depth >> 3 ; y++) {
			for(u16 x = 0 ; x < m_width >> 3 ; x++) {
				int chunkIndex = x + (y * (m_width >> 3)) + (z * (m_width >> 3) * (m_depth >> 3));
				if(x - 1 >= 0) m_chunks[chunkIndex]->setSurroundingChunk(0, m_chunks[chunkIndex - 1]);
				if(x + 1 < m_width >> 3) m_chunks[chunkIndex]->setSurroundingChunk(1, m_chunks[chunkIndex + 1]);
				if(y - 1 >= 0) m_chunks[chunkIndex]->setSurroundingChunk(2, m_chunks[chunkIndex - (m_width >> 3)]);
				if(y + 1 < m_depth >> 3) m_chunks[chunkIndex]->setSurroundingChunk(3, m_chunks[chunkIndex + (m_width >> 3)]);
				if(z + 1 < m_height >> 3) m_chunks[chunkIndex]->setSurroundingChunk(4, m_chunks[chunkIndex + ((m_width >> 3) * (m_depth >> 3))]);
				if(z - 1 >= 0) m_chunks[chunkIndex]->setSurroundingChunk(5, m_chunks[chunkIndex - ((m_width >> 3) * (m_depth >> 3))]);
			}
		}
	}
	
	int cubeCount = 0;
	
	for(int i = 0 ; i < ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)); i++) {
		for(s32 z = m_chunks[i]->z() ; z < m_chunks[i]->z() + CHUNK_HEIGHT ; z++) {
			for(s32 y = m_chunks[i]->y() ; y < m_chunks[i]->y() + CHUNK_DEPTH ; y++) {
				for(s32 x = m_chunks[i]->x() ; x < m_chunks[i]->x() + CHUNK_WIDTH ; x++) {
					if(m_map[_MAP_POS(x, y, z)] != 0) m_chunks[i]->addCube(new Cube(x, y, z, m_map[_MAP_POS(x, y, z)]));
					cubeCount++;
				}
			}
		}
	}
	
	uint32_t time = SDL_GetTicks();
	
	for(int i = 0 ; i < ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)); i++) {
		m_chunks[i]->refreshVBO();
		cout << "Chunks loaded: " << i+1 << "/" << ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)) << endl;
	}
	
	cout << "Chunks loading time: " << (SDL_GetTicks() - time) << " ms for " << cubeCount << " cubes" << endl;
	
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	selectedCube = new Cube(-1, -1, -1, 0);
}

Map::~Map() {
	free(m_map);
	
	for(int i = 0 ; i < ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)); i++) {
		if(m_chunks[i] != NULL) delete m_chunks[i];
	}
	
	delete[] m_chunks;
}

void Map::draw() {
	glColor3ub(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, -0.01);
		glVertex3f(m_width, 0, -0.01);
		glVertex3f(m_width, m_depth, -0.01);
		glVertex3f(0, m_depth, -0.01);
	glEnd();
	glColor3ub(255, 255, 255);
	
	glBindTexture(GL_TEXTURE_2D, m_textures["textures"]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	uint32_t time = SDL_GetTicks();
	
	for(int i = 0 ; i < ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)); i++) {
		glPushMatrix();
		glTranslatef(float(m_chunks[i]->x()), float(m_chunks[i]->y()), float(m_chunks[i]->z()));
		m_chunks[i]->render();
		glPopMatrix();
	}
	
	time = SDL_GetTicks() - time;
	//cout << "Render time: " << time << " ms" << endl;
	
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
	
	for(int i = 0 ; i < ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)); i++) {
		vect3D center;
		
		center.x = m_chunks[i]->x() + CHUNK_WIDTH / 2;
		center.y = m_chunks[i]->y() + CHUNK_DEPTH / 2;
		center.z = m_chunks[i]->z() + CHUNK_HEIGHT / 2;
		
		float d = sqrt(pow(center.x - x, 2) + pow(center.y - y, 2) + pow(center.z - z, 2));
		
		if(d < distance) {
			distance = d;
			chunk = m_chunks[i];
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
	unordered_map<int, Cube*> cubes;
	for(unsigned short i = 0 ; i < 7 ; i++) {
		if(i == 6) cubes = currentChunk->cubes();
		else {
			if(currentChunk->surroundingChunks()[i] == NULL) continue;
			cubes = currentChunk->surroundingChunks()[i]->cubes();
		}
		
		for(std::unordered_map<int, Cube*>::iterator it = cubes.begin() ; it != cubes.end() ; it++) {
			if(it->second == NULL) continue;
			
			it->second->setSelected(false, -1);
			
			float d = -1;
			s8 f = -1;
			
			bool result = intersectionLineCube(it->second->x(), it->second->y(), it->second->z(), linePoint, directionVector, &d, &f);
			
			if(result && (d < distance) && (d < 5)) {
				distance = d;
				cube = it->second;
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

