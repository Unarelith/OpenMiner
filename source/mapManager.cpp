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
#include <map>
#include <unordered_map>
#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>

#include "config.h"
#include "types.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "player.h"
#include "game.h"
#include "mapManager.h"

bool passableCubes[CUBE_TYPES] {
	true,					// 00: Void
	false,					// 01: Stone
	false, false, false,	// 02: Dirt
	false,					// 05: Bedrock
	false,					// 06: Obsidian
	false,					// 07: Gravel
	false,					// 08: Sand
	false,					// 09: Coal ore
	false,					// 10: Diamond ore
	false,					// 11: Emeraud ore
	false,					// 12: Gold ore
	false,					// 13: Iron ore
	false,					// 14: Lapis lazuli ore
	false					// 15: Redstone ore
};

bool passable(s16 caseX, s16 caseY, s16 caseZ) {
	int pos = MAP_POS(caseX, caseY, caseZ);
	if(pos > 0 && pos < Game::map->width() * Game::map->depth() * Game::map->height()) {
		int cubeid = Game::map->map()[pos];
		if(passableCubes[cubeid]) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

