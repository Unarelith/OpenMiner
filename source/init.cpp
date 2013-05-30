/*---------------------------------------------------------------------------------
	
	KubKraft
	Copyright (C) 2013 Quentin Bazin <quent42340@gmail.com>
	
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

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "init.h"
#include "config.h"

void initSDL() {
	// Start SDL with video module
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error while initializing SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	// Creation of the window initialized to work with OpenGL
	SDL_Surface *screen = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 32, SDL_OPENGL);
	if(screen == 0) {
		fprintf(stderr, "Error while initializing display: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	// Label of application
	SDL_WM_SetCaption(APP_LABEL, NULL);
}

void initOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // Turn on hidden face tagging
	glCullFace(GL_BACK); // Hidden faces = back faces
	glFrontFace(GL_CCW); // Front face = Trigo way
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.9);
	glEnable(GL_ALPHA_TEST);
	
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, DIST_FAR - (4 * (CHUNK_WIDTH + CHUNK_DEPTH) / 2));
	glFogf(GL_FOG_END, DIST_FAR);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	GLfloat fog_c[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glFogfv(GL_FOG_COLOR, fog_c);
	glEnable(GL_FOG);
	
	// Clean screen RGBa color
	glClearColor(0.5, 0.5, 1.0, 0);
	
	// Window definition
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glRasterPos2f(0, 0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Visible area definition
	gluPerspective(WIN_FOV, (GLdouble)WIN_WIDTH / (GLdouble)WIN_HEIGHT, DIST_NEAR, DIST_FAR - (2 * (CHUNK_WIDTH + CHUNK_DEPTH) / 2));
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	std::cout << glGetString(GL_VERSION) << std::endl;
}

