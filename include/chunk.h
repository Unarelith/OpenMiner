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
#ifndef CHUNK_H
#define CHUNK_H

class Chunk {
	public:
		Chunk(int x, int y, int z, Textures textures);
		~Chunk();
		
		void setSurroundingChunk(unsigned char face, Chunk* chunk);
		
		void render();
		
		void deleteCube(Cube *cube);
		void addCube(Cube *selectedCube, int type = 2);
		
		void refreshVBO();
		
		int x() const { return m_x; }
		int y() const { return m_y; }
		int z() const { return m_z; }
		
		std::unordered_map<int, Cube*> cubes() const { return m_cubes; }
		
		Chunk **surroundingChunks() const { return m_surroundingChunks; }
		
	private:
		int m_x;
		int m_y;
		int m_z;
		
		GLuint m_texture;
		
		std::unordered_map<int, Cube*> m_cubes;
		Cube *getCube(int x, int y, int z);
		
		Chunk **m_surroundingChunks;
		
		GLuint m_vboVertices;
		GLuint m_vboTexCoords;
		GLuint m_vboColors;
		int m_vboVertexCount;
};

#endif // CHUNK_H
