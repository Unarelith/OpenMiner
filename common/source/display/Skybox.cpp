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

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Player.hpp"
#include "Skybox.hpp"
#include "World.hpp"

Skybox::Skybox() {
	const float cubeCoords[6 * 4 * 3] = {
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


	float colors[3 * 4 * 6] = {
		//0.196078, 0.6, 0.8,
		//0.196078, 0.6, 0.8,
		//0.196078, 0.6, 0.8,
		//0.196078, 0.6, 0.8
		1.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0
	};

	for(int i = 1 ; i < 6 ; i++) {
		memcpy(&colors[3 * 4 * i], &colors[0], 3 * 4 * sizeof(float));
	}

	gk::VertexBuffer::bind(&m_vbo);

	m_vbo.setData(36 * 16 * 9 * sizeof(float), nullptr, GL_STATIC_DRAW);
	m_vbo.updateData(0, 6 * 4 * 3 * sizeof(float), cubeCoords);
	m_vbo.updateData(6 * 4 * 3 * sizeof(float), 6 * 4 * 3 * sizeof(float), colors);

	gk::VertexBuffer::bind(nullptr);
}

void Skybox::draw(gk::Shader &shader) {
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),
	                        glm::vec3(Player::getInstance().x() - World::renderDistance * Chunk::width / 2,
	                                  Player::getInstance().y() - World::renderDistance * Chunk::width / 2,
	                                  Player::getInstance().z() - World::renderDistance * Chunk::width / 2));
	modelMatrix *= glm::scale(glm::mat4(1.0), glm::vec3(World::renderDistance * Chunk::width));

	shader.setUniform("u_modelMatrix", modelMatrix);

	gk::VertexBuffer::bind(&m_vbo);

	shader.enableVertexAttribArray("coord3d");
	shader.enableVertexAttribArray("color");

	glVertexAttribPointer(shader.attrib("coord3d"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(shader.attrib("color"), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(3 * 4 * 6 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, 4 * 6);

	shader.disableVertexAttribArray("color");
	shader.disableVertexAttribArray("coord3d");

	gk::VertexBuffer::bind(nullptr);
}

