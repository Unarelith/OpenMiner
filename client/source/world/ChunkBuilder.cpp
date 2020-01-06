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

				const int surroundingBlocksPos[6][3] = {
					{x - 1, y, z},
					{x + 1, y, z},
					{x, y - 1, z},
					{x, y + 1, z},
					{x, y, z - 1},
					{x, y, z + 1},
				};

				if (block.drawType() == BlockDrawType::Solid) {
					for(u8 i = 0 ; i < 6 ; i++) {
						addFace(x, y, z, i, chunk, &block, surroundingBlocksPos[i]);
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

void ChunkBuilder::addFace(u8 x, u8 y, u8 z, u8 i, const ClientChunk &chunk, const Block *block, const int surroundingBlockPos[3]) {
	// Get surrounding block for that face
	u16 surroundingBlockID = chunk.getBlock(surroundingBlockPos[0], surroundingBlockPos[1], surroundingBlockPos[2]);
	const Block *surroundingBlock = &Registry::getInstance().getBlock(surroundingBlockID);

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

		// FIXME: Duplicated below
		if (Config::isSunSmoothLightingEnabled)
			vertices[j].lightValue[0] = getLightForVertex(Light::Sun, x, y, z, i, j, normal, chunk);
		else
			vertices[j].lightValue[0] = chunk.lightmap().getSunlight(
					surroundingBlockPos[0], surroundingBlockPos[1], surroundingBlockPos[2]);

		int torchlight = chunk.lightmap().getTorchlight(x, y, z);

		if (Config::isTorchSmoothLightingEnabled && torchlight == 0)
			vertices[j].lightValue[1] = getLightForVertex(Light::Torch, x, y, z, i, j, normal, chunk);
		else
			vertices[j].lightValue[1] = chunk.lightmap().getTorchlight(
				surroundingBlockPos[0], surroundingBlockPos[1], surroundingBlockPos[2]);

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

	// Flipping quad to fix anisotropy issue
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

	static glm::vec3 normal{0, 0, 0};

	for (int i = 0 ; i < 2 ; ++i) {
		gk::Vertex vertices[4];
		for (int j = 0 ; j < 4 ; ++j) {
			vertices[j].coord3d[0] = x + crossCoords[i * 12 + j * 3];
			vertices[j].coord3d[1] = y + crossCoords[i * 12 + j * 3 + 1];
			vertices[j].coord3d[2] = z + crossCoords[i * 12 + j * 3 + 2];
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

			// FIXME: Duplicated above
			// if (Config::isSunSmoothLightingEnabled)
			// 	vertices[j].lightValue[0] = getLightForVertex(Light::Sun, x, y, z, i, j, normal, chunk);
			// else
				vertices[j].lightValue[0] = chunk.lightmap().getSunlight(x, y, z);

			// if (Config::isTorchSmoothLightingEnabled)
			// 	vertices[j].lightValue[1] = getLightForVertex(Light::Torch, x, y, z, i, j, normal, chunk);
			// else
				vertices[j].lightValue[1] = chunk.lightmap().getTorchlight(x, y, z);

			// if (Config::isAmbientOcclusionEnabled)
			// 	vertices[j].ambientOcclusion = getAmbientOcclusion(x, y, z, i, j, chunk);
			// else
				vertices[j].ambientOcclusion = 5;

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

gk::Vector3i ChunkBuilder::getOffsetFromVertex(u8 i, u8 j) const {
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
			(i == 5 && (j == 0 || j == 1))) ? -1 : 1;

	return offset;
}

// Based on this article: https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/
// FIXME: Not used anymore, but can be added as an option when smooth lighting is disabled though
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

float ChunkBuilder::getLightForVertex(Light light, u8 x, u8 y, u8 z, u8 i, u8 j, const glm::vec3 &normal, const ClientChunk &chunk) {
	gk::Vector3i offset = getOffsetFromVertex(i, j);

	gk::Vector3i minOffset{
		(normal.x != 0) ? offset.x : 0,
		(normal.y != 0) ? offset.y : 0,
		(normal.z != 0) ? offset.z : 0
	};

	// FIXME: Air blocks have a light level of 0
	if (light == Light::Sun)
		return (chunk.lightmap().getSunlight(x + minOffset.x, y + offset.y,    z + minOffset.z)
		      + chunk.lightmap().getSunlight(x + offset.x,    y + minOffset.y, z + minOffset.z)
		      + chunk.lightmap().getSunlight(x + minOffset.x, y + minOffset.y, z + offset.z)
		      + chunk.lightmap().getSunlight(x + offset.x,    y + offset.y,    z + offset.z)) / 4.0f;
	else
		return (chunk.lightmap().getTorchlight(x + minOffset.x, y + offset.y,    z + minOffset.z)
		      + chunk.lightmap().getTorchlight(x + offset.x,    y + minOffset.y, z + minOffset.z)
		      + chunk.lightmap().getTorchlight(x + minOffset.x, y + minOffset.y, z + offset.z)
		      + chunk.lightmap().getTorchlight(x + offset.x,    y + offset.y,    z + offset.z)) / 4.0f;
}

