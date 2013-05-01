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

typedef struct {
	float x;
	float y;
	float z;
} vect3D;

class Map {
	public:
		Map(u16 width, u16 depth, u16 height);
		~Map();
		
		void render();
		
		Chunk *findNearestChunk(float x, float y, float z);
		Chunk *getChunk(int x, int y, int z);
		
		bool intersectionLinePlane(vect3D normal, vect3D planePoint, vect3D lineOrigPoint, vect3D directionVector, float *distance);
		bool intersectionLineCube(int cubeX, int cubeY, int cubeZ, vect3D lineOrigPoint, vect3D directionVector, float *distance, s8 *face);
		void testCubes();
		
		u16 width() const { return m_width; }
		u16 height() const { return m_height; }
		u16 depth() const { return m_depth; }
		
		u16 *map() const { return m_map; }
		
		Chunk **chunks() const { return m_chunks; }
		
		static Chunk *currentChunk;
		static Chunk *selectedChunk;
		static Cube *selectedCube;
		
	private:
		u16 m_width;
		u16 m_height;
		u16 m_depth;
		
		GLuint m_texture;
		
		u16 *m_map;
		
		Chunk** m_chunks;
};

#endif // MAP_H
