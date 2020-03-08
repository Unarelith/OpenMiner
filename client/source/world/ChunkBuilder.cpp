/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "ClientChunk.hpp"
#include "ChunkBuilder.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"

constexpr int nAxes = 6;
constexpr int nAxesP2 = 8; // Next power of 2 to nAxes
constexpr int nRots = 4;
constexpr int nFaces = 6;
constexpr int nCrossFaces = 2;
constexpr int nVertsPerFace = 4;
constexpr int nNormals = 1;
constexpr int nCoords = 3;
constexpr int nCoordsPerUV = 2;

// Same order as enum BlockFace in TilesDef.hpp
static constexpr ChunkBuilder::tCubeCoord cubeCoords[nFaces][nVertsPerFace + nNormals][nCoords] = {
	// West
	{
		{0, 1, 0},
		{0, 0, 0},
		{0, 0, 1},
		{0, 1, 1},

		{-1, 0, 0}, // normal
	},

	// East
	{
		{1, 0, 0},
		{1, 1, 0},
		{1, 1, 1},
		{1, 0, 1},

		{1, 0, 0},
	},

	// South
	{
		{0, 0, 0},
		{1, 0, 0},
		{1, 0, 1},
		{0, 0, 1},

		{0,-1, 0},
	},

	// North
	{
		{1, 1, 0},
		{0, 1, 0},
		{0, 1, 1},
		{1, 1, 1},

		{0, 1, 0},
	},

	// Bottom
	{
		{1, 0, 0},
		{0, 0, 0},
		{0, 1, 0},
		{1, 1, 0},

		{0, 0,-1},
	},

	// Top
	{
		{1, 1, 1},
		{0, 1, 1},
		{0, 0, 1},
		{1, 0, 1},

		{0, 0, 1},
	},
};

static constexpr ChunkBuilder::tCubeCoord crossCoords[nCrossFaces][nVertsPerFace][nCoords] = {
	{
		{1, 1, 0},
		{0, 0, 0},
		{0, 0, 1},
		{1, 1, 1},
	},

	{
		{1, 0, 0},
		{0, 1, 0},
		{0, 1, 1},
		{1, 0, 1},
	},
};

static ChunkBuilder::tCubeCoord orientCubeCoords[nAxesP2 * nRots][nFaces][nVertsPerFace + nNormals][nCoords];

ChunkBuilder::ChunkBuilder(TextureAtlas &textureAtlas) : m_textureAtlas(textureAtlas) {
	static bool isOrientInitialized = false;

	if (!isOrientInitialized) {
		initializeOrientation();
		isOrientInitialized = true;
	}
}

std::array<std::size_t, ChunkBuilder::layers> ChunkBuilder::buildChunk(const ClientChunk &chunk, const std::array<gk::VertexBuffer, layers> &vbo) {
	for (s8f i = 0 ; i < layers ; ++i)
		m_vertices[i].reserve(CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT * 6 * 4);

	for (s8f z = 0 ; z < CHUNK_HEIGHT ; z++) {
		for (s8f y = 0 ; y < CHUNK_DEPTH ; y++) {
			for (s8f x = 0 ; x < CHUNK_WIDTH ; x++) {
				const Block &block = Registry::getInstance().getBlock(chunk.getBlock(x, y, z));
				if (!block.id()) continue;
				if (!chunk.getBlock(x, y, z)) continue;

				if (block.drawType() == BlockDrawType::Solid
				 || block.drawType() == BlockDrawType::Leaves
				 || block.drawType() == BlockDrawType::Liquid
				 || block.drawType() == BlockDrawType::Glass
				 || block.drawType() == BlockDrawType::BoundingBox) {
					for (s8f i = 0 ; i < nFaces ; i++) {
						addFace(x, y, z, i, chunk, &block);
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

inline void ChunkBuilder::addFace(s8f x, s8f y, s8f z, s8f f, const ClientChunk &chunk, const Block *block) {
	u8 orientation = block->isRotatable() ? chunk.getData(x, y, z) & 0x1F : 0;
	gk::Vector3i normal{orientCubeCoords[orientation][f][4][0],
	                    orientCubeCoords[orientation][f][4][1],
	                    orientCubeCoords[orientation][f][4][2]};

	// Get surrounding block for that face
	u16 surroundingBlockID = chunk.getBlock(x + normal.x, y + normal.y, z + normal.z);
	const Block *surroundingBlock = &Registry::getInstance().getBlock(surroundingBlockID);

	// Skip hidden faces
	if (surroundingBlock && surroundingBlock->id()
	&& ((block->drawType() == BlockDrawType::Solid && surroundingBlock->drawType() == BlockDrawType::Solid && surroundingBlock->isOpaque())
	 || (block->id() == surroundingBlock->id() && (block->drawType() == BlockDrawType::Liquid || block->drawType() == BlockDrawType::Glass))
	 || (block->drawType() == BlockDrawType::Liquid && surroundingBlock->drawType() == BlockDrawType::Solid)))
		return;

	const BlockData *blockData = chunk.getBlockData(x, y, z);
	const std::string &texture = block->tiles().getTextureForFace(f, blockData ? blockData->useAltTiles : false);
	const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(texture);
	float faceTexCoords[nVertsPerFace][nCoordsPerUV] = {
		{blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y},
		{blockTexCoords.x,                        blockTexCoords.y},
	};

	const gk::FloatBox boundingBox = block->boundingBox();

	// Store vertex information
	gk::Vertex vertices[nVertsPerFace];
	for (s8f v = 0 ; v < nVertsPerFace; v++) {
		tCubeCoord *vertexPosPtr = orientCubeCoords[orientation][f][v];
		if (block->drawType() == BlockDrawType::BoundingBox) {
			vertices[v].coord3d[0] = x + vertexPosPtr[0] * boundingBox.sizeX + boundingBox.x;
			vertices[v].coord3d[1] = y + vertexPosPtr[1] * boundingBox.sizeY + boundingBox.y;
			vertices[v].coord3d[2] = z + vertexPosPtr[2] * boundingBox.sizeZ + boundingBox.z;
		}
		else {
			vertices[v].coord3d[0] = x + vertexPosPtr[0];
			vertices[v].coord3d[1] = y + vertexPosPtr[1];
			vertices[v].coord3d[2] = z + vertexPosPtr[2];
		}

		vertices[v].coord3d[3] = f;

		vertices[v].normal[0] = normal.x;
		vertices[v].normal[1] = normal.y;
		vertices[v].normal[2] = normal.z;

		const gk::Color colorMultiplier = block->colorMultiplier();
		vertices[v].color[0] = colorMultiplier.r;
		vertices[v].color[1] = colorMultiplier.g;
		vertices[v].color[2] = colorMultiplier.b;
		vertices[v].color[3] = colorMultiplier.a;

		vertices[v].texCoord[0] = faceTexCoords[v][0];
		vertices[v].texCoord[1] = faceTexCoords[v][1];


		if (Config::isSunSmoothLightingEnabled && block->drawType() != BlockDrawType::Liquid)
			vertices[v].lightValue[0] = getLightForVertex(Light::Sun, x, y, z, vertexPosPtr, normal, chunk);
		else
			vertices[v].lightValue[0] = chunk.lightmap().getSunlight(x + normal.x, y + normal.y, z + normal.z);

		int torchlight = chunk.lightmap().getTorchlight(x, y, z);
		if (Config::isTorchSmoothLightingEnabled && torchlight == 0 && block->drawType() != BlockDrawType::Liquid)
			vertices[v].lightValue[1] = getLightForVertex(Light::Torch, x, y, z, vertexPosPtr, normal, chunk);
		else
			vertices[v].lightValue[1] = chunk.lightmap().getTorchlight(x + normal.x, y + normal.y, z + normal.z);

		vertices[v].ambientOcclusion = getAmbientOcclusion(x, y, z, vertexPosPtr, chunk);
	}

	auto addVertex = [&](u8 v) {
		if (!Config::isAmbientOcclusionEnabled)
			vertices[v].ambientOcclusion = 5;

		if (block->drawType() == BlockDrawType::Liquid)
			m_vertices[Layer::Liquid].emplace_back(vertices[v]);
		else if (block->drawType() == BlockDrawType::Glass)
			m_vertices[Layer::Glass].emplace_back(vertices[v]);
		else
			m_vertices[Layer::Solid].emplace_back(vertices[v]);
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

inline void ChunkBuilder::addCross(s8f x, s8f y, s8f z, const ClientChunk &chunk, const Block *block) {
	const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(block->tiles().getTextureForFace(0));
	float faceTexCoords[nFaces][nCoordsPerUV] = {
		{blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y},
		{blockTexCoords.x,                        blockTexCoords.y},
	};

	static gk::Vector3i normal{0, 0, 0};

	for (int i = 0 ; i < nCrossFaces ; ++i) {
		gk::Vertex vertices[nVertsPerFace];
		for (int j = 0 ; j < nVertsPerFace ; ++j) {
			vertices[j].coord3d[0] = x + crossCoords[i][j][0];
			vertices[j].coord3d[1] = y + crossCoords[i][j][1];
			vertices[j].coord3d[2] = z + crossCoords[i][j][2];
			vertices[j].coord3d[3] = 6;

			vertices[j].normal[0] = normal.x;
			vertices[j].normal[1] = normal.y;
			vertices[j].normal[2] = normal.z;

			const gk::Color colorMultiplier = block->colorMultiplier();
			vertices[j].color[0] = colorMultiplier.r;
			vertices[j].color[1] = colorMultiplier.g;
			vertices[j].color[2] = colorMultiplier.b;
			vertices[j].color[3] = colorMultiplier.a;

			vertices[j].texCoord[0] = faceTexCoords[j][0];
			vertices[j].texCoord[1] = faceTexCoords[j][1];

			vertices[j].lightValue[0] = chunk.lightmap().getSunlight(x, y, z);
			vertices[j].lightValue[1] = chunk.lightmap().getTorchlight(x, y, z);

			vertices[j].ambientOcclusion = 5;
		}

		m_vertices[Layer::Flora].emplace_back(vertices[0]);
		m_vertices[Layer::Flora].emplace_back(vertices[1]);
		m_vertices[Layer::Flora].emplace_back(vertices[3]);
		m_vertices[Layer::Flora].emplace_back(vertices[3]);
		m_vertices[Layer::Flora].emplace_back(vertices[1]);
		m_vertices[Layer::Flora].emplace_back(vertices[2]);
	}
}

inline gk::Vector3i ChunkBuilder::getOffsetFromVertex(const tCubeCoord *const vertexPosPtr) const {
	return gk::Vector3i{vertexPosPtr[0] * 2 - 1, vertexPosPtr[1] * 2 - 1, vertexPosPtr[2] * 2 - 1};
}

// Based on this article: https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/
inline u8 ChunkBuilder::getAmbientOcclusion(s8f x, s8f y, s8f z, const tCubeCoord *const vertexPosPtr, const ClientChunk &chunk) {
	gk::Vector3i offset = getOffsetFromVertex(vertexPosPtr);

	const Block &block0 = Registry::getInstance().getBlock(chunk.getBlock(x + offset.x, y,            z + offset.z));
	const Block &block1 = Registry::getInstance().getBlock(chunk.getBlock(x,            y + offset.y, z + offset.z));
	const Block &block2 = Registry::getInstance().getBlock(chunk.getBlock(x + offset.x, y + offset.y, z + offset.z));

	bool side1  = block0.id() != 0 && block0.drawType() != BlockDrawType::XShape;
	bool side2  = block1.id() != 0 && block1.drawType() != BlockDrawType::XShape;
	bool corner = block2.id() != 0 && block2.drawType() != BlockDrawType::XShape;

	return (side1 && side2) ? 0 : 3 - (side1 + side2 + corner);
}

inline u8 ChunkBuilder::getLightForVertex(Light light, s8f x, s8f y, s8f z, const tCubeCoord *const vertexPosPtr, const gk::Vector3i &normal, const ClientChunk &chunk) {
	std::function<s8(const Chunk *chunk, s8, s8, s8)> getLight = [&](const Chunk *chunk, s8 x, s8 y, s8 z) -> s8 {
		if (x < 0)             return chunk->getSurroundingChunk(0) && chunk->getSurroundingChunk(0)->isInitialized() ? getLight(chunk->getSurroundingChunk(0), x + CHUNK_WIDTH, y, z) : -1;
		if (x >= CHUNK_WIDTH)  return chunk->getSurroundingChunk(1) && chunk->getSurroundingChunk(1)->isInitialized() ? getLight(chunk->getSurroundingChunk(1), x - CHUNK_WIDTH, y, z) : -1;
		if (y < 0)             return chunk->getSurroundingChunk(2) && chunk->getSurroundingChunk(2)->isInitialized() ? getLight(chunk->getSurroundingChunk(2), x, y + CHUNK_DEPTH, z) : -1;
		if (y >= CHUNK_DEPTH)  return chunk->getSurroundingChunk(3) && chunk->getSurroundingChunk(3)->isInitialized() ? getLight(chunk->getSurroundingChunk(3), x, y - CHUNK_DEPTH, z) : -1;
		if (z < 0)             return chunk->getSurroundingChunk(4) && chunk->getSurroundingChunk(4)->isInitialized() ? getLight(chunk->getSurroundingChunk(4), x, y, z + CHUNK_HEIGHT) : -1;
		if (z >= CHUNK_HEIGHT) return chunk->getSurroundingChunk(5) && chunk->getSurroundingChunk(5)->isInitialized() ? getLight(chunk->getSurroundingChunk(5), x, y, z - CHUNK_HEIGHT) : -1;

		if (light == Light::Sun)
			return chunk->isInitialized() ? chunk->lightmap().getSunlight(x, y, z) : -1;
		else
			return chunk->isInitialized() ? chunk->lightmap().getTorchlight(x, y, z) : -1;
	};

	gk::Vector3i offset = getOffsetFromVertex(vertexPosPtr);

	gk::Vector3i minOffset{
		(normal.x != 0) ? offset.x : 0,
		(normal.y != 0) ? offset.y : 0,
		(normal.z != 0) ? offset.z : 0
	};

	// Get light values for surrounding nodes
	s8 lightValues[4] = {
		getLight(&chunk, x + minOffset.x, y + minOffset.y, z + offset.z),
		getLight(&chunk, x + offset.x,    y + minOffset.y, z + minOffset.z),
		getLight(&chunk, x + minOffset.x, y + offset.y,    z + minOffset.z),
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

void ChunkBuilder::initializeOrientation() {
	// Build all 24 rotated versions of cubeCoords

	// Local rotation of top face
	glm::mat3 topRotation = {
	    1, 0, 0,
	    0, 1, 0,
	    0, 0, 1,
	};
	// Matrix for a rotation of 90 degrees CCW around the Z axis, used to
	// rotate the top face in 90 degree steps around its local Z axis.
	const glm::mat3 rotate90Z = {
		// Note glm matrices are column-major, so each row here is
		// actually a column of the matrix.
		 0, 1, 0,
		-1, 0, 0,
		 0, 0, 1,
	};
	// Rotations that place the top face on each of the axes
	const glm::mat3 rot2axis[nAxes] = {
		{ 1,  0,  0,   0,  1,  0,   0,  0,  1},  // top face on +Z, no rotation (identity matrix)
		{ 1,  0,  0,   0, -1,  0,   0,  0, -1},  // top face on -Z, rotating around the X axis
		{ 1,  0,  0,   0,  0, -1,   0,  1,  0},  // top face on +Y, rotating around the X axis
		{ 1,  0,  0,   0,  0,  1,   0, -1,  0},  // top face on -Y, rotating around the X axis
		{ 0,  0,  1,   0,  1,  0,  -1,  0,  0},  // top face on +X, rotating around the Y axis
		{ 0,  0, -1,   0,  1,  0,   1,  0,  0},  // top face on -X, rotating around the Y axis
	};

	for (s8f axis = 0; axis < nAxes; ++axis) {
		for (s8f angle = 0; angle < nRots; ++angle) {
			glm::mat3 finalMat = rot2axis[axis] * topRotation;

			for (s8f face = 0; face < nFaces; ++face) {
				for (s8f vNum = 0; vNum < nVertsPerFace + nNormals; ++vNum) {
					glm::vec3 vertex{cubeCoords[face][vNum][0], cubeCoords[face][vNum][1], cubeCoords[face][vNum][2]};
					if (vNum < nVertsPerFace)
						vertex -= glm::vec3{0.5, 0.5, 0.5};  // Translate by centre of the cube
					vertex = finalMat * vertex;
					tCubeCoord *pVertex = orientCubeCoords[axis * nRots + angle][face][vNum];
					if (vNum < nVertsPerFace)
						vertex += glm::vec3{0.5, 0.5, 0.5};  // Translate back
					pVertex[0] = tCubeCoord(vertex[0]);
					pVertex[1] = tCubeCoord(vertex[1]);
					pVertex[2] = tCubeCoord(vertex[2]);
				}
			}

			// Next top rotation
			topRotation = rotate90Z * topRotation;
		}
	}

	// Since we use bit masking, we add eight null orientations to the normal
	// set of 24 to complete a power of two, to make the code robust against
	// invalid orientation values.
	for (s8f axis = nAxes; axis < nAxesP2; ++axis) {
		for (s8f angle = 0; angle < nRots; ++angle) {
			memcpy(&orientCubeCoords[axis * nRots + angle], &orientCubeCoords[0],
			       nFaces * (nVertsPerFace + nNormals) * nCoords * sizeof(tCubeCoord));
		}
	}
}
