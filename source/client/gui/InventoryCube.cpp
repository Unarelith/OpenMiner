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
#include "common/core/BlockGeometry.hpp"
#include "common/core/EngineConfig.hpp"
#include "common/math/Math.hpp"
#include "common/resource/ResourceHandler.hpp"
#include "common/world/Block.hpp"

#include "client/core/Config.hpp"
#include "client/graphics/TextureAtlas.hpp"
#include "client/graphics/Vertex.hpp"
#include "client/gui/InventoryCube.hpp"

InventoryCube::InventoryCube(float size, bool isEntity)
	: m_textureAtlas(&ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	m_vbo.setupDefaultLayout();

	m_size = size;
	m_isEntity = isEntity;

	if (!m_isEntity) {
		m_transform.setOrigin(size * 0.5f, size * 0.5f, size * 0.5f);

		// NOTE: intrinsic rotations! The axis is the local axis of the object.
		// Note also that we start looking at the bottom of the cube due to how
		// glm::ortho is used (see comment below).
		m_transform.rotateX(120.f);
		m_transform.rotateZ(-45.f);
	}

	m_view.setSize((float)Config::screenWidth, (float)Config::screenHeight);
	m_view.setCenter((float)Config::screenWidth / 2.f, (float)Config::screenHeight / 2.f);
}

using namespace BlockGeometry;

void InventoryCube::updateVertexBuffer(const Block &block, u8 state) {
	if (!block.id() || (block.id() == m_blockID && state == m_blockState)) return;

	m_blockID = block.id();
	m_blockState = state;

	const BlockState &blockState = block.getState(state);

	Vertex vertices[nFaces][nVertsPerFace];

	glm::vec3 vertexPos[nVertsPerCube] {
		// Order is important. It matches the bit order defined in BlockGeometry::cubeVerts.
		glm::vec3{0,      0,      0},
		glm::vec3{m_size, 0,      0},
		glm::vec3{0,      m_size, 0},
		glm::vec3{m_size, m_size, 0},
		glm::vec3{0,      0,      m_size},
		glm::vec3{m_size, 0,      m_size},
		glm::vec3{0,      m_size, m_size},
		glm::vec3{m_size, m_size, m_size},
	};

	const FloatBox &boundingBox = blockState.boundingBox();

	constexpr s8f faceValue[nFaces]{2, 2, 4, 4, 3, 3};

	for (u8 f = 0; f < nFaces; ++f) {
		// Calculate UV's
		// These are tough to obtain. Note that texture Y grows in the up-down direction, and so does V.
		// Vertex index in the bitmap array and U/V correspondence is:
		//    U0V0 -> 3 2 <- U1V0
		//    U0V1 -> 0 1 <- U1V1
		float U0, V0, U1, V1;
		if (blockState.drawType() == BlockDrawType::Cactus) {
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

		const FloatRect &blockTexCoords = m_textureAtlas->getTexCoords(blockState.tiles().getTextureForFace(f));

		for (u8f v = 0; v < nVertsPerFace; ++v) {
			if (blockState.drawType() == BlockDrawType::Cactus) {
				vertices[f][v].coord3d[0] = vertexPos[cubeVerts[f][v]].x - boundingBox.x * faceNormals[f][0] * m_size;
				vertices[f][v].coord3d[1] = vertexPos[cubeVerts[f][v]].y - boundingBox.y * faceNormals[f][1] * m_size;
				vertices[f][v].coord3d[2] = vertexPos[cubeVerts[f][v]].z - boundingBox.z * faceNormals[f][2] * m_size;
			}
			else {
				vertices[f][v].coord3d[0] = vertexPos[cubeVerts[f][v]].x * boundingBox.sizeX + boundingBox.x;
				vertices[f][v].coord3d[1] = vertexPos[cubeVerts[f][v]].y * boundingBox.sizeY + boundingBox.y;
				vertices[f][v].coord3d[2] = vertexPos[cubeVerts[f][v]].z * boundingBox.sizeZ + boundingBox.z;
			}
			vertices[f][v].coord3d[3] = faceValue[f];

			float U = (v == 0 || v == 3) ? U0 : U1;
			float V = (v >= 2) ? V0 : V1;
			vertices[f][v].texCoord[0] = math::qlerpf(blockTexCoords.x, blockTexCoords.x + blockTexCoords.sizeX, U);
			vertices[f][v].texCoord[1] = math::qlerpf(blockTexCoords.y, blockTexCoords.y + blockTexCoords.sizeY, V);

			const Color &colorMultiplier = blockState.colorMultiplier();
			vertices[f][v].color[0] = colorMultiplier.r;
			vertices[f][v].color[1] = colorMultiplier.g;
			vertices[f][v].color[2] = colorMultiplier.b;
			vertices[f][v].color[3] = colorMultiplier.a;

			// FIXME: Find a way to get the light from the position
			if (m_isEntity) {
				vertices[f][v].lightValue[0] = 15;
				vertices[f][v].lightValue[1] = 0;
			}
		}
	}

	m_vbo.init(vertices, sizeof(vertices), true);
}

void InventoryCube::draw(RenderTarget &target, RenderStates states) const {
	if (!m_vbo.isValid()) return;

	states.transform *= getTransform();
	states.transform *= m_transform.getTransform();

	// NOTE: This matrix has Y inverted as well as Z. This means that
	// negative Z is closer to the user, and that the bottom side is visible
	// at start.
	target.setView(m_view);

	states.texture = &m_textureAtlas->texture();

	states.isDepthTestEnabled = false;

	target.drawElements(m_vbo, target.cubeIndexBuffer(), 0, 0, states);
}
