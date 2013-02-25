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
#ifndef MAP_H
#define MAP_H

#define MAP_POS(x, y, z) ((x) + ((y) * m_map->width) + ((z) * m_map->width * m_map->depth))

typedef struct {
	u16 width;
	u16 depth;
	u16 height;
	u16 *map;
} Map;

#endif // MAP_H
