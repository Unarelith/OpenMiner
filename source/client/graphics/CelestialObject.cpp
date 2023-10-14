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
#include <gk/gl/Texture.hpp>
#include <gk/gl/Vertex.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "CelestialObject.hpp"
#include "GameTime.hpp"
#include "Vertex.hpp"

CelestialObject::CelestialObject() {
	m_vbo.layout().setupDefaultLayout();
}

void CelestialObject::setTexture(const std::string &textureName) {
	if (textureName.empty()) return;

	m_texture = &gk::ResourceHandler::getInstance().get<gk::Texture>(textureName);

	m_isUpdateNeeded = true;
}

void CelestialObject::updateVertexBuffer() const {
	if (m_width <= 0.f || m_height <= 0.f) {
		gkError() << "Trying to update vertex buffer for celestial object of invalid size";
		return;
	}

	gk::Vertex vertices[4] = {
		// Rectangle vertices
		{{0,  m_width / 2.f, -m_height / 2.f, -1}},
		{{0, -m_width / 2.f, -m_height / 2.f, -1}},
		{{0, -m_width / 2.f,  m_height / 2.f, -1}},
		{{0,  m_width / 2.f,  m_height / 2.f, -1}},
	};

	for (u8 i = 0 ; i < 4 ; ++i) {
		vertices[i].color[0] = m_color.r;
		vertices[i].color[1] = m_color.g;
		vertices[i].color[2] = m_color.b;
		vertices[i].color[3] = m_color.a;
	}

	if (m_texture) {
		gk::FloatRect texRect{0, 0, 1, 1};

		if (m_phaseCount && m_phaseSize && m_currentPhase < m_phaseCount) {
			u16 currentPhaseX = u16(m_currentPhase % (m_texture->getSize().x / m_phaseSize));
			u16 currentPhaseY = u16(m_currentPhase / (m_texture->getSize().x / m_phaseSize));
			texRect.x = currentPhaseX * m_phaseSize / float(m_texture->getSize().x);
			texRect.y = currentPhaseY * m_phaseSize / float(m_texture->getSize().y);
			texRect.sizeX = m_phaseSize / float(m_texture->getSize().x);
			texRect.sizeY = m_phaseSize / float(m_texture->getSize().y);
		}

		vertices[0].texCoord[0] = texRect.x + texRect.sizeX;
		vertices[0].texCoord[1] = texRect.y;
		vertices[1].texCoord[0] = texRect.x;
		vertices[1].texCoord[1] = texRect.y;
		vertices[2].texCoord[0] = texRect.x;
		vertices[2].texCoord[1] = texRect.y + texRect.sizeY;
		vertices[3].texCoord[0] = texRect.x + texRect.sizeX;
		vertices[3].texCoord[1] = texRect.y + texRect.sizeY;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	gk::VertexBuffer::bind(nullptr);

	m_isUpdateNeeded = false;
}

void CelestialObject::updateAxisTransform() const {
	// Set m_rotAxisTransform to a rotation that points the local Y axis
	// towards m_rotationAxis, while keeping the local X axis horizontal.
	// Keeping the X axis horizontal guarantees that the sun and moon cycle
	// still make some sense, that is, that the sun goes above the horizon
	// at 6:00 and below at 18:00 and the moon does the opposite.
	if (m_rotationAxis.x == 0.0f && m_rotationAxis.y == 0.0f) {
		// The axis is completely vertical. This does not make much sense,
		// because the sun and the moon will rotate horizontally following
		// the horizon, but let's not crash! There are infinite rotations
		// that can keep the sun and moon in the horizon. Let's pick one:
		// local X axis pointing to world's X, local Y axis pointing to
		// world's Z, and local Z axis pointing to world's -Y. That places
		// Polaris at the top, and the sun to the East at 6:00 and to the
		// South at 12:00.
		m_rotAxisTransform = {1,0,0,0, 0,0,1,0, 0,-1,0,0, 0,0,0,1};
	}
	else {
		// A reference vector that points to the world's Z.
		glm::vec3 tmp{0.f, 0.f, 1.f};
		// Normalize the rotation axis
		glm::vec3 axis = glm::normalize(glm::vec3{m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z});
		// Point the Y vector of the matrix to the normalized axis
		m_rotAxisTransform[1].x = axis.x;
		m_rotAxisTransform[1].y = axis.y;
		m_rotAxisTransform[1].z = axis.z;
		// Follow the right hand rule (Y x Z = X) to obtain a local X vector
		// perpendicular to both the rotation axis and the world's Z vector,
		// using a cross product. That gives us our horizontal axis (local X
		// vector). Normalize the result, as the two vectors implied may not
		// be perpendicular even if they are both normalized.
		tmp = glm::normalize(glm::cross(axis, tmp));
		// Place it in the X vector of the matrix
		m_rotAxisTransform[0].x = tmp.x;
		m_rotAxisTransform[0].y = tmp.y;
		m_rotAxisTransform[0].z = tmp.z;
		// To complete the orthogonal basis, we need a third vector
		// perpendicular to the other two. Follow the right hand rule to
		// obtain the Z vector (X x Y = Z) with a cross product. The result
		// does not need normalization because both input vectors are already
		// perpendicular to each other and normalized.
		tmp = glm::cross(tmp, axis);
		// Place it in the Z vector of the matrix
		m_rotAxisTransform[2].x = tmp.x;
		m_rotAxisTransform[2].y = tmp.y;
		m_rotAxisTransform[2].z = tmp.z;
		// Fill in the rest of fields of the matrix (4th row and column)
		m_rotAxisTransform[0].w = 0.f;
		m_rotAxisTransform[1].w = 0.f;
		m_rotAxisTransform[2].w = 0.f;
		m_rotAxisTransform[3].x = 0.f;
		m_rotAxisTransform[3].y = 0.f;
		m_rotAxisTransform[3].z = 0.f;
		m_rotAxisTransform[3].w = 1.f;
	}
	m_axisXfNeedsUpdate = false;
}

void CelestialObject::draw(RenderTarget &target, RenderStates states) const {
	if (m_isUpdateNeeded)
		updateVertexBuffer();

	if (m_axisXfNeedsUpdate)
		updateAxisTransform();

	states.transform *= m_rotAxisTransform;
	states.transform.rotateY(-GameTime::getCurrentTime(m_rotationOffset, m_rotationSpeed) * 360.f);
	states.transform *= getTransform();

	if (m_texture)
		states.texture = m_texture;

	static const GLubyte indices[] = {
		0, 1, 3,
		3, 1, 2
	};

	target.drawElements(m_vbo, GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices, states);
}

