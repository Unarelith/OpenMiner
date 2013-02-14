#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "init.h"
#include "config.h"

SDL_Surface *screen = NULL;

void initSDL() {
	// Start SDL with video module
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error while initializing SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	// Creation of the window initialized to work with OpenGL
	screen = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 32, SDL_OPENGL);
	if(screen == 0) {
		fprintf(stderr, "Error while initializing display: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	
	// Label of application
	SDL_WM_SetCaption(APP_LABEL, NULL);
}

void initOpenGL() {
	glEnable(GL_DEPTH_TEST); // Turn on hidden face tagging
	glCullFace(GL_BACK); // Hidden faces = back faces
	glFrontFace(GL_CCW); // Front face = Trigo way
	
	// Clean screen RGBa color
	glClearColor(0.5, 0.5, 1.0, 0);
	
	// Window definition
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Visible area definition
	gluPerspective(VISION_ANGLE, (GLdouble)WIN_WIDTH / (GLdouble)WIN_HEIGHT, NEAR, FAR);
}

