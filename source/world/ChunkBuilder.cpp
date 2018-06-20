/*
 * =====================================================================================
 *
 *       Filename:  ChunkBuilder.cpp
 *
 *    Description:
 *
 *        Created:  19/06/2018 22:28:52
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Chunk.hpp"
#include "ChunkBuilder.hpp"
#include "Vertex.hpp"

std::size_t ChunkBuilder::buildChunk(const Chunk &chunk, const VertexBuffer &vbo) {
	static const float cubeCoords[6 * 4 * 3] = {
		// Left
		0, 0, 0,
		0, 0, 1,
		0, 1, 1,
		0, 1, 0,

		// Right
		1, 0, 1,
		1, 0, 0,
		1, 1, 0,
		1, 1, 1,

		// Bottom
		0, 0, 0,
		1, 0, 0,
		1, 0, 1,
		0, 0, 1,

		// Top
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,

		// Front
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,
		1, 1, 0,

		// Back
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1,
	};

	std::vector<Vertex> vertices;
	vertices.reserve(Chunk::width * Chunk::height * Chunk::depth * 6 * 4);

	// Needed in the loop (avoid a lot of glm::vec3 creation)
	glm::vec3 a, b, c, v1, v2, normal;

	for(u8 z = 0 ; z < Chunk::depth ; z++) {
		for(u8 y = 0 ; y < Chunk::height ; y++) {
			for(u8 x = 0 ; x < Chunk::width ; x++) {
				Block *block = chunk.getBlock(x, y, z);
				if(!block || !block->id()) continue;

				Block *surroundingBlocks[6] = {
					chunk.getBlock(x - 1, y, z),
					chunk.getBlock(x + 1, y, z),
					chunk.getBlock(x, y - 1, z),
					chunk.getBlock(x, y + 1, z),
					chunk.getBlock(x, y, z - 1),
					chunk.getBlock(x, y, z + 1),
				};

				for(u8 i = 0 ; i < 6 ; i++) {
					// Skip hidden faces
					if(surroundingBlocks[i] && surroundingBlocks[i]->id()
					&& (surroundingBlocks[i]->isOpaque() || block->id() == surroundingBlocks[i]->id()))
						continue;

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

					const glm::vec4 &blockTexCoords = block->getTexCoords(i);
					float faceTexCoords[2 * 4] = {
						blockTexCoords.x, blockTexCoords.w,
						blockTexCoords.z, blockTexCoords.w,
						blockTexCoords.z, blockTexCoords.y,
						blockTexCoords.x, blockTexCoords.y
					};

					// Store vertex information
					for(u8 j = 0 ; j < 4 ; j++) {
						Vertex vertex;

						vertex.coord3d[0] = x + cubeCoords[i * 12 + j * 3];
						vertex.coord3d[1] = y + cubeCoords[i * 12 + j * 3 + 1];
						vertex.coord3d[2] = z + cubeCoords[i * 12 + j * 3 + 2];
						vertex.coord3d[3] = static_cast<GLfloat>(block->id());

						vertex.normal[0] = normal.x;
						vertex.normal[1] = normal.y;
						vertex.normal[2] = normal.z;

						vertex.texCoord[0] = faceTexCoords[j * 2];
						vertex.texCoord[1] = faceTexCoords[j * 2 + 1];

						vertices.emplace_back(vertex);
					}
				}
			}
		}
	}

	vertices.shrink_to_fit();

	VertexBuffer::bind(&vbo);
	vbo.setData(vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);

	return vertices.size();
}

