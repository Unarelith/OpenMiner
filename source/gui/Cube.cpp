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
#include "Color.hpp"
#include "Config.hpp"
#include "Cube.hpp"
#include "ResourceHandler.hpp"
#include "Vertex.hpp"

Cube::Cube(float size) : m_texture(ResourceHandler::getInstance().get<Texture>("texture-blocks")) {
	m_size = size;

	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-180.0f), glm::vec3{0, 0, 1});
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-30.0f), glm::vec3{1, 0, 0});
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-45.0f), glm::vec3{0, 1, 0});
}

void Cube::updateVertexBuffer(const Block &block) const {
	Vertex vertices[6 * 4] = {
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

		// Bottom
		{{0, 0, 0, -1}},
		{{m_size, 0, 0, -1}},
		{{m_size, 0, m_size, -1}},
		{{0, 0, m_size, -1}},

		// Top
		{{0, m_size, m_size, 3}},
		{{m_size, m_size, m_size, 3}},
		{{m_size, m_size, 0, 3}},
		{{0, m_size, 0, 3}},

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
		const glm::vec4 &blockTexCoords = block.getTexCoords(i, 0);
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

	Color color = Color::white;
	for (u8 i = 0 ; i < 6 * 4 ; ++i) {
		vertices[i].color[0] = color.r;
		vertices[i].color[1] = color.g;
		vertices[i].color[2] = color.b;
		vertices[i].color[3] = color.a;
	}

	VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void Cube::draw(RenderTarget &target, RenderStates states) const {
	glm::mat4 tmpMatrix;
	if (states.modelMatrix) {
		tmpMatrix = *states.modelMatrix * m_modelMatrix;
		states.modelMatrix = &tmpMatrix;
	}

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

