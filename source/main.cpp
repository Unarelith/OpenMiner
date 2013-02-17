#include <string>
#include <map>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "sdlglutils.h"

#include "types.h"
#include "init.h"
#include "map.h"
#include "cube.h"
#include "biome.h"
#include "player.h"
#include "scene.h"

int main(int argc, char *argv[]) {
	// Init SDL and OpenGL
	initSDL();
	initOpenGL();
	
	// Scene execution
	Scene scene;
	scene.exec();
	
	// Stop SDL
	SDL_Quit();
	
	return 0;
}

