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

Player *Scene::player;

bool Scene::intersectionLineSphere(vect3D center, float radius, vect3D linePoint, vect3D directionVector) {
	vect3D u;
	
	u.x = center.x - linePoint.x;
	u.y = center.y - linePoint.y;
	u.z = center.z - linePoint.z;
	
	float k = u.x * directionVector.x + u.y * directionVector.y + u.z * directionVector.z;
	
	vect3D h;
	
	h.x = linePoint.x + k * directionVector.x;
	h.y = linePoint.y + k * directionVector.y;
	h.z = linePoint.z + k * directionVector.z;
	
	if(sqrt(pow(h.x - center.x, 2) + pow(h.y - center.y, 2) + pow(h.z - center.z, 2)) < radius) {
		return true;
	} else {
		return false;
	}
}

bool intersectionLinePlane(vect3D normal, vect3D planePoint, vect3D lineOrigPoint, vect3D directionVector) {
	float p1 = directionVector.x * normal.x + directionVector.y * normal.y + directionVector.z * normal.z; // First point to be tested
	
	if(p1 == 0) return false; // Degenerate case
	
	vect3D u; // planePoint - lineOrigPoint
	
	u.x = planePoint.x - lineOrigPoint.x;
	u.y = planePoint.y - lineOrigPoint.y;
	u.z = planePoint.z - lineOrigPoint.z;
	
	float p2 = u.x * normal.x + u.y * normal.y + u.z * normal.z; // Second point to be tested
	
	float k = p1 / p2;
	
	vect3D i; // Intersection point
	
	i.x = lineOrigPoint.x + k * directionVector.x;
	i.y = lineOrigPoint.y + k * directionVector.y;
	i.z = lineOrigPoint.z + k * directionVector.z;
}

void Scene::testCubes(std::vector<Cube*> cubes) {
	float radius = sqrt(3) / 2;
	
	vect3D linePoint;
	
	linePoint.x = Scene::player->x();
	linePoint.y = Scene::player->y();
	linePoint.z = Scene::player->z();
	
	vect3D directionVector;
	
	directionVector.x = Scene::player->pointTargetedx() - Scene::player->x();
	directionVector.y = Scene::player->pointTargetedy() - Scene::player->y();
	directionVector.z = Scene::player->pointTargetedz() - Scene::player->z();
	
	float distance = FAR;
	Cube *cube = cubes[0];
	for(std::vector<Cube*>::iterator it = cubes.begin() ; it != cubes.end() ; it++) {
		vect3D center;
		
		center.x = (*it)->x() + radius;
		center.y = (*it)->y() + radius;
		center.z = (*it)->z() + radius;
		
		if(intersectionLineSphere(center, radius, linePoint, directionVector)) {
			float d = sqrt(pow(linePoint.x - center.x, 2) + pow(linePoint.y - center.y, 2) + pow(linePoint.z - center.z, 2));
			
			if(d < distance) {
				distance = d;
				cube = (*it);
			}
		}
		
		(*it)->setSelected(false);
	}
	
	cube->setSelected(true);
}

Scene::Scene() {
	//player = new Player(7, 7, 5, 90);
	player = new Player(4, 4, 2, 90);
	
	loadTextures();
	
	m_biome = new Biome(0, 0, 0, m_textures["stone"]);
}

Scene::~Scene() {
	// Deleting loaded textures
	for(Textures::iterator element = m_textures.begin() ; element != m_textures.end() ; element++) {
		glDeleteTextures(1, &element->second);
		element->second = 0;
	}
	
	delete m_biome;
	delete player;
}

void Scene::exec() {
	lockMouse();
	
	m_cont = true;
	
	while(m_cont) {
		manageEvents();
		animate();
		draw();
		display();
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
	
	m_biome->draw();
	
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

