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
#include <GL/glew.h>

#include <png.h>

#include "types.h"
#include "config.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "player.h"
#include "game.h"

#include "genutils.h"
#include "texutils.h"

using namespace std;

Chunk *Map::currentChunk = NULL;
Chunk *Map::selectedChunk = NULL;
Cube *Map::selectedCube = NULL;

Map::Map(u16 width, u16 depth, u16 height) {
	m_width = width;
	m_depth = depth;
	m_height = height;
	
	glGenTextures(1, &m_texture);
	m_texture = loadPngTexture("textures/blocks.png");
	
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
					float perlin = get2DPerlinNoiseValue(x, y, 64) * 3 // 3 - 0
								 + get2DPerlinNoiseValue(x, y, 32) * 2 // 0 - 4
								 + get2DPerlinNoiseValue(x, y, 16) * 2 // 0.5 - 3
								 + get2DPerlinNoiseValue(x, y, 8) * 1; // 0.1 - 2
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
				m_chunks[CHUNK_POS(x, y, z)] = new Chunk(x << 3, y << 3, z << 3);
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
	
	glDeleteTextures(1, &m_texture);
}

void Map::render() {
	glColor3ub(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(0, 0, -0.01);
		glVertex3f(m_width, 0, -0.01);
		glVertex3f(m_width, m_depth, -0.01);
		glVertex3f(0, m_depth, -0.01);
	glEnd();
	glColor3ub(255, 255, 255);
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	currentChunk = findNearestChunk(Game::player->x(), Game::player->y(), Game::player->z());
	
	//uint32_t time = SDL_GetTicks();
	
	for(int i = 0 ; i < ((m_width >> 3) * (m_depth >> 3) * (m_height >> 3)); i++) {
		glPushMatrix();
		glTranslatef(float(m_chunks[i]->x()), float(m_chunks[i]->y()), float(m_chunks[i]->z()));
		m_chunks[i]->render();
		glPopMatrix();
	}
	
	//time = SDL_GetTicks() - time;
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
	float distance = DIST_FAR;
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

Chunk* Map::getChunk(int x, int y, int z) {
	if ((x < 0) || (x >= m_width >> 3) || (y < 0) || (y >= m_depth >> 3) || (z < 0) || (z >= m_height >> 3)) {
		return NULL;
	}
	
	int coords = x + (y * (m_width >> 3)) + (z * (m_width >> 3) * (m_depth >> 3));
	
	return m_chunks[coords];
}

bool Map::intersectionLinePlane(vect3D normal, vect3D planePoint, vect3D lineOrigPoint, vect3D directionVector, float *distance) {
	float p1 = directionVector.x * normal.x + directionVector.y * normal.y + directionVector.z * normal.z; // First point to be tested
	
	if(p1 == 0) return false; // Degenerate case
	
	vect3D u = vect3D(planePoint.x - lineOrigPoint.x,
					  planePoint.y - lineOrigPoint.y,
					  planePoint.z - lineOrigPoint.z);
	
	float p2 = u.x * normal.x + u.y * normal.y + u.z * normal.z; // Second point to be tested
	
	float k = p2 / p1;
	
	if((k < 0) || (k > 5)) return false;
	
	// Intersection point
	vect3D i = vect3D(lineOrigPoint.x + k * directionVector.x,
					  lineOrigPoint.y + k * directionVector.y,
					  lineOrigPoint.z + k * directionVector.z);
	
	vect3D v = vect3D(i.x - planePoint.x,
					  i.y - planePoint.y,
					  i.z - planePoint.z);
	
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
	vect3D planePoint[6] = {
		vect3D(cubeX + 0.5, cubeY + 1, cubeZ + 0.5), // back
		vect3D(cubeX + 1, cubeY + 0.5, cubeZ + 0.5), // right
		vect3D(cubeX, cubeY + 0.5, cubeZ + 0.5), // left
		vect3D(cubeX + 0.5, cubeY, cubeZ + 0.5), // front
		vect3D(cubeX + 0.5, cubeY + 0.5, cubeZ + 1), // top
		vect3D(cubeX + 0.5, cubeY + 0.5, cubeZ) // bottom
	};
	
	vect3D normal[6] = {
		vect3D(0, 1, 0), // back
		vect3D(1, 0, 0), // right
		vect3D(-1, 0, 0), // left
		vect3D(0, -1, 0), // front
		vect3D(0, 0, 1), // top
		vect3D(0, 0, -1) // bottom
	};
	
	float shortestDistance = DIST_FAR;
	float dist = DIST_FAR + 1.0;
	int nearestFace = -1;
	
	for (int i = 0; i < 6; i++) {
		bool result = intersectionLinePlane(normal[i], planePoint[i], lineOrigPoint, directionVector, &dist);
		if (result && (dist < shortestDistance)) {
			shortestDistance = dist;
			nearestFace = i;
		}
	}
	
	if (nearestFace < 0) {
		return false;
	} else {
		if (distance != NULL) *distance = shortestDistance;
		if (face != NULL) *face = nearestFace;
		return true;
	}
}

void Map::testCubes() {
	vect3D linePoint = vect3D(Game::player->x(),
							  Game::player->y(),
							  Game::player->z());
	
	vect3D directionVector = vect3D(Game::player->pointTargetedx() - Game::player->x(),
									Game::player->pointTargetedy() - Game::player->y(),
									Game::player->pointTargetedz() - Game::player->z());
	
	float distance = DIST_FAR;
	Cube *cube = NULL;
	int face = -1;
	Chunk *chunk = NULL;
	Chunk *c = NULL;
	unordered_map<int, Cube*> *cubes;
	for(unsigned short i = 0 ; i < 7 + 20 ; i++) {
		if(i == 6) cubes = currentChunk->cubes();
		else if(i < 6) {
			if(currentChunk->surroundingChunks()[i] == NULL) continue;
			cubes = currentChunk->surroundingChunks()[i]->cubes();
		} else {
			int coords[20 * 3] = {
				1, 0, 1,	-1, 0, 1,	
				1, 1, 0,	-1, 1, 0,
				1, -1, 0,	-1, -1, 0,
				1, 1, 1,	-1, 1, 1,	
				1, -1, 1,	-1, -1, 1,
				1, 0, -1,	-1, 0, -1,
				1, 1, -1,	-1, 1, -1,
				1, -1, -1,	-1, -1, -1,
				0, 1, 1,	0, -1, 1,
				0, 1, -1,	0, -1, -1
			};
			
			c = getChunk((currentChunk->x() >> 3) + coords[(i-7)*3],
						 (currentChunk->y() >> 3) + coords[(i-7)*3 + 1],
						 (currentChunk->z() >> 3) + coords[(i-7)*3 + 2]);
			
			if(c == NULL) continue;
			
			cubes = c->cubes();
		}
		
		for(std::unordered_map<int, Cube*>::iterator it = cubes->begin() ; it != cubes->end() ; it++) {
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
				else if(i < 6) chunk = currentChunk->surroundingChunks()[i];
				else if(c != NULL) chunk = c;
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

