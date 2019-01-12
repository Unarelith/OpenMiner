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
#include "ClientChunk.hpp"
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

static const float crossCoords[2 * 4 * 3] = {
	0, 0, 0,
	1, 0, 1,
	1, 1, 1,
	0, 1, 0,

	0, 0, 1,
	1, 0, 0,
	1, 1, 0,
	0, 1, 1,
};


std::array<std::size_t, ChunkBuilder::layers> ChunkBuilder::buildChunk(const ClientChunk &chunk, const std::array<gk::VertexBuffer, layers> &vbo) {
	for (u8 i = 0 ; i < layers ; ++i)
		m_vertices[i].reserve(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH * 6 * 4);

	for(u8 z = 0 ; z < CHUNK_DEPTH ; z++) {
		for(u8 y = 0 ; y < CHUNK_HEIGHT ; y++) {
			for(u8 x = 0 ; x < CHUNK_WIDTH ; x++) {
				const Block &block = Registry::getInstance().getBlock(chunk.getBlock(x, y, z));
				if(!block.id()) continue;
				if (!chunk.getBlock(x, y, z)) continue;

				const Block *surroundingBlocks[6] = {
					&Registry::getInstance().getBlock(chunk.getBlock(x - 1, y, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x + 1, y, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y - 1, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y + 1, z)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y, z - 1)),
					&Registry::getInstance().getBlock(chunk.getBlock(x, y, z + 1)),
				};

				if (block.drawType() == BlockDrawType::Solid) {
					for(u8 i = 0 ; i < 6 ; i++) {
						addFace(x, y, z, i, chunk, &block, surroundingBlocks[i]);
					}
				}
				else if (block.drawType() == BlockDrawType::XShape) {
					addCross(x, y, z, chunk, &block);
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

void ChunkBuilder::addFace(u8 x, u8 y, u8 z, u8 i, const ClientChunk &chunk, const Block *block, const Block *surroundingBlock) {
	// Skip hidden faces
	if(surroundingBlock && surroundingBlock->id() && surroundingBlock->drawType() == BlockDrawType::Solid
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
		vertices[j].coord3d[0] = x + cubeCoords[i * 12 + j * 3];     // * boundingBox.width  + boundingBox.x;
		vertices[j].coord3d[1] = y + cubeCoords[i * 12 + j * 3 + 1]; // * boundingBox.height + boundingBox.y;
		vertices[j].coord3d[2] = z + cubeCoords[i * 12 + j * 3 + 2]; // * boundingBox.depth  + boundingBox.z;
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
		// FIXME
		// if (block->id() == BlockType::Water)
		// 	m_vertices[Layer::Liquid].emplace_back(vertices[j]);
		// else if (block->id() == BlockType::Leaves)
		// 	m_vertices[Layer::Other].emplace_back(vertices[j]);
		// else
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

void ChunkBuilder::addCross(u8 x, u8 y, u8 z, const ClientChunk &chunk, const Block *block) {
	const glm::vec4 &blockTexCoords = block->getTexCoords(0, chunk.getData(x, y, z));
	float faceTexCoords[2 * 4] = {
		blockTexCoords.x, blockTexCoords.w,
		blockTexCoords.z, blockTexCoords.w,
		blockTexCoords.z, blockTexCoords.y,
		blockTexCoords.x, blockTexCoords.y,
	};

	for (int i = 0 ; i < 2 ; ++i) {
		gk::Vertex vertices[4];
		for (int j = 0 ; j < 4 ; ++j) {
			vertices[j].coord3d[0] = x + crossCoords[i * 12 + j * 3];
			vertices[j].coord3d[1] = y + crossCoords[i * 12 + j * 3 + 1];
			vertices[j].coord3d[2] = z + crossCoords[i * 12 + j * 3 + 2];
			vertices[j].coord3d[3] = i;

			vertices[j].normal[0] = 0;
			vertices[j].normal[1] = 0;
			vertices[j].normal[2] = 0;

			vertices[j].color[0] = 1.0;
			vertices[j].color[1] = 1.0;
			vertices[j].color[2] = 1.0;
			vertices[j].color[3] = 1.0;

			vertices[j].texCoord[0] = faceTexCoords[j * 2];
			vertices[j].texCoord[1] = faceTexCoords[j * 2 + 1];

			vertices[j].blockType = block->id();
		}

		m_vertices[Layer::Other].emplace_back(vertices[0]);
		m_vertices[Layer::Other].emplace_back(vertices[1]);
		m_vertices[Layer::Other].emplace_back(vertices[3]);
		m_vertices[Layer::Other].emplace_back(vertices[3]);
		m_vertices[Layer::Other].emplace_back(vertices[1]);
		m_vertices[Layer::Other].emplace_back(vertices[2]);
	}
}

gk::Vector3i ChunkBuilder::getOffsetFromVertex(u8 i, u8 j) {
	gk::Vector3i offset;
	offset.x = (
			(i == 0) ||
			(i == 2 && (j == 0 || j == 3)) ||
			(i == 3 && (j == 0 || j == 3)) ||
			(i == 4 && (j == 1 || j == 2)) ||
			(i == 5 && (j == 0 || j == 3))) ? -1 : 1;

	offset.z = (
			(i == 4) ||
			(i == 0 && (j == 0 || j == 3)) ||
			(i == 1 && (j == 1 || j == 2)) ||
			(i == 2 && (j == 0 || j == 1)) ||
			(i == 3 && (j == 2 || j == 3))) ? -1 : 1;

	offset.y = (
			(i == 2) ||
			(i == 0 && (j == 0 || j == 1)) ||
			(i == 1 && (j == 0 || j == 1)) ||
			(i == 4 && (j == 0 || j == 1)) ||
			(i == 5 && (j == 0 || j == 1))) ? 0 : 1;

	return offset;
}

u8 ChunkBuilder::getAmbientOcclusion(u8 x, u8 y, u8 z, u8 i, u8 j, const ClientChunk &chunk) {
	gk::Vector3i offset = getOffsetFromVertex(i, j);

	u16 blocks[3] = {
		chunk.getBlock(x + offset.x, y + offset.y, z),
		chunk.getBlock(x,            y + offset.y, z + offset.z),
		chunk.getBlock(x + offset.x, y + offset.y, z + offset.z)
	};

	bool side1  = blocks[0] != 0 && blocks[0] != BlockType::Flower;
	bool side2  = blocks[1] != 0 && blocks[1] != BlockType::Flower;
	bool corner = blocks[2] != 0 && blocks[2] != BlockType::Flower;

	if (side1 && side2)
		return 0;

	return 3 - (side1 + side2 + corner);
}

float ChunkBuilder::getLightForVertex(Light light, u8 x, u8 y, u8 z, u8 i, u8 j, const ClientChunk &chunk) {
	gk::Vector3i offset = getOffsetFromVertex(i, j);

	// FIXME: Air blocks have a light level of 0
	if (light == Light::Sun)
		return (chunk.lightmap().getSunlight(x,            y + offset.y, z)
		      + chunk.lightmap().getSunlight(x + offset.x, y + offset.y, z)
		      + chunk.lightmap().getSunlight(x,            y + offset.y, z + offset.z)
		      + chunk.lightmap().getSunlight(x + offset.x, y + offset.y, z + offset.z)) / 4.0f;
	else
		return (chunk.lightmap().getTorchlight(x,            y + offset.y, z)
		      + chunk.lightmap().getTorchlight(x + offset.x, y + offset.y, z)
		      + chunk.lightmap().getTorchlight(x,            y + offset.y, z + offset.z)
		      + chunk.lightmap().getTorchlight(x + offset.x, y + offset.y, z + offset.z)) / 4.0f;
}

