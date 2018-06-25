/*
 * =====================================================================================
 *
 *       Filename:  Cube.cpp
 *
 *    Description:
 *
 *        Created:  26/06/2018 00:02:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Block.hpp"
#include "Config.hpp"
#include "Cube.hpp"
#include "ResourceHandler.hpp"
#include "Vertex.hpp"

Cube::Cube(float size) : m_texture(ResourceHandler::getInstance().get<Texture>("texture-blocks")) {
	m_size = size;
}

void Cube::updateVertexBuffer(const Block &block) const {
	Vertex vertices[6 * 4] = {
		// Left
		{{0, 0, 0, -1}},
		{{0, 0, m_size, -1}},
		{{0, m_size, m_size, -1}},
		{{0, m_size, 0, -1}},

		// Right
		{{m_size, 0, m_size, -1}},
		{{m_size, 0, 0, -1}},
		{{m_size, m_size, 0, -1}},
		{{m_size, m_size, m_size, -1}},

		// Bottom
		{{0, 0, 0, -1}},
		{{m_size, 0, 0, -1}},
		{{m_size, 0, m_size, -1}},
		{{0, 0, m_size, -1}},

		// Top
		{{0, m_size, m_size, -1}},
		{{m_size, m_size, m_size, -1}},
		{{m_size, m_size, 0, -1}},
		{{0, m_size, 0, -1}},

		// Front
		{{m_size, 0, 0, -1}},
		{{0, 0, 0, -1}},
		{{0, m_size, 0, -1}},
		{{m_size, m_size, 0, -1}},

		// Back
		{{0, 0, m_size, -1}},
		{{m_size, 0, m_size, -1}},
		{{m_size, m_size, m_size, -1}},
		{{0, m_size, m_size, -1}},
	};

	for (u8 i = 0 ; i < 6 ; ++i) {
		const glm::vec4 &blockTexCoords = block.getTexCoords(i);
		float faceTexCoords[2 * 4] = {
			blockTexCoords.x, blockTexCoords.w,
			blockTexCoords.z, blockTexCoords.w,
			blockTexCoords.z, blockTexCoords.y,
			blockTexCoords.x, blockTexCoords.y
		};
		for(u8 j = 0 ; j < 4 ; j++) {
			vertices[j + i * 4].texCoord[0] = faceTexCoords[j * 2];
			vertices[j + i * 4].texCoord[1] = faceTexCoords[j * 2 + 1];
		}
	}

	VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void Cube::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	states.viewMatrix = nullptr;

	static glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -40.0f, DIST_FAR);
	states.projectionMatrix = &projectionMatrix;

	states.texture = &m_texture;

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	target.draw(m_vbo, GL_QUADS, 4 * 0, 4, states);
	// target.draw(m_vbo, GL_QUADS, 4 * 1, 4, states);
	// target.draw(m_vbo, GL_QUADS, 4 * 2, 4, states);
	target.draw(m_vbo, GL_QUADS, 4 * 3, 4, states);
	target.draw(m_vbo, GL_QUADS, 4 * 4, 4, states);
	// target.draw(m_vbo, GL_QUADS, 4 * 5, 4, states);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}

