/*
 * =====================================================================================
 *
 *       Filename:  RectangleShape.cpp
 *
 *    Description:
 *
 *        Created:  27/09/2014 17:06:57
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Config.hpp"
#include "RectangleShape.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"

RectangleShape::RectangleShape(u16 width, u16 height, const Color &color) {
	m_color = color;

	setSize(width, height);
}

void RectangleShape::updateVertexBuffer() const {
	Vertex vertices[4] = {
		{{m_width, 0,        0, -1}},
		{{0,       0,        0, -1}},
		{{0,       m_height, 0, -1}},
		{{m_width, m_height, 0, -1}},
	};

	for (u8 i = 0 ; i < 4 ; ++i) {
		vertices[i].color[0] = m_color.r;
		vertices[i].color[1] = m_color.g;
		vertices[i].color[2] = m_color.b;
		vertices[i].color[3] = m_color.a;
	}

	// GLubyte indices[] = {
	// 	0, 1, 3,
	// 	3, 1, 2
	// };

	VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void RectangleShape::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	states.viewMatrix = nullptr;

	static glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);
	states.projectionMatrix = &projectionMatrix;

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	if(m_wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	target.draw(m_vbo, GL_QUADS, 0, 4, states);
	if(m_wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

