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
#include <gk/core/GameClock.hpp>
#include <gk/gl/GLCheck.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "CelestialObject.hpp"
#include "Vertex.hpp"

CelestialObject::CelestialObject() {
}

void CelestialObject::setTexture(const std::string &textureName) {
	m_texture = &gk::ResourceHandler::getInstance().get<gk::Texture>(textureName);

	m_isUpdateNeeded = true;
}

void CelestialObject::updateVertexBuffer() const {
	if (!m_width || !m_height) {
		gkError() << "Can't update vertex buffer for celestial object of size 0";
		return;
	}

	Vertex vertices[4] = {
		// Rectangle vertices
		{{0, m_width,        0, -1}},
		{{0,       0,        0, -1}},
		{{0,       0, m_height, -1}},
		{{0, m_width, m_height, -1}},
	};

	for (u8 i = 0 ; i < 4 ; ++i) {
		vertices[i].color[0] = m_color.r;
		vertices[i].color[1] = m_color.g;
		vertices[i].color[2] = m_color.b;
		vertices[i].color[3] = m_color.a;
	}

	if (m_texture) {
		vertices[0].texCoord[0] = 1.f;
		vertices[0].texCoord[1] = 0.f;
		vertices[1].texCoord[0] = 0.f;
		vertices[1].texCoord[1] = 0.f;
		vertices[2].texCoord[0] = 0.f;
		vertices[2].texCoord[1] = 1.f;
		vertices[3].texCoord[0] = 1.f;
		vertices[3].texCoord[1] = 1.f;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	gk::VertexBuffer::bind(nullptr);

	m_isUpdateNeeded = false;
}

void CelestialObject::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_isUpdateNeeded)
		updateVertexBuffer();

	states.transform.rotate(-fmod(gk::GameClock::getInstance().getTicks() * 1.f / 1000.f, 360), {0, 1, 0});
	states.transform *= getTransform();

	states.vertexAttributes = VertexAttribute::All;

	if (m_texture)
		states.texture = m_texture;

	static const GLubyte indices[] = {
		0, 1, 3,
		3, 1, 2
	};

	target.drawElements(m_vbo, GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices, states);
}

