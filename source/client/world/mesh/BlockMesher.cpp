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
#include "BlockGeometry.hpp"
#include "BlockMesher.hpp"
#include "ClientProfiler.hpp"
#include "Config.hpp"
#include "LightUtils.hpp"

using namespace BlockGeometry;
using namespace LightUtils;

static std::unordered_map<u32, Vertex[nFaces][nVertsPerFace]> s_blockCache;

void BlockMesher::addBlock(s8f x, s8f y, s8f z, ChunkMeshBuildingJob &job,
                           const BlockState &blockState, u16 blockParam)
{
	if (s_blockCache.find(blockState.block().id()) == s_blockCache.end())
		addBlockToCache(job.textureAtlas, blockState);

	// vNeighbour is used to find neighbouring cubes per vertex.
	// Same binary layout.
	//glm::vec3 vNeighbour[nVertsPerCube] = {
	//	{-1,-1,-1}, { 1,-1,-1}, {-1, 1,-1}, { 1, 1,-1}, {-1,-1, 1}, { 1,-1, 1}, {-1, 1, 1}, {1, 1, 1},
	//};

	Vertex vertices[nFaces][nVertsPerFace];
	memcpy(vertices, s_blockCache[blockState.block().id()], sizeof(Vertex) * nFaces * nVertsPerFace);

	for (s8f f = 0; f < nFaces ; ++f) {
		// Construct an array with the 4 vertex neighbours of this face
		// (as GameKit integer vectors)
		//const gk::Vector3<s8f> neighbourOfs[nVertsPerFace]{
		//	{s8f(vNeighbour[cubeVerts[f][0]].x), s8f(vNeighbour[cubeVerts[f][0]].y), s8f(vNeighbour[cubeVerts[f][0]].z)},
		//	{s8f(vNeighbour[cubeVerts[f][1]].x), s8f(vNeighbour[cubeVerts[f][1]].y), s8f(vNeighbour[cubeVerts[f][1]].z)},
		//	{s8f(vNeighbour[cubeVerts[f][2]].x), s8f(vNeighbour[cubeVerts[f][2]].y), s8f(vNeighbour[cubeVerts[f][2]].z)},
		//	{s8f(vNeighbour[cubeVerts[f][3]].x), s8f(vNeighbour[cubeVerts[f][3]].y), s8f(vNeighbour[cubeVerts[f][3]].z)},
		//};

		// Get surrounding block for the face
		s8f sx = x + vertices[f][0].normal[0];
		s8f sy = y + vertices[f][0].normal[1];
		s8f sz = z + vertices[f][0].normal[2];

		const BlockState *surroundingBlockState = job.chunkData.getBlockState(sx, sy, sz);

		// Skip hidden faces
		if (surroundingBlockState && surroundingBlockState->block().id()
		&& ((blockState.drawType() == BlockDrawType::Solid && surroundingBlockState->drawType() == BlockDrawType::Solid && surroundingBlockState->isOpaque())
		 || (blockState.block().id() == surroundingBlockState->block().id() && (blockState.drawType() == BlockDrawType::Liquid || blockState.drawType() == BlockDrawType::Glass))
		 || (blockState.drawType() == BlockDrawType::Liquid && surroundingBlockState->drawType() == BlockDrawType::Solid)
		 || (blockState.drawType() == BlockDrawType::Cactus && surroundingBlockState->block().id() == blockState.block().id() && f > 3)))
			continue;

		const std::string &texture = blockState.tiles().getTextureForFace(f);
		const gk::FloatRect &blockTexCoords = job.textureAtlas->getTexCoords(texture);

		// Prepare vertex information for VBO
		for (s8f v = 0; v < nVertsPerFace; ++v) {
			vertices[f][v].coord3d[0] += x;
			vertices[f][v].coord3d[1] += y;
			vertices[f][v].coord3d[2] += z;

			if (Config::isSmoothLightingEnabled)
				vertices[f][v].lightValue[0] = job.chunkData.getSunlight(sx, sy, sz);
				//vertices[f][v].lightValue[0] = getLightForVertex(LightType::Sun, x, y, z, neighbourOfs[v], normal, job.chunkData);
			else if (blockState.isOpaque())
				vertices[f][v].lightValue[0] = job.chunkData.getSunlight(sx, sy, sz);
			else
				vertices[f][v].lightValue[0] = job.chunkData.getSunlight(x, y, z);

			if (Config::isSmoothLightingEnabled && !blockState.isLightSource())
				//vertices[f][v].lightValue[1] = getLightForVertex(LightType::Torch, x, y, z, neighbourOfs[v], normal, job.chunkData);
				vertices[f][v].lightValue[0] = job.chunkData.getSunlight(sx, sy, sz);
			else if (blockState.isOpaque())
				vertices[f][v].lightValue[1] = job.chunkData.getTorchlight(sx, sy, sz);
			else
				vertices[f][v].lightValue[1] = job.chunkData.getTorchlight(x, y, z);

			//vertices[f][v].ambientOcclusion = getAmbientOcclusion(x, y, z, neighbourOfs[v], normal, job.chunkData);
			vertices[f][v].ambientOcclusion = 4;
		}

		auto addVertex = [&](s8f v) {
			if (Config::ambientOcclusion != 1 || blockState.isLightSource())
				vertices[f][v].ambientOcclusion = 4;

			if (blockState.drawType() == BlockDrawType::Liquid)
				job.vertices[ChunkMeshLayer::Liquid].emplace_back(vertices[f][v]);
			else if (blockState.drawType() == BlockDrawType::Glass)
				job.vertices[ChunkMeshLayer::Glass].emplace_back(vertices[f][v]);
			else if (blockState.colorMultiplier() != gk::Color::White)
				job.vertices[ChunkMeshLayer::NoMipMap].emplace_back(vertices[f][v]);
			else
				job.vertices[ChunkMeshLayer::Solid].emplace_back(vertices[f][v]);
		};

		// Flipping quad to fix anisotropy issue
		if (vertices[f][0].ambientOcclusion + vertices[f][2].ambientOcclusion >
			vertices[f][1].ambientOcclusion + vertices[f][3].ambientOcclusion)
		{
			addVertex(0);
			addVertex(1);
			addVertex(2);
			addVertex(2);
			addVertex(3);
			addVertex(0);
		}
		else
		{
			addVertex(0);
			addVertex(1);
			addVertex(3);
			addVertex(3);
			addVertex(1);
			addVertex(2);
		}

		job.totalVertexCount += 6;
	}
}

void BlockMesher::addBlockToCache(const TextureAtlas *textureAtlas, const BlockState &blockState)
{
	const gk::FloatBox &boundingBox = blockState.boundingBox();

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

	Vertex (*vertices)[nVertsPerFace] = s_blockCache[blockState.block().id()];
	for (s8f f = 0; f < nFaces ; ++f) {
		// Construct an array with the 4 vertex positions of this face
		glm::vec3 *faceVerts[nVertsPerFace]{
			&vertexPos[cubeVerts[f][0]], &vertexPos[cubeVerts[f][1]],
			&vertexPos[cubeVerts[f][2]], &vertexPos[cubeVerts[f][3]]
		};

		// Construct an array with the 4 vertex neighbours of this face
		// (as GameKit integer vectors)
		const std::string &texture = blockState.tiles().getTextureForFace(f);
		const gk::FloatRect &blockTexCoords = textureAtlas->getTexCoords(texture);

		// Calculate UV's
		// These are tough to obtain. Note that texture Y grows in the up-down direction, and so does V.
		// Vertex index in the bitmap array and U/V correspondence is:
		//    U0V0 -> 3 2 <- U1V0
		//    U0V1 -> 0 1 <- U1V1
		float U0 = (f == 0) ? 1.f - (boundingBox.y + boundingBox.sizeY) : (f == 1) ? boundingBox.y :
		           (f == 3) ? 1.f - (boundingBox.x + boundingBox.sizeX) : boundingBox.x;
		float V0 = (f <= 3) ? 1.f - (boundingBox.z + boundingBox.sizeZ) : (f == 4) ? boundingBox.y : 1.f - (boundingBox.y + boundingBox.sizeY);
		float U1 = (f == 0) ? 1.f - boundingBox.y : (f == 1) ? boundingBox.y + boundingBox.sizeY :
		           (f == 3) ? 1.f - boundingBox.x : boundingBox.x + boundingBox.sizeX;
		float V1 = (f <= 3) ? 1.f - boundingBox.z : (f == 4) ? boundingBox.y + boundingBox.sizeY : 1.f - boundingBox.y;

		// Prepare vertex information
		for (s8f v = 0; v < nVertsPerFace; ++v) {
			vertices[f][v].coord3d[0] = faceVerts[v]->x + blockState.drawOffset().x;
			vertices[f][v].coord3d[1] = faceVerts[v]->y + blockState.drawOffset().y;
			vertices[f][v].coord3d[2] = faceVerts[v]->z + blockState.drawOffset().z;
			vertices[f][v].coord3d[3] = f;

			vertices[f][v].normal[0] = faceNormals[f][0];
			vertices[f][v].normal[1] = faceNormals[f][1];
			vertices[f][v].normal[2] = faceNormals[f][2];

			const gk::Color colorMultiplier = blockState.colorMultiplier();
			vertices[f][v].color[0] = colorMultiplier.r;
			vertices[f][v].color[1] = colorMultiplier.g;
			vertices[f][v].color[2] = colorMultiplier.b;
			vertices[f][v].color[3] = colorMultiplier.a;

			float U = (v == 0 || v == 3) ? U0 : U1;
			float V = (v >= 2) ? V0 : V1;
			vertices[f][v].texCoord[0] = gk::qlerpf(blockTexCoords.x, blockTexCoords.x + blockTexCoords.sizeX, U);
			vertices[f][v].texCoord[1] = gk::qlerpf(blockTexCoords.y, blockTexCoords.y + blockTexCoords.sizeY, V);
		}
	}
}
