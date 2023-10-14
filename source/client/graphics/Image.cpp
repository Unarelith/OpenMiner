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
#include <gk/gl/Vertex.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Image.hpp"

Image::Image() {
	m_vbo.layout().setupDefaultLayout();
}

Image::Image(const std::string &textureName) : Image() {
	load(gk::ResourceHandler::getInstance().get<gk::Texture>(textureName));
}

Image::Image(const gk::Texture &texture) : Image() {
	load(texture);
}

void Image::load(const Image &image) {
	m_texture = image.m_texture;

	m_width = image.m_width;
	m_height = image.m_height;

	m_clipRect = image.m_clipRect;
	m_posRect = image.m_posRect;

	m_color = image.m_color;

	updateVertexBuffer();
}

void Image::load(const std::string &textureName) {
	load(gk::ResourceHandler::getInstance().get<gk::Texture>(textureName));
}

void Image::load(const gk::Texture &texture) {
	m_texture = &texture;

	m_width = (u16)m_texture->getSize().x;
	m_height = (u16)m_texture->getSize().y;

	setClipRect(0, 0, m_width, m_height);
	setPosRect(0, 0, m_width, m_height);
}

void Image::setTexture(const std::string &textureName) {
	m_texture = &gk::ResourceHandler::getInstance().get<gk::Texture>(textureName);
}

void Image::setClipRect(float x, float y, u16 width, u16 height) {
	m_clipRect = gk::FloatRect(x, y, width, height);

	m_posRect.sizeX = width;
	m_posRect.sizeY = height;

	updateVertexBuffer();
}

void Image::setPosRect(float x, float y, u16 width, u16 height) {
	m_posRect = gk::FloatRect(x, y, width, height);

	updateVertexBuffer();
}

void Image::updateVertexBuffer() const {
	gk::Vertex vertices[4] = {
		{{m_posRect.x + m_posRect.sizeX, m_posRect.y,                   0, -1}},
		{{m_posRect.x,                   m_posRect.y,                   0, -1}},
		{{m_posRect.x,                   m_posRect.y + m_posRect.sizeY, 0, -1}},
		{{m_posRect.x + m_posRect.sizeX, m_posRect.y + m_posRect.sizeY, 0, -1}},
	};

	gk::FloatRect texRect{
		m_clipRect.x / float(m_width),
		m_clipRect.y / float(m_height),
		m_clipRect.sizeX / float(m_width),
		m_clipRect.sizeY / float(m_height)
	};

	if (!m_isFlipped) {
		vertices[0].texCoord[0] = texRect.x + texRect.sizeX;
		vertices[0].texCoord[1] = texRect.y;
		vertices[1].texCoord[0] = texRect.x;
		vertices[1].texCoord[1] = texRect.y;
		vertices[2].texCoord[0] = texRect.x;
		vertices[2].texCoord[1] = texRect.y + texRect.sizeY;
		vertices[3].texCoord[0] = texRect.x + texRect.sizeX;
		vertices[3].texCoord[1] = texRect.y + texRect.sizeY;
	}
	else {
		vertices[0].texCoord[0] = texRect.x;
		vertices[0].texCoord[1] = texRect.y;
		vertices[1].texCoord[0] = texRect.x + texRect.sizeX;
		vertices[1].texCoord[1] = texRect.y;
		vertices[2].texCoord[0] = texRect.x + texRect.sizeX;
		vertices[2].texCoord[1] = texRect.y + texRect.sizeY;
		vertices[3].texCoord[0] = texRect.x;
		vertices[3].texCoord[1] = texRect.y + texRect.sizeY;
	}

	for (u8 i = 0 ; i < 4 ; ++i) {
		vertices[i].color[0] = m_color.r;
		vertices[i].color[1] = m_color.g;
		vertices[i].color[2] = m_color.b;
		vertices[i].color[3] = m_color.a;
	}

	VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void Image::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	states.texture = m_texture;

	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDisable(GL_DEPTH_TEST));

	static const GLubyte indices[] = {
		0, 1, 3,
		3, 1, 2
	};

	target.drawElements(m_vbo, GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices, states);
}
