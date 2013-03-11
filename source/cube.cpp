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

#include "config.h"
#include "types.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "player.h"
#include "game.h"

using namespace std;

Cube::Cube(int x, int y, int z, GLuint texture, unsigned int type) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture = texture;
	
	m_type = type;
	
	m_selected = false;
	m_selectedFace = -1;
}

Cube::~Cube() {
}

