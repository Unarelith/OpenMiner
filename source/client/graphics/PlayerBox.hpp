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
#ifndef PLAYERBOX_HPP_
#define PLAYERBOX_HPP_

#include <gk/gl/Transformable.hpp>

#include "Camera.hpp"
#include "Drawable.hpp"
#include "IndexBuffer.hpp"
#include "Player.hpp"
#include "VertexBuffer.hpp"

class PlayerBox : public Drawable, public gk::Transformable, public Player {
	public:
		PlayerBox(const Camera &camera);

		void setPosition(double x, double y, double z) {
			Player::setPosition(x, y, z);
		}

	private:
		void initVertexBuffer();
		void initIndexBuffer();

		void draw(RenderTarget &target, RenderStates states) const override;

		VertexBuffer m_vbo;
		IndexBuffer m_ibo;

		const Camera &m_camera;

		Texture &m_texture;
};

#endif // PLAYERBOX_HPP_
