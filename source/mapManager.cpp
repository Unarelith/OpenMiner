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

u16 nonPassableTiles[2] {
	1, 2
};

bool inTable(u16 t[], u16 n) {
	int i = 0;
	while(t[i]) {
		if(t[i] == n) {
			return true;
		}
		i++;
	}
	return false;
}

bool passable(s16 caseX, s16 caseY, s16 caseZ) {
	// TOO SLOW
	/*if(inTable(nonPassableTiles, Game::map->map()[MAP_POS(caseX, caseY, caseZ)])) {
		return false;
	} else {
		return true;
	}*/
	int cubeid = Game::map->map()[MAP_POS(caseX, caseY, caseZ)];
	if(cubeid == 1 || cubeid == 2) {
		return false;
	} else {
		return true;
	}
}

