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
#ifndef CONFIG_H
#define CONFIG_H

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_FOV 80

#define APP_LABEL "KubKraft"

#define DIST_NEAR 0.1
#define DIST_FAR 1000.

#define CHUNK_WIDTH 8
#define CHUNK_DEPTH 8
#define CHUNK_HEIGHT 8

#define MAP_POS(x, y, z) ((x) + ((y) * Game::mapWidth) + ((z) * Game::mapWidth * Game::mapDepth))
#define _MAP_POS(x, y, z) ((x) + ((y) * m_width) + ((z) * m_width * m_depth))
#define CHUNK_POS(x, y, z) ((x) + ((y) * (m_width >> 3)) + ((z) * (m_width >> 3) * (m_depth >> 3)))
#define CUBE_POS(x, y, z) (((x) - m_x) + (((y) - m_y) * CHUNK_WIDTH) + (((z) - m_z) * CHUNK_WIDTH * CHUNK_HEIGHT))

#define PLAYER_HEIGHT 1.8
#define MOVEMENT_SPEED 3.0
#define FLY_SPEED 0.05
#define JUMP_SPEED 0.5
#define GRAVITY 0.025

#endif // CONFIG_H
