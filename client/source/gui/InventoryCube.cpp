/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/graphics/Color.hpp>
#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Vertex.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Block.hpp"
#include "Config.hpp"
#include "InventoryCube.hpp"
#include "TextureAtlas.hpp"

InventoryCube::InventoryCube(float size) : m_textureAtlas(gk::ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks")) {
	m_size = size;

	// FIXME: Using Transform may be better here
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-180.0f), glm::vec3{0, 0, 1});
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-30.0f), glm::vec3{1, 0, 0});
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-45.0f), glm::vec3{0, 1, 0});
}

void InventoryCube::updateVertexBuffer(const Block &block) {
	if (!block.id()) return;

	gk::Vertex vertices[6 * 4] = {
		// Top
		{{m_size, m_size, 0, 3}},
		{{0, m_size, 0, 3}},
		{{0, m_size, m_size, 3}},
		{{m_size, m_size, m_size, 3}},

		// Bottom
		{{0, 0, 0, -1}},
		{{m_size, 0, 0, -1}},
		{{m_size, 0, m_size, -1}},
		{{0, 0, m_size, -1}},

		// Left
		{{0, 0, 0, 2}},
		{{0, 0, m_size, 2}},
		{{0, m_size, m_size, 2}},
		{{0, m_size, 0, 2}},

		// Right
		{{m_size, 0, m_size, -1}},
		{{m_size, 0, 0, -1}},
		{{m_size, m_size, 0, -1}},
		{{m_size, m_size, m_size, -1}},

		// Front
		{{m_size, 0, 0, 4}},
		{{0, 0, 0, 4}},
		{{0, m_size, 0, 4}},
		{{m_size, m_size, 0, 4}},

		// Back
		{{0, 0, m_size, -1}},
		{{m_size, 0, m_size, -1}},
		{{m_size, m_size, m_size, -1}},
		{{0, m_size, m_size, -1}},
	};

	for (u8 i = 0 ; i < 6 ; ++i) {
		const gk::FloatRect &blockTexCoords = m_textureAtlas.getTexCoords(block.tiles().getTextureForFace(i)); // block.getTexCoords(i, 0);
		float faceTexCoords[2 * 4] = {
			blockTexCoords.x,                        blockTexCoords.y + blockTexCoords.height,
			blockTexCoords.x + blockTexCoords.width, blockTexCoords.y + blockTexCoords.height,
			blockTexCoords.x + blockTexCoords.width, blockTexCoords.y,
			blockTexCoords.x,                        blockTexCoords.y
		};
		for(u8 j = 0 ; j < 4 ; j++) {
			vertices[j + i * 4].texCoord[0] = faceTexCoords[j * 2];
			vertices[j + i * 4].texCoord[1] = faceTexCoords[j * 2 + 1];

			vertices[j + i * 4].coord3d[0] = vertices[j + i * 4].coord3d[0] * block.boundingBox().width  + block.boundingBox().x;
			vertices[j + i * 4].coord3d[1] = vertices[j + i * 4].coord3d[1] * block.boundingBox().height + block.boundingBox().y;
			vertices[j + i * 4].coord3d[2] = vertices[j + i * 4].coord3d[2] * block.boundingBox().depth  + block.boundingBox().z;
		}
	}

	gk::Color color = gk::Color::White;
	for (u8 i = 0 ; i < 6 * 4 ; ++i) {
		vertices[i].color[0] = color.r;
		vertices[i].color[1] = color.g;
		vertices[i].color[2] = color.b;
		vertices[i].color[3] = color.a;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);

	m_isVboInitialized = true;
}

void InventoryCube::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!m_isVboInitialized) return;

	states.transform *= m_modelMatrix;
	states.transform *= getTransform();

	states.viewMatrix = gk::Transform::Identity;

	states.projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -40.0f, DIST_FAR);

	states.texture = &m_textureAtlas.texture();
	states.vertexAttributes = gk::VertexAttribute::Only2d;

	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDisable(GL_DEPTH_TEST));

	target.draw(m_vbo, GL_QUADS, 4 * 0, 4, states);
	// target.draw(m_vbo, GL_QUADS, 4 * 1, 4, states);
	target.draw(m_vbo, GL_QUADS, 4 * 2, 4, states);
	// target.draw(m_vbo, GL_QUADS, 4 * 3, 4, states);
	target.draw(m_vbo, GL_QUADS, 4 * 4, 4, states);
	// target.draw(m_vbo, GL_QUADS, 4 * 5, 4, states);

	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glEnable(GL_CULL_FACE));

}

