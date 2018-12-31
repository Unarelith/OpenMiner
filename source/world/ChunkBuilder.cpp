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

std::array<std::size_t, ChunkBuilder::layers> ChunkBuilder::buildChunk(const Chunk &chunk, const std::array<gk::VertexBuffer, layers> &vbo) {
	for (u8 i = 0 ; i < layers ; ++i)
		m_vertices[i].reserve(Chunk::width * Chunk::height * Chunk::depth * 6 * 4);

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

	std::array<std::size_t, layers> verticesCount;
	for (u8 i = 0 ; i < layers ; ++i) {
		m_vertices[i].shrink_to_fit();

		gk::VertexBuffer::bind(&vbo[i]);
		vbo[i].setData(m_vertices[i].size() * sizeof(gk::Vertex), m_vertices[i].data(), GL_DYNAMIC_DRAW);
		gk::VertexBuffer::bind(nullptr);

		verticesCount[i] = m_vertices[i].size();

		m_vertices[i].clear();
	}

	return verticesCount;
}

void ChunkBuilder::addFace(u8 x, u8 y, u8 z, u8 i, const Chunk &chunk, const Block *block, const Block *surroundingBlock) {
	// Skip hidden faces
	if(surroundingBlock && surroundingBlock->id()
	&& (surroundingBlock->isOpaque() || (block->id() == surroundingBlock->id() && block->id() != BlockType::Leaves && ((i != 2 && i != 3) || block->id() != BlockType::PlankSlab))))
		return;

	static glm::vec3 a, b, c, v1, v2, normal;

	const gk::FloatBox boundingBox = block->boundingBox();

	// Three points of the face
	a.x = cubeCoords[i * 12 + 0] * boundingBox.width  + boundingBox.x;
	a.y = cubeCoords[i * 12 + 1] * boundingBox.height + boundingBox.y;
	a.z = cubeCoords[i * 12 + 2] * boundingBox.depth  + boundingBox.z;

	b.x = cubeCoords[i * 12 + 3] * boundingBox.width  + boundingBox.x;
	b.y = cubeCoords[i * 12 + 4] * boundingBox.height + boundingBox.y;
	b.z = cubeCoords[i * 12 + 5] * boundingBox.depth  + boundingBox.z;

	c.x = cubeCoords[i * 12 + 6] * boundingBox.width  + boundingBox.x;
	c.y = cubeCoords[i * 12 + 7] * boundingBox.height + boundingBox.y;
	c.z = cubeCoords[i * 12 + 8] * boundingBox.depth  + boundingBox.z;

	// Computing two vectors
	v1 = b - a;
	v2 = c - a;

	// Computing face normal (already normalized because vertexCoords are normalized)
	normal = glm::cross(v1, v2);

	const glm::vec4 &blockTexCoords = block->getTexCoords(i, chunk.getData(x, y, z));
	float faceTexCoords[2 * 4] = {
		blockTexCoords.x, blockTexCoords.w,
		blockTexCoords.z, blockTexCoords.w,
		blockTexCoords.z, blockTexCoords.y,
		blockTexCoords.x, blockTexCoords.y
	};

	// Store vertex information
	gk::Vertex vertices[4];
	for(u8 j = 0 ; j < 4 ; j++) {
		vertices[j].coord3d[0] = x + cubeCoords[i * 12 + j * 3]     * boundingBox.width  + boundingBox.x;
		vertices[j].coord3d[1] = y + cubeCoords[i * 12 + j * 3 + 1] * boundingBox.height + boundingBox.y;
		vertices[j].coord3d[2] = z + cubeCoords[i * 12 + j * 3 + 2] * boundingBox.depth  + boundingBox.z;
		vertices[j].coord3d[3] = i;

		vertices[j].normal[0] = normal.x;
		vertices[j].normal[1] = normal.y;
		vertices[j].normal[2] = normal.z;

		vertices[j].color[0] = 1.0;
		vertices[j].color[1] = 1.0;
		vertices[j].color[2] = 1.0;
		vertices[j].color[3] = 1.0;

		vertices[j].texCoord[0] = faceTexCoords[j * 2];
		vertices[j].texCoord[1] = faceTexCoords[j * 2 + 1];

		// int sunlight = chunk.lightmap().getSunlight(x, y, z);
		// if ((i == 0 || i == 1 || i == 4 || i == 5) && sunlight > 2)
		// 	vertices[j].lightValue[0] = sunlight - 2;
		// if (i == 4 && sunlight > 3)
		// 	vertices[j].lightValue[0] = sunlight - 3;
		// else

		if (Config::isSmoothLightingEnabled) {
			if (Config::isAmbientOcclusionEnabled)
				vertices[j].lightValue[0] = chunk.lightmap().getSunlight(x, y, z);
			else
				vertices[j].lightValue[0] = getLightForVertex(Light::Sun, x, y, z, i, j, chunk);

			vertices[j].lightValue[1] = getLightForVertex(Light::Torch, x, y, z, i, j, chunk);
		}
		else {
			vertices[j].lightValue[0] = chunk.lightmap().getSunlight(x, y, z);
			vertices[j].lightValue[1] = chunk.lightmap().getTorchlight(x, y, z);
		}

		if (Config::isAmbientOcclusionEnabled)
			vertices[j].ambientOcclusion = getAmbientOcclusion(x, y, z, i, j, chunk);
		else
			vertices[j].ambientOcclusion = 5;

		vertices[j].blockType = block->id();
	}

	auto addVertex = [&](u8 j) {
		if (block->id() == BlockType::Water)
			m_vertices[Layer::Liquid].emplace_back(vertices[j]);
		else if (block->id() == BlockType::Leaves)
			m_vertices[Layer::Other].emplace_back(vertices[j]);
		else
			m_vertices[Layer::Solid].emplace_back(vertices[j]);
	};

	if (vertices[0].ambientOcclusion + vertices[2].ambientOcclusion >
			vertices[1].ambientOcclusion + vertices[3].ambientOcclusion) {
		addVertex(0);
		addVertex(1);
		addVertex(3);
		addVertex(3);
		addVertex(1);
		addVertex(2);
	} else {
		addVertex(0);
		addVertex(1);
		addVertex(2);
		addVertex(2);
		addVertex(3);
		addVertex(0);
	}
}

u8 ChunkBuilder::getAmbientOcclusion(u8 x, u8 y, u8 z, u8 i, u8 j, const Chunk &chunk) {
	// s16 offsetX = 0;
	s16 offsetX = (
			(i == 0) ||
			(i == 2 && (j == 0 || j == 3)) ||
			(i == 3 && (j == 0 || j == 3)) ||
			(i == 4 && (j == 1 || j == 2)) ||
			(i == 5 && (j == 0 || j == 3))) ? -1 : 1;

	// s16 offsetZ = 0;
	s16 offsetZ = (
			(i == 4) ||
			(i == 0 && (j == 0 || j == 3)) ||
			(i == 1 && (j == 1 || j == 2)) ||
			(i == 2 && (j == 0 || j == 1)) ||
			(i == 3 && (j == 2 || j == 3))) ? -1 : 1;

	s16 offsetY = (
			(i == 2) ||
			(i == 0 && (j == 0 || j == 1)) ||
			(i == 1 && (j == 0 || j == 1)) ||
			(i == 4 && (j == 0 || j == 1)) ||
			(i == 5 && (j == 0 || j == 1))) ? 0 : 1;

	bool side1 = chunk.getBlock(x + offsetX, y + offsetY, z) != 0;
	bool side2 = chunk.getBlock(x, y + offsetY, z + offsetZ) != 0;
	bool corner = chunk.getBlock(x + offsetX, y + offsetY, z + offsetZ) != 0;

	if (side1 && side2)
		return 0;

	return 3 - (side1 + side2 + corner);
}

float ChunkBuilder::getAverageLight(Light light, u8 x, u8 y, u8 z, s8 offsetX, s8 offsetY, s8 offsetZ, const Chunk &chunk) {
	// FIXME: Air blocks have a light level of 0
	if (light == Light::Sun)
		return (chunk.lightmap().getSunlight(x,           y + offsetY, z)
		      + chunk.lightmap().getSunlight(x + offsetX, y + offsetY, z)
		      + chunk.lightmap().getSunlight(x,           y + offsetY, z + offsetZ)
		      + chunk.lightmap().getSunlight(x + offsetX, y + offsetY, z + offsetZ)) / 4;
	else
		return (chunk.lightmap().getTorchlight(x,           y + offsetY, z)
		      + chunk.lightmap().getTorchlight(x + offsetX, y + offsetY, z)
		      + chunk.lightmap().getTorchlight(x,           y + offsetY, z + offsetZ)
		      + chunk.lightmap().getTorchlight(x + offsetX, y + offsetY, z + offsetZ)) / 4;
}

float ChunkBuilder::getLightForVertex(Light light, u8 x, u8 y, u8 z, u8 i, u8 j, const Chunk &chunk) {
	float lightValues[4] = {
		getAverageLight(light, x, y, z, -1, 0, -1, chunk),
		getAverageLight(light, x, y, z, -1, 0,  1, chunk),
		getAverageLight(light, x, y, z,  1, 0, -1, chunk),
		getAverageLight(light, x, y, z,  1, 0,  1, chunk),
	};

	if (i == Face::Left) {
		if (j == 0)      return lightValues[0];
		else if (j == 1) return lightValues[1];
		else if (j == 2) return lightValues[1];
		else if (j == 3) return lightValues[0];
	}
	else if (i == Face::Front) {
		if (j == 0)      return lightValues[2];
		else if (j == 1) return lightValues[0];
		else if (j == 2) return lightValues[0];
		else if (j == 3) return lightValues[2];
	}
	else if (i == Face::Top) {
		if (j == 0)      return lightValues[1];
		else if (j == 1) return lightValues[3];
		else if (j == 2) return lightValues[2];
		else if (j == 3) return lightValues[0];
	}
	else if (i == Face::Right) {
		if (j == 0)      return lightValues[3];
		else if (j == 1) return lightValues[2];
		else if (j == 2) return lightValues[2];
		else if (j == 3) return lightValues[3];
	}
	else if (i == Face::Back) {
		if (j == 0)      return lightValues[1];
		else if (j == 1) return lightValues[3];
		else if (j == 2) return lightValues[3];
		else if (j == 3) return lightValues[1];
	}
	else if (i == Face::Bottom) {
		if (j == 0)      return lightValues[0];
		else if (j == 1) return lightValues[2];
		else if (j == 2) return lightValues[3];
		else if (j == 3) return lightValues[1];
	}

	if (light == Light::Sun)
		return chunk.lightmap().getSunlight(x, y, z);
	else
		return chunk.lightmap().getTorchlight(x, y, z);
}

