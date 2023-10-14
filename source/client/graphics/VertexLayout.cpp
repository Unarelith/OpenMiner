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
#include <gk/gl/RenderStates.hpp>
#include <gk/gl/VertexBufferLayout.hpp>

#include "Vertex.hpp"
#include "VertexLayout.hpp"

void VertexBufferLayout::setupDefaultLayout() {
	addAttribute(0, "coord3d", 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, coord3d)));
	addAttribute(1, "texCoord", 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, texCoord)));
	addAttribute(2, "color", 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
}

void VertexBufferLayout::enableLayout() const {
	assert(!m_attributes.empty());

	for (auto &attr : m_attributes) {
		glCheck(glEnableVertexAttribArray(attr.id));
		glCheck(glVertexAttribPointer(attr.id, attr.size, attr.type, attr.normalized, attr.stride, attr.offset));
	}
}

void VertexBufferLayout::disableLayout() const {
	for (auto &attr : m_attributes)
		glCheck(glDisableVertexAttribArray(attr.id));
}
