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
#ifndef INVENTORYCUBE_HPP_
#define INVENTORYCUBE_HPP_

#include <gk/gl/Drawable.hpp>
#include <gk/gl/Transformable.hpp>
#include <gk/gl/VertexBuffer.hpp>

class Block;
class TextureAtlas;

class InventoryCube : public gk::Drawable, public gk::Transformable {
	public:
		InventoryCube(float size = 1.0f, bool isEntity = false);

		void updateVertexBuffer(const Block &block);

		float size() const { return m_size; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		float m_size = 1.0f;

		const TextureAtlas *m_textureAtlas;

		gk::VertexBuffer m_vbo;
		bool m_isVboInitialized = false;

		gk::Transformable m_transform;

		bool m_isEntity = false;
};

#endif // INVENTORYCUBE_HPP_
