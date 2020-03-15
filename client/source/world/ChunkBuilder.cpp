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
#include <gk/math/Math.hpp>

#include "BlockGeometry.hpp"
#include "ClientChunk.hpp"
#include "ChunkBuilder.hpp"
#include "Registry.hpp"
#include "TextureAtlas.hpp"

using namespace BlockGeometry;

std::array<std::size_t, ChunkBuilder::layers> ChunkBuilder::buildChunk(const ClientChunk &chunk,
                                                                       const std::array<gk::VertexBuffer, layers> &vbo)
{
	for (s8f i = 0 ; i < layers ; ++i)
		m_vertices[i].reserve(CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT * nFaces * nVertsPerFace);

	for (s8f z = 0 ; z < CHUNK_HEIGHT ; z++) {
		for (s8f y = 0 ; y < CHUNK_DEPTH ; y++) {
			for (s8f x = 0 ; x < CHUNK_WIDTH ; x++) {
				const Block &block = Registry::getInstance().getBlock(chunk.getBlock(x, y, z));
				if (!block.id()) continue;
				if (!chunk.getBlock(x, y, z)) continue;

				const gk::FloatBox &boundingBox = block.boundingBox();

				u8f orientation = block.isRotatable() ? chunk.getData(x, y, z) & 0x1F : 0;
				const glm::mat3 &orientMatrix = orientMatrices[orientation];

				if (block.drawType() == BlockDrawType::Solid
				 || block.drawType() == BlockDrawType::Leaves
				 || block.drawType() == BlockDrawType::Liquid
				 || block.drawType() == BlockDrawType::Glass
				 || block.drawType() == BlockDrawType::Cactus
				 || block.drawType() == BlockDrawType::BoundingBox)
				{
					glm::vec3 vertexPos[nVertsPerCube]{
						// Order is important. It matches the bit order defined in BlockGeometry::cubeVerts.
						{boundingBox.x,                     boundingBox.y,                     boundingBox.z},
						{boundingBox.x + boundingBox.sizeX, boundingBox.y,                     boundingBox.z},
						{boundingBox.x,                     boundingBox.y + boundingBox.sizeY, boundingBox.z},
						{boundingBox.x + boundingBox.sizeX, boundingBox.y + boundingBox.sizeY, boundingBox.z},
						{boundingBox.x,                     boundingBox.y,                     boundingBox.z + boundingBox.sizeZ},
						{boundingBox.x + boundingBox.sizeX, boundingBox.y,                     boundingBox.z + boundingBox.sizeZ},
						{boundingBox.x,                     boundingBox.y + boundingBox.sizeY, boundingBox.z + boundingBox.sizeZ},
						{boundingBox.x + boundingBox.sizeX, boundingBox.y + boundingBox.sizeY, boundingBox.z + boundingBox.sizeZ},
					};

					if (block.drawType() == BlockDrawType::Cactus) {
						// Ignore bounding box, initialize it to full node coordinates
						for (u8f i = 0; i < nVertsPerCube; ++i) {
							vertexPos[i].x = (i >> 0) & 1;
							vertexPos[i].y = (i >> 1) & 1;
							vertexPos[i].z = (i >> 2) & 1;
						}
					}

					// vNeighbour is used to find neighbouring cubes per vertex.
					// Same binary layout.
					glm::vec3 vNeighbour[nVertsPerCube] = {
						{-1,-1,-1}, { 1,-1,-1}, {-1, 1,-1}, { 1, 1,-1}, {-1,-1, 1}, { 1,-1, 1}, {-1, 1, 1}, {1, 1, 1},
					};

					if (orientation) { // don't work extra if it's not oriented differently
						static const glm::vec3 half{0.5, 0.5, 0.5};
						// Rotate each vertex coordinate around the centre of the
						// cube, and each vertex neighbour around the origin
						for (int i = 0; i < nVertsPerCube; ++i) {
							vertexPos[i] = orientMatrix * (vertexPos[i] - half) + half;
							vNeighbour[i] = orientMatrix * vNeighbour[i];
						}
					}

					for (s8f f = 0; f < nFaces ; ++f) {
						// Construct the normal vector to a face
						const glm::vec3 glmNormal = orientMatrix * faceNormals[f];
						const gk::Vector3i normal{int(glmNormal.x), int(glmNormal.y), int(glmNormal.z)};

						// Construct an array with the 4 vertex positions of this face
						glm::vec3 *faceVerts[nVertsPerFace]{&vertexPos[cubeVerts[f][0]], &vertexPos[cubeVerts[f][1]],
						                                    &vertexPos[cubeVerts[f][2]], &vertexPos[cubeVerts[f][3]]};

						// Construct an array with the 4 vertex neighbours of this face
						// (as GameKit integer vectors)
						const gk::Vector3i corner0{int(vNeighbour[cubeVerts[f][0]].x), int(vNeighbour[cubeVerts[f][0]].y), int(vNeighbour[cubeVerts[f][0]].z)};
						const gk::Vector3i corner1{int(vNeighbour[cubeVerts[f][1]].x), int(vNeighbour[cubeVerts[f][1]].y), int(vNeighbour[cubeVerts[f][1]].z)};
						const gk::Vector3i corner2{int(vNeighbour[cubeVerts[f][2]].x), int(vNeighbour[cubeVerts[f][2]].y), int(vNeighbour[cubeVerts[f][2]].z)};
						const gk::Vector3i corner3{int(vNeighbour[cubeVerts[f][3]].x), int(vNeighbour[cubeVerts[f][3]].y), int(vNeighbour[cubeVerts[f][3]].z)};

						const gk::Vector3i *vFaceNeighbours[nVertsPerFace]{&corner0, &corner1, &corner2, &corner3};

						addFace(x, y, z, f, chunk, block, normal, faceVerts, vFaceNeighbours);
					}
				}
				else if (block.drawType() == BlockDrawType::XShape) {
					glm::vec3 vertexPos[nVertsPerCube]{
						{0, 0, 0},
						{1, 0, 0},
						{0, 1, 0},
						{1, 1, 0},
						{0, 0, 1},
						{1, 0, 1},
						{0, 1, 1},
						{1, 1, 1},
					};
					const glm::vec3 *const faceVertices[nCrossFaces][nVertsPerFace]{
						{&vertexPos[crossVerts[0][0]], &vertexPos[crossVerts[0][1]],
						 &vertexPos[crossVerts[0][2]], &vertexPos[crossVerts[0][3]]},
						{&vertexPos[crossVerts[1][0]], &vertexPos[crossVerts[1][1]],
						 &vertexPos[crossVerts[1][2]], &vertexPos[crossVerts[1][3]]},
					};
					addCross(x, y, z, chunk, block, faceVertices);
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

inline void ChunkBuilder::addFace(s8f x, s8f y, s8f z, s8f f, const ClientChunk &chunk, const Block &block,
                                  const gk::Vector3i &normal, const glm::vec3 *const vertexPos[nVertsPerFace],
                                  const gk::Vector3i *const neighbourOfs[nVertsPerFace])
{
	// Get surrounding block for the face
	u16 surroundingBlockID = chunk.getBlock(x + normal.x, y + normal.y, z + normal.z);
	const Block *surroundingBlock = &Registry::getInstance().getBlock(surroundingBlockID);

	// Skip hidden faces
	if (surroundingBlock && surroundingBlock->id()
	&& ((block.drawType() == BlockDrawType::Solid && surroundingBlock->drawType() == BlockDrawType::Solid && surroundingBlock->isOpaque())
	 || (block.id() == surroundingBlock->id() && (block.drawType() == BlockDrawType::Liquid || block.drawType() == BlockDrawType::Glass))
	 || (block.drawType() == BlockDrawType::Liquid && surroundingBlock->drawType() == BlockDrawType::Solid)
	 || (block.drawType() == BlockDrawType::Cactus && surroundingBlock->id() == block.id() && f > 3)))
		return;

	const gk::FloatBox &boundingBox = block.boundingBox();

	const BlockData *blockData = chunk.getBlockData(x, y, z);
	const std::string &texture = block.tiles().getTextureForFace(f, blockData ? blockData->useAltTiles : false);
	const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(texture);

	// Calculate UV's
	// These are tough to obtain. Note that texture Y grows in the up-down direction, and so does V.
	// Vertex index in the bitmap array and U/V correspondence is:
	//    U0V0 -> 3 2 <- U1V0
	//    U0V1 -> 0 1 <- U1V1
	float U0, V0, U1, V1;
	if (block.drawType() == BlockDrawType::Cactus) {
		U0 = 0.f;
		V0 = 0.f;
		U1 = 1.f;
		V1 = 1.f;
	}
	else {
		U0 = (f == 0) ? 1.f - (boundingBox.y + boundingBox.sizeY) : (f == 1) ? boundingBox.y :
		     (f == 3) ? 1.f - (boundingBox.x + boundingBox.sizeX) : boundingBox.x;
		V0 = (f <= 3) ? 1.f - (boundingBox.z + boundingBox.sizeZ) : (f == 4) ? boundingBox.y : 1.f - (boundingBox.y + boundingBox.sizeY);
		U1 = (f == 0) ? 1.f - boundingBox.y : (f == 1) ? boundingBox.y + boundingBox.sizeY :
		     (f == 3) ? 1.f - boundingBox.x : boundingBox.x + boundingBox.sizeX;
		V1 = (f <= 3) ? 1.f - boundingBox.z : (f == 4) ? boundingBox.y + boundingBox.sizeY : 1.f - boundingBox.y;
	}

	// Prepare vertex information for VBO
	gk::Vertex vertices[nVertsPerFace];
	for (s8f v = 0; v < nVertsPerFace; ++v) {
		if (block.drawType() == BlockDrawType::Cactus) {
			vertices[v].coord3d[0] = x + vertexPos[v]->x - boundingBox.x * normal.x;
			vertices[v].coord3d[1] = y + vertexPos[v]->y - boundingBox.y * normal.y;
			vertices[v].coord3d[2] = z + vertexPos[v]->z - boundingBox.z * normal.z;
		}
		else {
			float blockHeight = vertexPos[v]->z;
			if (block.drawType() == BlockDrawType::Liquid && (!surroundingBlock || !surroundingBlock->id())) {
				if (f == BlockFace::Bottom)
					blockHeight = vertexPos[v]->z - 2.f / 16.f;
				else
					blockHeight = vertexPos[v]->z * 14.f / 16.f;
			}

			vertices[v].coord3d[0] = x + vertexPos[v]->x;
			vertices[v].coord3d[1] = y + vertexPos[v]->y;
			vertices[v].coord3d[2] = z + blockHeight;
		}

		vertices[v].coord3d[3] = f;

		vertices[v].normal[0] = normal.x;
		vertices[v].normal[1] = normal.y;
		vertices[v].normal[2] = normal.z;

		const gk::Color colorMultiplier = block.colorMultiplier();
		vertices[v].color[0] = colorMultiplier.r;
		vertices[v].color[1] = colorMultiplier.g;
		vertices[v].color[2] = colorMultiplier.b;
		vertices[v].color[3] = colorMultiplier.a;

		float U = (v == 0 || v == 3) ? U0 : U1;
		float V = (v >= 2) ? V0 : V1;
		vertices[v].texCoord[0] = gk::qlerp(blockTexCoords.x, blockTexCoords.x + blockTexCoords.sizeX, U);
		vertices[v].texCoord[1] = gk::qlerp(blockTexCoords.y, blockTexCoords.y + blockTexCoords.sizeY, V);

		if (Config::isSunSmoothLightingEnabled && block.drawType() != BlockDrawType::Liquid)
			vertices[v].lightValue[0] = getLightForVertex(Light::Sun, x, y, z, *neighbourOfs[v], normal, chunk);
		else
			vertices[v].lightValue[0] = chunk.lightmap().getSunlight(x + normal.x, y + normal.y, z + normal.z);

		int torchlight = chunk.lightmap().getTorchlight(x, y, z);
		if (Config::isTorchSmoothLightingEnabled && torchlight == 0 && block.drawType() != BlockDrawType::Liquid)
			vertices[v].lightValue[1] = getLightForVertex(Light::Torch, x, y, z, *neighbourOfs[v], normal, chunk);
		else
			vertices[v].lightValue[1] = chunk.lightmap().getTorchlight(x + normal.x, y + normal.y, z + normal.z);

		vertices[v].ambientOcclusion = getAmbientOcclusion(x, y, z, *neighbourOfs[v], chunk);
	}

	auto addVertex = [&](u8 v) {
		if (!Config::isAmbientOcclusionEnabled)
			vertices[v].ambientOcclusion = 5;

		if (block.drawType() == BlockDrawType::Liquid)
			m_vertices[Layer::Liquid].emplace_back(vertices[v]);
		else if (block.drawType() == BlockDrawType::Glass)
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

inline void ChunkBuilder::addCross(s8f x, s8f y, s8f z, const ClientChunk &chunk, const Block &block, const glm::vec3 *const vertexPos[nCrossFaces][nVertsPerFace]) {
	const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(block.tiles().getTextureForFace(0));
	float faceTexCoords[nVertsPerFace][nCoordsPerUV] = {
		{blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y},
		{blockTexCoords.x,                        blockTexCoords.y},
	};

	for (int f = 0; f < nCrossFaces ; ++f) {
		gk::Vertex vertices[nVertsPerFace];
		for (int v = 0 ; v < nVertsPerFace ; ++v) {
			vertices[v].coord3d[0] = x + vertexPos[f][v]->x;
			vertices[v].coord3d[1] = y + vertexPos[f][v]->y;
			vertices[v].coord3d[2] = z + vertexPos[f][v]->z;
			vertices[v].coord3d[3] = 6;

			vertices[v].normal[0] = 0;
			vertices[v].normal[1] = 0;
			vertices[v].normal[2] = 0;

			const gk::Color colorMultiplier = block.colorMultiplier();
			vertices[v].color[0] = colorMultiplier.r;
			vertices[v].color[1] = colorMultiplier.g;
			vertices[v].color[2] = colorMultiplier.b;
			vertices[v].color[3] = colorMultiplier.a;

			vertices[v].texCoord[0] = faceTexCoords[v][0];
			vertices[v].texCoord[1] = faceTexCoords[v][1];

			vertices[v].lightValue[0] = chunk.lightmap().getSunlight(x, y, z);
			vertices[v].lightValue[1] = chunk.lightmap().getTorchlight(x, y, z);

			vertices[v].ambientOcclusion = 5;
		}

		m_vertices[Layer::Flora].emplace_back(vertices[0]);
		m_vertices[Layer::Flora].emplace_back(vertices[1]);
		m_vertices[Layer::Flora].emplace_back(vertices[3]);
		m_vertices[Layer::Flora].emplace_back(vertices[3]);
		m_vertices[Layer::Flora].emplace_back(vertices[1]);
		m_vertices[Layer::Flora].emplace_back(vertices[2]);
	}
}

// Based on this article: https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/
inline u8 ChunkBuilder::getAmbientOcclusion(s8f x, s8f y, s8f z, const gk::Vector3i &offset, const ClientChunk &chunk) {
	const Block &block0 = Registry::getInstance().getBlock(chunk.getBlock(x + offset.x, y,            z + offset.z));
	const Block &block1 = Registry::getInstance().getBlock(chunk.getBlock(x,            y + offset.y, z + offset.z));
	const Block &block2 = Registry::getInstance().getBlock(chunk.getBlock(x + offset.x, y + offset.y, z + offset.z));

	bool side1  = block0.id() != 0 && block0.drawType() != BlockDrawType::XShape;
	bool side2  = block1.id() != 0 && block1.drawType() != BlockDrawType::XShape;
	bool corner = block2.id() != 0 && block2.drawType() != BlockDrawType::XShape;

	return (side1 && side2) ? 0 : 3 - (side1 + side2 + corner);
}

inline u8 ChunkBuilder::getLightForVertex(Light light, s8f x, s8f y, s8f z, const gk::Vector3i &offset, const gk::Vector3i &normal, const ClientChunk &chunk) {
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
