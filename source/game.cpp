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
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "game.h"

using namespace std;

Player *Game::player;
Map *Game::map;

Game::Game() {
	player = new Player(4, 4, 18, 90);
	
	loadTextures();
	
	map = new Map(16 << 3, 16 << 3, 8 << 3, m_textures); // No size implemented for the moment
}

Game::~Game() {
	// Deleting loaded textures
	for(Textures::iterator element = m_textures.begin() ; element != m_textures.end() ; element++) {
		glDeleteTextures(1, &element->second);
		element->second = 0;
	}
	
	delete player;
}

void Game::exec() {
	lockMouse();
	
	m_cont = true;
	m_paused = false;
	
	uint32_t lastTime = SDL_GetTicks();
	int nbFrames = 0;
	
	while(m_cont) {
		uint32_t currentTime = SDL_GetTicks();
		nbFrames++;
		if(currentTime - lastTime >= 1000) {
			cout << 1000/double(nbFrames) << " ms/frame" << endl;
			nbFrames = 0;
			lastTime += 1000;
		}
		
		manageEvents();
		if(!m_paused) {
			animate();
			draw();
		}
	}
	
	unlockMouse();
}

void Game::manageEvents() {
	SDL_Event event;
	
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				m_cont = false;
				break;
				
			case SDL_MOUSEMOTION:
				if(!m_paused && ((WIN_WIDTH / 2) != event.motion.x || (WIN_HEIGHT / 2) != event.motion.y)) {
					player->turnH(-event.motion.xrel * 0.06);
					player->turnV(-event.motion.yrel * 0.06);
					
					SDL_WarpMouse((WIN_WIDTH / 2), (WIN_HEIGHT / 2));
				}
				break;
				
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					m_cont = false;
				}
				if(event.key.keysym.sym == SDLK_BACKSPACE) {
					m_paused = true;
					unlockMouse();
				}
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				if(m_paused && (event.button.button == SDL_BUTTON_LEFT)) {
					m_paused = false;
					lockMouse();
				} else {
					if((event.button.button == SDL_BUTTON_LEFT) && (Map::selectedChunk != NULL)) {
						Map::selectedChunk->deleteCube(Map::selectedCube);
						Map::selectedCube = new Cube(-1, -1, -1, 0, 0);
					}
					if((event.button.button == SDL_BUTTON_RIGHT) && (Map::selectedChunk != NULL)) {
						Map::selectedChunk->addCube(Map::selectedCube);
					}
				}
				break;
		}
	}
}

void Game::animate() {
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

void Game::draw() {
	// Clean screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// Put camera
	player->watch();
	
	// Draw the map
	map->draw();
	
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	float ratio = floor(float(WIN_WIDTH) / float(WIN_HEIGHT));
	gluOrtho2D(-ratio * 40, ratio * 40, -30.0, 30.0);
	
	glMatrixMode(GL_MODELVIEW);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glColor3ub(255, 255, 255);
	glBegin(GL_LINES);
		glVertex2i(-1, 0);
		glVertex2i(1, 0);
		glVertex2i(0, -1);
		glVertex2i(0, 1);
	
	glEnd();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
	
	glFlush();
	SDL_GL_SwapBuffers();
}

void Game::loadTextures() {
	// Load textures
	m_textures["dirt"] = loadTexture("textures/dirt.bmp");
	m_textures["grass"] = loadTexture("textures/grass.bmp");
	m_textures["cobblestone"] = loadTexture("textures/cobblestone.bmp");
	m_textures["stone"] = loadTexture("textures/stone.bmp");
	m_textures["bedrock"] = loadTexture("textures/bedrock.bmp");
}

void Game::lockMouse() {
	SDL_WM_GrabInput(SDL_GRAB_ON);
	
	SDL_WarpMouse((WIN_WIDTH / 2), (WIN_HEIGHT / 2));
	
	SDL_ShowCursor(false);
}

void Game::unlockMouse() {
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	
	SDL_ShowCursor(true);
}

