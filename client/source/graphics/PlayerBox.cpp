/*
 * =====================================================================================
 *
 *       Filename:  PlayerBox.cpp
 *
 *    Description:
 *
 *        Created:  17/03/2019 15:33:04
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/gl/Vertex.hpp>

#include "PlayerBox.hpp"

static const float cubeCoords[6 * 4 * 3] = {
	// Left
	0, 0, 0,
	0, 0, 1,
	0, 1, 1,
	0, 1, 0,

	// Right
	1, 0, 1,
	1, 0, 0,
	1, 1, 0,
	1, 1, 1,

	// Bottom
	0, 0, 0,
	1, 0, 0,
	1, 0, 1,
	0, 0, 1,

	// Top
	0, 1, 1,
	1, 1, 1,
	1, 1, 0,
	0, 1, 0,

	// Front
	1, 0, 0,
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,

	// Back
	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1,
};

PlayerBox::PlayerBox() {
	setScale(1, 2, 1);

	updateVertexBuffer();
}

void PlayerBox::updateVertexBuffer() {
	gk::Vertex vertices[24];
	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = cubeCoords[i * 3];
		vertices[i].coord3d[1] = cubeCoords[i * 3 + 1] - 0.5;
		vertices[i].coord3d[2] = cubeCoords[i * 3 + 2];
		vertices[i].coord3d[3] = -1;

		vertices[i].color[0] = 0.3f;
		vertices[i].color[1] = 0.8f;
		vertices[i].color[2] = 0.7f;
		vertices[i].color[3] = 1.0f;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void PlayerBox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_vbo, GL_QUADS, 0, 24, states);
}

