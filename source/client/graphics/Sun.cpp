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
#include <gk/gl/GLCheck.hpp>
#include <gk/graphics/Color.hpp>

#include "Sun.hpp"
#include "Vertex.hpp"

Sun::Sun() {
	updateVertexBuffer();
}

void Sun::updateVertexBuffer() const {
	float width = 20.f;
	float height = 20.f;
	Vertex vertices[4] = {
		// Rectangle vertices
		{{0, width,      0, -1}},
		{{0,     0,      0, -1}},
		{{0,     0, height, -1}},
		{{0, width, height, -1}},
	};

	gk::Color color = gk::Color::Yellow;
	for (u8 i = 0 ; i < 4 ; ++i) {
		vertices[i].color[0] = color.r;
		vertices[i].color[1] = color.g;
		vertices[i].color[2] = color.b;
		vertices[i].color[3] = color.a;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void Sun::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	// states.transform *= getTransform();

	states.vertexAttributes = VertexAttribute::All;

	// glCheck(glEnable(GL_CULL_FACE));
	// glCheck(glEnable(GL_DEPTH_TEST));

	static const GLubyte indices[] = {
		0, 1, 3,
		3, 1, 2
	};

	target.drawElements(m_vbo, GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices, states);
}

