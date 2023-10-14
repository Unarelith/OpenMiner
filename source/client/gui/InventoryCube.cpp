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
#include <glm/gtc/matrix_transform.hpp>

#include <gk/graphics/Color.hpp>
#include <gk/gl/GLCheck.hpp>
#include <gk/math/Math.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Block.hpp"
#include "BlockGeometry.hpp"
#include "Config.hpp"
#include "EngineConfig.hpp"
#include "InventoryCube.hpp"
#include "TextureAtlas.hpp"
#include "Vertex.hpp"

InventoryCube::InventoryCube(float size, bool isEntity)
	: m_textureAtlas(&gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	m_vbo.layout().addAttribute(0, "coord3d", 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, coord3d)));
	m_vbo.layout().addAttribute(1, "texCoord", 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord)));
	m_vbo.layout().addAttribute(2, "color", 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));

	if (isEntity) {
		m_vbo.layout().addAttribute(3, "normal", 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
		m_vbo.layout().addAttribute(4, "lightValue", 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, lightValue)));
		m_vbo.layout().addAttribute(5, "ambientOcclusion", 1, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, ambientOcclusion)));
	}

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
}

using namespace BlockGeometry;

void InventoryCube::updateVertexBuffer(const Block &block, u8 state) {
	if (!block.id()) return;

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

	const gk::FloatBox &boundingBox = blockState.boundingBox();

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

		const gk::FloatRect &blockTexCoords = m_textureAtlas->getTexCoords(blockState.tiles().getTextureForFace(f));

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
			vertices[f][v].texCoord[0] = gk::qlerpf(blockTexCoords.x, blockTexCoords.x + blockTexCoords.sizeX, U);
			vertices[f][v].texCoord[1] = gk::qlerpf(blockTexCoords.y, blockTexCoords.y + blockTexCoords.sizeY, V);

			const gk::Color &colorMultiplier = blockState.colorMultiplier();
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

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);

	m_isVboInitialized = true;
}

void InventoryCube::draw(gk::RenderTarget &target, RenderStates states) const {
	if (!m_isVboInitialized) return;

	states.transform *= getTransform();
	states.transform *= m_transform.getTransform();

	states.viewMatrix = gk::Transform::Identity;

	// NOTE: This matrix has Y inverted as well as Z. This means that
	// negative Z is closer to the user, and that the bottom side is visible
	// at start.
	states.projectionMatrix = glm::ortho(0.0f, (float)Config::screenWidth, (float)Config::screenHeight, 0.0f, DIST_2D_FAR, DIST_2D_NEAR);

	states.texture = &m_textureAtlas->texture();

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_DEPTH_TEST));

	target.draw(m_vbo, GL_QUADS, 0, nFaces * nVertsPerFace, states);
}
