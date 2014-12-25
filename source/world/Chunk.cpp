/*
 * =====================================================================================
 *
 *       Filename:  Chunk.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 17:31:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <glm/gtc/noise.hpp>

#include "Chunk.hpp"

Chunk::Chunk(s32 x, s32 y, s32 z) {
	m_x = x;
	m_y = y;
	m_z = z;
	
	m_texture.load("textures/cobblestone.bmp");
	
	m_changed = false;
	m_initialized = false;
	m_generated = false;
	
	m_surroundingChunks[0] = nullptr;
	m_surroundingChunks[1] = nullptr;
	m_surroundingChunks[2] = nullptr;
	m_surroundingChunks[3] = nullptr;
}

Chunk::~Chunk() {
}

void Chunk::generate() {
	if(m_generated) return;
	else m_generated = true;
	
	time_t seed = time(NULL);
	
	for(u8 z = 0 ; z < depth ; z++) {
		for(u8 x = 0 ; x < width ; x++) {
			float n = noise2d((x + m_x * width) / 256.0, (z + m_z * depth) / 256.0, seed, 5, 0.5) * 4;
			float h = 10 + n * 2;
			
			for(u8 y = 0 ; y < height ; y++) {
				if(y + m_y * height < h) {
					m_data.push_back(1);
				} else {
					m_data.push_back(0);
				}
				
				// Random value used to determine land type
				/*float r = noise3d_abs((x + m_x * 32) / 16.0, (y + m_y * 32) / 16.0, (z + m_z * CZ) / 16.0, -seed, 2, 1);
				
				// Sand layer
				if(n + r * 5 < 4) {
					//m_blocks[x][y][z] = 7;
					m_data.push_back(1);
				}
				// Dirt layer, but use grass blocks for the top
				else if(n + r * 5 < 8) {
					//m_blocks[x][y][z] = (h < 4 || y + m_y * 32 < h - 1) ? 1 : 3;
					m_data.push_back(1);
				}
				// Rock layer
				else if(r < 1.25) {
					//m_blocks[x][y][z] = 6;
					m_data.push_back(1);
				// Sometimes, ores!
				} else {
					//m_blocks[x][y][z] = 11;
					m_data.push_back(1);
				}*/
			}
		}
	}
	
	m_changed = true;
}

#include "Debug.hpp"
#include "GameClock.hpp"

void Chunk::update() {
	m_changed = false;
	
	static const float cubeCoords[6 * 4 * 3] = {
		0, 1, 1,
		0, 1, 0,
		0, 0, 0,
		0, 0, 1,
		
		1, 0, 1,
		1, 0, 0,
		1, 1, 0,
		1, 1, 1,
		
		0, 0, 0,
		1, 0, 0,
		1, 0, 1,
		0, 0, 1,
		
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,
		
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1,
		
		0, 1, 0,
		1, 1, 0,
		1, 0, 0,
		0, 0, 0,
	};
	
	float texCoords[2 * 4 * 6] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	
	for(int i = 1 ; i < 6 ; i++) {
		memcpy(&texCoords[2 * 4 * i], &texCoords[0], 2 * 4 * sizeof(float));	
	}
	
	m_vertices.clear();
	m_normals.clear();
	m_texCoords.clear();
	
	m_vertices.reserve(width * height * depth * 6 * 4 * 3);
	m_normals.reserve(width * height * depth * 6 * 4 * 3);
	m_texCoords.reserve(width * height * depth * 6 * 4 * 2);
	
	m_verticesID.clear();
	m_extendedFaces.clear();
	
	// Needed in the loop
	glm::vec3 a, b, c, v1, v2, normal;
	
	for(u8 z = 0 ; z < depth ; z++) {
		for(u8 y = 0 ; y < height ; y++) {
			for(u8 x = 0 ; x < width ; x++) {
				if(!getBlock(x, y, z)) {
					continue;
				}
				
				for(u8 i = 0 ; i < 6 ; i++) {
					// Skip hidden faces
					if((x > 0           && getBlock(x - 1, y, z) && i == 0)
					|| (x < width - 1   && getBlock(x + 1, y, z) && i == 1)
					|| (y > 0           && getBlock(x, y - 1, z) && i == 2)
					|| (y < height - 1  && getBlock(x, y + 1, z) && i == 3)
					|| (z > 0           && getBlock(x, y, z - 1) && i == 5)
					|| (z < depth - 1   && getBlock(x, y, z + 1) && i == 4)
					|| (x == 0          && m_surroundingChunks[0] && m_surroundingChunks[0]->getBlock(width - 1, y, z)  && i == 0)
					|| (x == width - 1  && m_surroundingChunks[1] && m_surroundingChunks[1]->getBlock(0, y, z)          && i == 1)
					|| (z == 0          && m_surroundingChunks[2] && m_surroundingChunks[2]->getBlock(x, y, depth - 1)  && i == 5)
					|| (z == depth - 1  && m_surroundingChunks[3] && m_surroundingChunks[3]->getBlock(x, y, 0)          && i == 4)
					) {
						continue;
					}
					
					// Merge adjacent faces
					if(x > 0 && getBlock(x - 1, y, z) && (i != 0 || i != 1) && vertexExists(x - 1, y, z, i, 0)) {
						m_vertices[getVertexID(x - 1, y, z, i, 1, 0)] += 1;
						m_vertices[getVertexID(x - 1, y, z, i, 2, 0)] += 1;
						
						m_texCoords[getTexCoordID(x - 1, y, z, i, 1, 0)] += 1;
						m_texCoords[getTexCoordID(x - 1, y, z, i, 2, 0)] += 1;
						
						m_extendedFaces[getCoordID(x, y, z, i, 0, 0)] = getCoordID(x - 1, y, z, i, 0, 0);
						
						continue;
					}
					if(z > 0 && getBlock(x, y, z - 1) && (i == 0 || i == 1) && vertexExists(x, y, z - 1, i, 0)) {
						m_vertices[getVertexID(x, y, z - 1, i, 0, 2)] += 1;
						m_vertices[getVertexID(x, y, z - 1, i, 3, 2)] += 1;
						
						m_texCoords[getTexCoordID(x, y, z - 1, i, 1, 0)] += 1;
						m_texCoords[getTexCoordID(x, y, z - 1, i, 2, 0)] += 1;
						
						m_extendedFaces[getCoordID(x, y, z, i, 0, 0)] = getCoordID(x, y, z - 1, i, 0, 0);
						
						continue;
					}
					
					/*if(y > 0 && getBlock(x, y - 1, z) && (i == 2 || i == 3) && vertexExists(x, y - 1, z, i, 0)) {
						m_vertices[getVertexID(x, y - 1, z, i, 0, 1)] += 1;
						m_vertices[getVertexID(x, y - 1, z, i, 3, 1)] += 1;
						
						m_texCoords[getTexCoordID(x, y - 1, z, i, 1, 1)] += 1;
						m_texCoords[getTexCoordID(x, y - 1, z, i, 2, 1)] += 1;
						
						m_extendedFaces[getCoordID(x, y, z, i, 0, 0)] = getCoordID(x, y - 1, z, i, 0, 0);
						
						continue;
					}*/
					
					// Three points of the face
					a.x = cubeCoords[i * 12 + 0];
					a.y = cubeCoords[i * 12 + 1];
					a.z = cubeCoords[i * 12 + 2];
					
					b.x = cubeCoords[i * 12 + 3];
					b.y = cubeCoords[i * 12 + 4];
					b.z = cubeCoords[i * 12 + 5];
					
					c.x = cubeCoords[i * 12 + 6];
					c.y = cubeCoords[i * 12 + 7];
					c.z = cubeCoords[i * 12 + 8];
					
					// Computing two vectors
					v1 = b - a;
					v2 = c - a;
					
					// Computing face normal (already normalized because cubeCoords are normalized)
					normal = glm::cross(v1, v2);
					
					// Store vertex information
					for(u8 j = 0 ; j < 4 ; j++) {
						m_vertices.push_back(x + cubeCoords[i * 12 + j * 3]);
						m_vertices.push_back(y + cubeCoords[i * 12 + j * 3 + 1]);
						m_vertices.push_back(z + cubeCoords[i * 12 + j * 3 + 2]);
						
						m_normals.push_back(normal.x);
						m_normals.push_back(normal.y);
						m_normals.push_back(normal.z);
						
						m_texCoords.push_back(texCoords[i * 8 + j * 2]);
						m_texCoords.push_back(texCoords[i * 8 + j * 2 + 1]);
						
						m_verticesID[getCoordID(x, y, z, i, j, 0)] = m_vertices.size() - 3;
						m_verticesID[getCoordID(x, y, z, i, j, 1)] = m_vertices.size() - 2;
						m_verticesID[getCoordID(x, y, z, i, j, 2)] = m_vertices.size() - 1;
					}
				}
			}
		}
	}
	
	m_vertices.shrink_to_fit();
	m_normals.shrink_to_fit();
	m_texCoords.shrink_to_fit();
	
	VertexBuffer::bind(&m_vbo);
	
	m_vbo.setData((m_vertices.size() + m_normals.size() + m_texCoords.size()) * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	m_vbo.updateData(0, m_vertices.size() * sizeof(float), m_vertices.data());
	m_vbo.updateData(m_vertices.size() * sizeof(float), m_normals.size() * sizeof(float), m_normals.data());
	m_vbo.updateData((m_vertices.size() + m_normals.size()) * sizeof(float), m_texCoords.size() * sizeof(float), m_texCoords.data());
	
	VertexBuffer::bind(nullptr);
}

void Chunk::draw(Shader &shader) {
	if(m_changed) update();
	
	if(m_vertices.size() == 0) return;
	
	VertexBuffer::bind(&m_vbo);
	
	shader.enableVertexAttribArray("coord3d");
	shader.enableVertexAttribArray("normal");
	shader.enableVertexAttribArray("texCoord");
	
	glVertexAttribPointer(shader.attrib("coord3d"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(shader.attrib("normal"), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m_vertices.size() * sizeof(float)));
	glVertexAttribPointer(shader.attrib("texCoord"), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((m_vertices.size() + m_normals.size()) * sizeof(float)));
	
	Texture::bind(&m_texture);
	
	glDrawArrays(GL_QUADS, 0, m_vertices.size() / 3);
	
	Texture::bind(nullptr);
	
	/*for(u32 i = 0 ; i < m_vertices.size() / 3 ; i += 4) {
		glDrawArrays(GL_LINE_LOOP, i, 4);
	}*/
	
	shader.disableVertexAttribArray("texCoord");
	shader.disableVertexAttribArray("normal");
	shader.disableVertexAttribArray("coord3d");
	
	VertexBuffer::bind(nullptr);
}

u8 Chunk::getBlock(s8 x, s8 y, s8 z) {
	u16 i = y + x * height + z * height * width;
	if(i < m_data.size()) {
		return m_data[i];
	} else {
		return 0;
	}
}

u32 Chunk::getCoordID(u8 x, u8 y, u8 z, u8 i, u8 j, u8 coordinate) {
	return (j + i * 4 + x * 4 * 6 + y * 4 * 6 * Chunk::width + z * 4 * 6 * Chunk::width * Chunk::height) * 3 + coordinate;
}

u32 Chunk::getVertexID(u8 x, u8 y, u8 z, u8 i, u8 j, u8 coordinate) {
	u32 id = getCoordID(x, y, z, i, j, coordinate);
	
	while(m_extendedFaces.count(id - j * 3 - coordinate)) {
		id = m_extendedFaces[id - j * 3 - coordinate] + j * 3 + coordinate;
	}
	
	return m_verticesID[id];
}

u32 Chunk::getTexCoordID(u8 x, u8 y, u8 z, u8 i, u8 j, u8 coordinate) {
	return float(getVertexID(x, y, z, i, j, coordinate)) / 3 * 2;
}

bool Chunk::vertexExists(u8 x, u8 y, u8 z, u8 i, u8 j) {
	return (m_verticesID.count(getCoordID(x, y, z, i, j, 0))
	     || m_extendedFaces.count(getCoordID(x, y, z, i, j, 0)));
}

float Chunk::noise2d(float x, float y, int seed, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;
	
	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * glm::simplex(glm::vec2(x, y) * scale);
		scale *= 2.0;
		strength *= persistence;
	}
	
	return sum;
}

float Chunk::noise3d_abs(float x, float y, float z, int seed, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;
	
	for(int i = 0 ; i < octaves ; i++) {
		sum += strength * fabs(glm::simplex(glm::vec3(x, y, z) * scale));
		scale *= 2.0;
		strength *= persistence;
	}
	
	return sum;
}

