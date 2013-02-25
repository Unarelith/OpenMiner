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

#include "sdlglutils.h"

#include "init.h"
#include "config.h"
#include "types.h"
#include "player.h"
#include "map.h"
#include "cube.h"
#include "biome.h"
#include "scene.h"

using namespace std;

float Scene::intersectionLinePlane(vect3D normal, vect3D planePoint, vect3D lineOrigPoint, vect3D directionVector) {
	float p1 = directionVector.x * normal.x + directionVector.y * normal.y + directionVector.z * normal.z; // First point to be tested
	
	if(p1 == 0) return -1; // Degenerate case
	
	vect3D u; // planePoint - lineOrigPoint
	
	u.x = planePoint.x - lineOrigPoint.x;
	u.y = planePoint.y - lineOrigPoint.y;
	u.z = planePoint.z - lineOrigPoint.z;
	
	float p2 = u.x * normal.x + u.y * normal.y + u.z * normal.z; // Second point to be tested
	
	float k = p2 / p1;
	
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
		return k;
	} else {
		return -1;
	}
}

int face = -1;
// Front right = 0 | Front left = 1
// Back right = 2 | Back left = 3
// Top = 4 | Bottom = 5
float Scene::intersectionLineCube(int cubeX, int cubeY, int cubeZ, vect3D lineOrigPoint, vect3D directionVector) {
	vect3D planePoint;
	vect3D normal;
	
	float k = 0;
	float smallestK = -1;
	
	// Front right	
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY + 1;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = 0;
	normal.y = 1;
	normal.z = 0;
	
	k = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector);
	if(k >= 0.0 && ((smallestK <= 0.0) || (smallestK > k))) {
		smallestK = k;
		face = 0;
	}
	
	// Front left
	planePoint.x = cubeX + 1;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = 1;
	normal.y = 0;
	normal.z = 0;
	
	k = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector);
	if(k >= 0.0 && ((smallestK <= 0.0) || (smallestK > k))) {
		smallestK = k;
		face = 1;
	}
	
	// Back left
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = 0;
	normal.y = -1;
	normal.z = 0;
	
	k = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector);
	if(k >= 0.0 && ((smallestK <= 0.0) || (smallestK > k))) {
		smallestK = k;
		face = 3;
	}
	
	// Back right
	planePoint.x = cubeX;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ + 0.5;
	
	normal.x = -1;
	normal.y = 0;
	normal.z = 0;
	
	k = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector);
	if(k >= 0.0 && ((smallestK <= 0.0) || (smallestK > k))) {
		smallestK = k;
		face = 2;
	}
	
	// Bottom
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ;
	
	normal.x = 0;
	normal.y = 0;
	normal.z = -1;
	
	k = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector);
	if(k >= 0.0 && ((smallestK <= 0.0) || (smallestK > k))) {
		smallestK = k;
		face = 5;
	}
	
	// Top
	planePoint.x = cubeX + 0.5;
	planePoint.y = cubeY + 0.5;
	planePoint.z = cubeZ + 1;
	
	normal.x = 0;
	normal.y = 0;
	normal.z = 1;
	
	k = intersectionLinePlane(normal, planePoint, lineOrigPoint, directionVector);
	if(k >= 0.0 && ((smallestK <= 0.0) || (smallestK > k))) {
		smallestK = k;
		face = 4;
	}
	
	return smallestK;
}

void Scene::testCubes(std::vector<Cube*> cubes) {
	vect3D linePoint;
	
	linePoint.x = Scene::player->x();
	linePoint.y = Scene::player->y();
	linePoint.z = Scene::player->z();
	
	vect3D directionVector;
	
	directionVector.x = Scene::player->pointTargetedx() - Scene::player->x();
	directionVector.y = Scene::player->pointTargetedy() - Scene::player->y();
	directionVector.z = Scene::player->pointTargetedz() - Scene::player->z();
	
	float distance = FAR;
	Cube *cube = NULL;
	for(std::vector<Cube*>::iterator it = cubes.begin() ; it != cubes.end() ; it++) {
		(*it)->setSelected(false, -1);
		
		float d = intersectionLineCube((*it)->x() + biome->x(), (*it)->y() + biome->y(), (*it)->z() + biome->z(), linePoint, directionVector);
		
		if(d >= 0.0 && d < distance) {
			distance = d;
			cube = (*it);
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

Player *Scene::player;
Biome *Scene::biome;
Cube *Scene::selectedCube;

Scene::Scene() {
	//player = new Player(7, 7, 5, 90);
	player = new Player(4, 4, 2, 90);
	
	loadTextures();
	
	biome = new Biome(4, 4, 0, m_textures["stone"]);
	
	selectedCube = new Cube(-1, -1, -1, m_textures["dirt"], NULL);
}

Scene::~Scene() {
	// Deleting loaded textures
	for(Textures::iterator element = m_textures.begin() ; element != m_textures.end() ; element++) {
		glDeleteTextures(1, &element->second);
		element->second = 0;
	}
	
	delete biome;
	delete player;
	delete selectedCube;
}

void Scene::exec() {
	lockMouse();
	
	m_cont = true;
	
	while(m_cont) {
		manageEvents();
		animate();
		draw();
		display();
		cout << "Selected: (" << selectedCube->x() << ";" << selectedCube->y() << ";" << selectedCube->z() << ") face: " << (int)selectedCube->selectedFace() << endl;
	}
	
	unlockMouse();
}

void Scene::manageEvents() {
	SDL_Event event;
	
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				m_cont = false;
				break;
				
			case SDL_MOUSEMOTION:
				if((WIN_WIDTH / 2) != event.motion.x || (WIN_HEIGHT / 2) != event.motion.y) {
					player->turnH(-event.motion.xrel * 0.06);
					player->turnV(-event.motion.yrel * 0.06);
					
					SDL_WarpMouse((WIN_WIDTH / 2), (WIN_HEIGHT / 2));
				}
				break;
				
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					m_cont = false;
				}
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT) {
					// To fix for multiples biomes
					biome->deleteCube(selectedCube);
					selectedCube = new Cube(-1, -1, -1, 0, NULL);
				}
				break;
		}
	}
}

void Scene::animate() {
	// Read keys state
	u8 *keys = SDL_GetKeyState(NULL);
	
	float direction = 0.0;
	bool movement = false;
	
	// Forward
	if(keys[SDLK_z]) {
		movement = true;
		
		if(keys[SDLK_q]) direction = 45.0;
		else if(keys[SDLK_d]) direction = -45.0;
		
		else direction = 0.0;
	}
	
	// Back
	else if(keys[SDLK_s]) {
		movement = true;
		
		if(keys[SDLK_q]) direction = 135.0;
		else if(keys[SDLK_d]) direction = -135.0;
		
		else direction = 180.0;
	}
	
	if(movement == false) {
		// Left
		if(keys[SDLK_q]) {
			direction = 90.0;
			movement = true;
		}
		
		// Right
		else if(keys[SDLK_d]) {
			direction = -90.0;
			movement = true;
		}
	}
	
	if(keys[SDLK_SPACE]) {
		player->fly();
	}
	else if(keys[SDLK_LSHIFT]) {
		player->land();
	}
	
	if(movement) {
		#define MOVEMENT_SPEED (2.0f)
		
		float distance = 20 * MOVEMENT_SPEED / 1000.0f;
		
		player->move(distance, direction);
	}
}

void Scene::draw() {
	// Clean screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Put camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	player->watch();
	
	// Drawing field
	drawField();
}

void Scene::display() {
	glFlush();
	
	SDL_GL_SwapBuffers();
}

void Scene::loadTextures() {
	// Load textures
	m_textures["ground"] = loadTexture("textures/ground.bmp");
	m_textures["dirt"] = loadTexture("textures/dirt.bmp");
	m_textures["grass"] = loadTexture("textures/grass.bmp");
	m_textures["cobble"] = loadTexture("textures/cobble.bmp");
	m_textures["stone"] = loadTexture("textures/stone.bmp");
}

void Scene::drawField() {
	// Turn on textures
	glEnable(GL_TEXTURE_2D);
	
	// Select texture
	glBindTexture(GL_TEXTURE_2D, m_textures["ground"]);
	
	// Add texture to scene
	glBegin(GL_QUADS);
	
	glColor3ub(255, 255, 255);
		glTexCoord2d(0, 1); glVertex3d(0, 0, -0.1);
		glTexCoord2d(0, 0); glVertex3d(50, 0, -0.1);
		glTexCoord2d(1, 0); glVertex3d(50, 50, -0.1);
		glTexCoord2d(1, 1); glVertex3d(0, 50, -0.1);
	
	glEnd();
	
	biome->draw();
	
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
	
	glMatrixMode(GL_MODELVIEW);
	
	glBindTexture(GL_TEXTURE_2D, m_textures["dirt"]);
	
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2i(-10, 0);
		glVertex2i(10, 0);
		glVertex2i(0, -10);
		glVertex2i(0, 10);
	
	glEnd();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
}

void Scene::lockMouse() {
	SDL_WM_GrabInput(SDL_GRAB_ON);
	
	SDL_WarpMouse((WIN_WIDTH / 2), (WIN_HEIGHT / 2));
	
	SDL_ShowCursor(false);
}

void Scene::unlockMouse() {
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	
	SDL_ShowCursor(true);
}

