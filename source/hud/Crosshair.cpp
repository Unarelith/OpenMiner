/*
 * =====================================================================================
 *
 *       Filename:  Crosshair.cpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 18:10:10
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Crosshair.hpp"

Crosshair::Crosshair() {
	float cross[4][3] = {
		{-0.04, 0, 0},//, 13},
		{+0.04, 0, 0},//, 13},
		{0, -0.05, 0},//, 13},
		{0, +0.05, 0},//, 13}
	};

	VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(cross), cross, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void Crosshair::draw(Shader &shader) {
	glDisable(GL_DEPTH_TEST);

	VertexBuffer::bind(&m_vbo);

	shader.setUniform("u_modelMatrix", glm::mat4(1));
	shader.setUniform("u_viewMatrix", glm::mat4(1));
	shader.setUniform("u_projectionMatrix", glm::mat4(1));

	shader.enableVertexAttribArray("coord3d");

	glVertexAttribPointer(shader.attrib("coord3d"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINES, 0, 4);

	shader.disableVertexAttribArray("coord3d");

	VertexBuffer::bind(nullptr);

	glEnable(GL_DEPTH_TEST);
}

