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

#include "client/graphics/Drawable.hpp"
#include "client/graphics/Transformable.hpp"
#include "client/graphics/VertexBuffer.hpp"
#include "client/graphics/View.hpp"

class Block;
class TextureAtlas;

class InventoryCube : public Drawable, public Transformable {
	public:
		InventoryCube(float size = 1.0f, bool isEntity = false);

		void updateVertexBuffer(const Block &block, u8 state = 0);

		float size() const { return m_size; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		float m_size = 1.0f;

		const TextureAtlas *m_textureAtlas;

		VertexBuffer m_vbo;

		Transformable m_transform;

		bool m_isEntity = false;

		View m_view;

		u16 m_blockID = 0;
		u16 m_blockState = 0;
};

#endif // INVENTORYCUBE_HPP_
