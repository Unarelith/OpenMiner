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
#include "TextureAtlas.hpp"

static const float cubeCoords[6 * 4 * 3] = {
	// Top
	0, 1, 1,
	1, 1, 1,
	1, 1, 0,
	0, 1, 0,

	// Bottom
	0, 0, 0,
	1, 0, 0,
	1, 0, 1,
	0, 0, 1,

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
					{x, y + 1, z},
					{x, y - 1, z},
					{x - 1, y, z},
					{x + 1, y, z},
					{x, y, z - 1},
					{x, y, z + 1},
				};

				if (block.drawType() == BlockDrawType::Solid || block.drawType() == BlockDrawType::AllFaces) {
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

inline void ChunkBuilder::addFace(u8 x, u8 y, u8 z, u8 i, const ClientChunk &chunk, const Block *block, const int surroundingBlockPos[3]) {
	// Get surrounding block for that face
	u16 surroundingBlockID = chunk.getBlock(surroundingBlockPos[0], surroundingBlockPos[1], surroundingBlockPos[2]);
	const Block *surroundingBlock = &Registry::getInstance().getBlock(surroundingBlockID);

	// Skip hidden faces
	if(surroundingBlock && surroundingBlock->id() && surroundingBlock->drawType() == BlockDrawType::Solid
	&& (surroundingBlock->isOpaque() || block->drawType() != BlockDrawType::AllFaces))
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

	const BlockData *blockData = chunk.getBlockData(x, y, z);
	const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(block->tiles().getTextureForFace(i, blockData ? blockData->useAltTiles : false));
	float faceTexCoords[2 * 4] = {
		blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.height,
		blockTexCoords.x + blockTexCoords.width, blockTexCoords.y + blockTexCoords.height,
		blockTexCoords.x + blockTexCoords.width, blockTexCoords.y,
		blockTexCoords.x,                        blockTexCoords.y
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

		vertices[j].ambientOcclusion = getAmbientOcclusion(x, y, z, i, j, chunk);

		vertices[j].blockType = block->id();
	}

	auto addVertex = [&](u8 j) {
		if (!Config::isAmbientOcclusionEnabled)
			vertices[j].ambientOcclusion = 5;

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
		addVertex(2);
		addVertex(2);
		addVertex(3);
		addVertex(0);
	} else {
		addVertex(0);
		addVertex(1);
		addVertex(3);
		addVertex(3);
		addVertex(1);
		addVertex(2);
	}
}

inline void ChunkBuilder::addCross(u8 x, u8 y, u8 z, const ClientChunk &chunk, const Block *block) {
	const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(block->tiles().getTextureForFace(0));
	float faceTexCoords[2 * 4] = {
		blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.height,
		blockTexCoords.x + blockTexCoords.width, blockTexCoords.y + blockTexCoords.height,
		blockTexCoords.x + blockTexCoords.width, blockTexCoords.y,
		blockTexCoords.x,                        blockTexCoords.y
	};

	static glm::vec3 normal{0, 0, 0};

	for (int i = 0 ; i < 2 ; ++i) {
		gk::Vertex vertices[4];
		for (int j = 0 ; j < 4 ; ++j) {
			vertices[j].coord3d[0] = x + crossCoords[i * 12 + j * 3];
			vertices[j].coord3d[1] = y + crossCoords[i * 12 + j * 3 + 1];
			vertices[j].coord3d[2] = z + crossCoords[i * 12 + j * 3 + 2];
			vertices[j].coord3d[3] = 6;

			vertices[j].normal[0] = normal.x;
			vertices[j].normal[1] = normal.y;
			vertices[j].normal[2] = normal.z;

			vertices[j].color[0] = 1.0;
			vertices[j].color[1] = 1.0;
			vertices[j].color[2] = 1.0;
			vertices[j].color[3] = 1.0;

			vertices[j].texCoord[0] = faceTexCoords[j * 2];
			vertices[j].texCoord[1] = faceTexCoords[j * 2 + 1];

			vertices[j].lightValue[0] = chunk.lightmap().getSunlight(x, y, z);
			vertices[j].lightValue[1] = chunk.lightmap().getTorchlight(x, y, z);

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

inline gk::Vector3i ChunkBuilder::getOffsetFromVertex(u8 i, u8 j) const {
	gk::Vector3i offset;
	offset.x = (
			(i == BlockFace::Left) ||
			(i == BlockFace::Bottom && (j == 0 || j == 3)) ||
			(i == BlockFace::Top    && (j == 0 || j == 3)) ||
			(i == BlockFace::Front  && (j == 1 || j == 2)) ||
			(i == BlockFace::Back   && (j == 0 || j == 3))) ? -1 : 1;

	offset.z = (
			(i == BlockFace::Front) ||
			(i == BlockFace::Left   && (j == 0 || j == 3)) ||
			(i == BlockFace::Right  && (j == 1 || j == 2)) ||
			(i == BlockFace::Bottom && (j == 0 || j == 1)) ||
			(i == BlockFace::Top    && (j == 2 || j == 3))) ? -1 : 1;

	offset.y = (
			(i == BlockFace::Bottom) ||
			(i == BlockFace::Left  && (j == 0 || j == 1)) ||
			(i == BlockFace::Right && (j == 0 || j == 1)) ||
			(i == BlockFace::Front && (j == 0 || j == 1)) ||
			(i == BlockFace::Back  && (j == 0 || j == 1))) ? -1 : 1;

	return offset;
}

// Based on this article: https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/
inline u8 ChunkBuilder::getAmbientOcclusion(u8 x, u8 y, u8 z, u8 i, u8 j, const ClientChunk &chunk) {
	gk::Vector3i offset = getOffsetFromVertex(i, j);

	const Block &block0 = Registry::getInstance().getBlock(chunk.getBlock(x + offset.x, y + offset.y, z));
	const Block &block1 = Registry::getInstance().getBlock(chunk.getBlock(x,            y + offset.y, z + offset.z));
	const Block &block2 = Registry::getInstance().getBlock(chunk.getBlock(x + offset.x, y + offset.y, z + offset.z));

	bool side1  = block0.id() != 0 && block0.drawType() != BlockDrawType::XShape;
	bool side2  = block1.id() != 0 && block1.drawType() != BlockDrawType::XShape;
	bool corner = block2.id() != 0 && block2.drawType() != BlockDrawType::XShape;

	if (side1 && side2)
		return 0;

	return 3 - (side1 + side2 + corner);
}

inline u8 ChunkBuilder::getLightForVertex(Light light, u8 x, u8 y, u8 z, u8 i, u8 j, const glm::vec3 &normal, const ClientChunk &chunk) {
	std::function<s8(const Chunk *chunk, s8, s8, s8)> getLight = [&](const Chunk *chunk, s8 x, s8 y, s8 z) -> s8 {
		if(x < 0)             return chunk->getSurroundingChunk(0) && chunk->getSurroundingChunk(0)->isInitialized() ? getLight(chunk->getSurroundingChunk(0), x + CHUNK_WIDTH, y, z) : -1;
		if(x >= CHUNK_WIDTH)  return chunk->getSurroundingChunk(1) && chunk->getSurroundingChunk(1)->isInitialized() ? getLight(chunk->getSurroundingChunk(1), x - CHUNK_WIDTH, y, z) : -1;
		if(y < 0)             return chunk->getSurroundingChunk(4) && chunk->getSurroundingChunk(4)->isInitialized() ? getLight(chunk->getSurroundingChunk(4), x, y + CHUNK_HEIGHT, z) : -1;
		if(y >= CHUNK_HEIGHT) return chunk->getSurroundingChunk(5) && chunk->getSurroundingChunk(5)->isInitialized() ? getLight(chunk->getSurroundingChunk(5), x, y - CHUNK_HEIGHT, z) : -1;
		if(z < 0)             return chunk->getSurroundingChunk(2) && chunk->getSurroundingChunk(2)->isInitialized() ? getLight(chunk->getSurroundingChunk(2), x, y, z + CHUNK_DEPTH) : -1;
		if(z >= CHUNK_DEPTH)  return chunk->getSurroundingChunk(3) && chunk->getSurroundingChunk(3)->isInitialized() ? getLight(chunk->getSurroundingChunk(3), x, y, z - CHUNK_DEPTH) : -1;

		if (light == Light::Sun)
			return chunk->isInitialized() ? chunk->lightmap().getSunlight(x, y, z) : -1;
		else
			return chunk->isInitialized() ? chunk->lightmap().getTorchlight(x, y, z) : -1;
	};

	gk::Vector3i offset = getOffsetFromVertex(i, j);

	gk::Vector3i minOffset{
		(normal.x != 0) ? offset.x : 0,
		(normal.y != 0) ? offset.y : 0,
		(normal.z != 0) ? offset.z : 0
	};

	// Get light values for surrounding nodes
	s8 lightValues[4] = {
		getLight(&chunk, x + minOffset.x, y + offset.y,    z + minOffset.z),
		getLight(&chunk, x + offset.x,    y + minOffset.y, z + minOffset.z),
		getLight(&chunk, x + minOffset.x, y + minOffset.y, z + offset.z),
		getLight(&chunk, x + offset.x,    y + offset.y,    z + offset.z),
	};

	u8 count = 0, total = 0;
	for (u8 i = 0 ; i < 4 ; ++i) {
		// Fix light approximation
		// if (i == 3 && lightValues[i] > lightValues[0] && !lightValues[1] && !lightValues[2])
		// 	continue;

		// If the chunk is initialized, add the light value to the total
		if (lightValues[i] != -1) {
			total += lightValues[i];
			++count;
		}
	}

	if (count)
		return total / count;
	else
		return 0;
}

