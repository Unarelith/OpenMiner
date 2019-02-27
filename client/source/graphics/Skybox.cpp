/*
 * =====================================================================================
 *
 *       Filename:  Skybox.cpp
 *
 *    Description:
 *
 *        Created:  25/12/2014 23:43:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <cstring>

#include <glm/gtc/matrix_transform.hpp>

#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Vertex.hpp>

#include "ClientPlayer.hpp"
#include "Config.hpp"
#include "Skybox.hpp"
#include "World.hpp"

static const float cubeCoords[6 * 4 * 3] = {
	0, 0, 1,
	0, 0, 0,
	0, 1, 0,
	0, 1, 1,

	1, 1, 1,
	1, 1, 0,
	1, 0, 0,
	1, 0, 1,

	0, 0, 1,
	1, 0, 1,
	1, 0, 0,
	0, 0, 0,

	0, 1, 0,
	1, 1, 0,
	1, 1, 1,
	0, 1, 1,

	0, 1, 1,
	1, 1, 1,
	1, 0, 1,
	0, 0, 1,

	0, 0, 0,
	1, 0, 0,
	1, 1, 0,
	0, 1, 0,
};

Skybox::Skybox() {
	float colors[3 * 4] = {
		//0.196078, 0.6, 0.8,
		//0.196078, 0.6, 0.8,
		//0.196078, 0.6, 0.8,
		//0.196078, 0.6, 0.8
		1.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0
	};


	gk::Vertex vertices[24];
	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = cubeCoords[i * 3];
		vertices[i].coord3d[1] = cubeCoords[i * 3 + 1];
		vertices[i].coord3d[2] = cubeCoords[i * 3 + 2];
		vertices[i].coord3d[3] = -1;

		vertices[i].color[0] = colors[(i % 4) * 3];
		vertices[i].color[1] = colors[(i % 4) * 3 + 1];
		vertices[i].color[1] = colors[(i % 4) * 3 + 2];
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void Skybox::update(ClientPlayer &player) {
	m_playerPosition = {player.x(), player.y(), player.z()};
}

void Skybox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),
	                        glm::vec3(m_playerPosition.x - Config::renderDistance * CHUNK_WIDTH / 2,
	                                  m_playerPosition.y - Config::renderDistance * CHUNK_WIDTH / 2,
	                                  m_playerPosition.z - Config::renderDistance * CHUNK_WIDTH / 2));
	modelMatrix *= glm::scale(glm::mat4(1.0), glm::vec3(Config::renderDistance * CHUNK_WIDTH));

	states.transform = modelMatrix;

	glCheck(glDisable(GL_CULL_FACE));

	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	target.draw(m_vbo, GL_QUADS, 0, 4 * 6, states);
	if(Config::isWireframeModeEnabled) glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

