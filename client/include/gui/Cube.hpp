/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef CUBE_HPP_
#define CUBE_HPP_

#include <gk/gl/Drawable.hpp>
#include <gk/gl/Transformable.hpp>
#include <gk/gl/VertexBuffer.hpp>

class Block;
class TextureAtlas;

class Cube : public gk::Drawable, public gk::Transformable {
	public:
		Cube(float size = 1.0f);

		void updateVertexBuffer(const Block &block);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		float m_size = 1.0f;

		const TextureAtlas &m_textureAtlas;

		gk::VertexBuffer m_vbo;
		bool m_isVboInitialized = false;

		glm::mat4 m_modelMatrix{1};
};

#endif // CUBE_HPP_
