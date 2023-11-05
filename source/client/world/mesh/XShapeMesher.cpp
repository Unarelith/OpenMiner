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
#include "ChunkMeshBuildingJob.hpp"
#include "XShapeMesher.hpp"

using namespace BlockGeometry;

void XShapeMesher::addCross(s8f x, s8f y, s8f z, ChunkMeshBuildingJob &job,
                            const BlockState &blockState)
{
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

	const std::string &texture = blockState.tiles().getTextureForFace(0);
	const FloatRect &blockTexCoords = job.textureAtlas->getTexCoords(texture);

	float faceTexCoords[nVertsPerFace][nCoordsPerUV] = {
		{blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y + blockTexCoords.sizeY},
		{blockTexCoords.x + blockTexCoords.sizeX, blockTexCoords.y},
		{blockTexCoords.x,                        blockTexCoords.y},
	};

	for (int f = 0; f < nCrossFaces ; ++f) {
		Vertex vertices[nVertsPerFace];
		for (int v = 0 ; v < nVertsPerFace ; ++v) {
			vertices[v].coord3d[0] = x + faceVertices[f][v]->x + blockState.drawOffset().x;
			vertices[v].coord3d[1] = y + faceVertices[f][v]->y + blockState.drawOffset().y;
			vertices[v].coord3d[2] = z + faceVertices[f][v]->z + blockState.drawOffset().z;
			vertices[v].coord3d[3] = 6;

			vertices[v].normal[0] = 0;
			vertices[v].normal[1] = 0;
			vertices[v].normal[2] = 0;

			const Color colorMultiplier = blockState.colorMultiplier();
			vertices[v].color[0] = colorMultiplier.r;
			vertices[v].color[1] = colorMultiplier.g;
			vertices[v].color[2] = colorMultiplier.b;
			vertices[v].color[3] = colorMultiplier.a;

			vertices[v].texCoord[0] = faceTexCoords[v][0];
			vertices[v].texCoord[1] = faceTexCoords[v][1];

			vertices[v].lightValue[0] = job.chunkData.getSunlight(x, y, z);
			vertices[v].lightValue[1] = job.chunkData.getTorchlight(x, y, z);

			vertices[v].ambientOcclusion = 4;
		}

		job.vertices[ChunkMeshLayer::Flora].emplace_back(vertices[0]);
		job.vertices[ChunkMeshLayer::Flora].emplace_back(vertices[1]);
		job.vertices[ChunkMeshLayer::Flora].emplace_back(vertices[3]);
		job.vertices[ChunkMeshLayer::Flora].emplace_back(vertices[3]);
		job.vertices[ChunkMeshLayer::Flora].emplace_back(vertices[1]);
		job.vertices[ChunkMeshLayer::Flora].emplace_back(vertices[2]);

		job.totalVertexCount += 6;
	}
}

