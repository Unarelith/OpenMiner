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
#include "Registry.hpp"

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

std::size_t ChunkBuilder::buildChunk(const Chunk &chunk, const VertexBuffer &vbo) {
	m_vertices.reserve(Chunk::width * Chunk::height * Chunk::depth * 6 * 4);

	for(u8 z = 0 ; z < Chunk::depth ; z++) {
		for(u8 y = 0 ; y < Chunk::height ; y++) {
			for(u8 x = 0 ; x < Chunk::width ; x++) {
				const Block &block = Registry::getInstance().getBlock(chunk.getBlock(x, y, z));
				if(!block.id()) continue;

				const Block *surroundingBlocks[6] = {
					&Registry::getInstance().getBlock(chunk.getBlock(x - 1, y, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x + 1, y, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y - 1, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y + 1, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y, z - 1)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y, z + 1)),
				};

				for(u8 i = 0 ; i < 6 ; i++) {
					addFace(x, y, z, i, chunk, &block, surroundingBlocks[i]);
				}
			}
		}
	}

	m_vertices.shrink_to_fit();

	VertexBuffer::bind(&vbo);
	vbo.setData(m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);

	std::size_t verticesCount = m_vertices.size();
	m_vertices.clear();
	return verticesCount;
}

void ChunkBuilder::addFace(u8 x, u8 y, u8 z, u8 i, const Chunk &chunk, const Block *block, const Block *surroundingBlock) {
	// Skip hidden faces
	if(surroundingBlock && surroundingBlock->id()
	&& (surroundingBlock->isOpaque() || (block->id() == surroundingBlock->id() && block->id() != BlockType::Leaves)))
		return;

	static glm::vec3 a, b, c, v1, v2, normal;

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

	const glm::vec4 &blockTexCoords = block->getTexCoords(i, chunk.getData(x, y, z));
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

		int sunlight = chunk.lightmap().getSunlight(x, y, z);
		if ((i == 0 || i == 1 || i == 4 || i == 5) && sunlight > 2)
			vertex.lightValue[0] = sunlight - 2;
		if (i == 4 && sunlight > 3)
			vertex.lightValue[0] = sunlight - 3;
		else
			vertex.lightValue[0] = sunlight;
		vertex.lightValue[1] = chunk.lightmap().getTorchlight(x, y, z);

		m_vertices.emplace_back(vertex);
	}
}

