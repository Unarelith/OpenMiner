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
#include <map>
#include <vector>
#include <cmath>

#include <SDL/SDL.h>
#include <GL/glfw.h>

#include "sdlglutils.h"

#include "types.h"
#include "init.h"
#include "map.h"
#include "cube.h"
#include "chunk.h"
#include "player.h"
#include "game.h"

using namespace std;

int main(int argc, char *argv[]) {
	glfwInit();
	
	// Init SDL and OpenGL
	initSDL();
	initOpenGL();
	
	// Game execution
	Game game;
	game.exec();
	
	// Stop SDL
	SDL_Quit();
	
	glfwTerminate();
	
	return 0;
}

