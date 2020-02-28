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

#include <gk/gl/Camera.hpp>
#include <gk/gl/Drawable.hpp>
#include <gk/gl/VertexBuffer.hpp>
#include <gk/gl/Transformable.hpp>
#include <gk/gl/Texture.hpp>

#include "Player.hpp"

class PlayerBox : public gk::Drawable, public gk::Transformable, public Player {
	public:
		PlayerBox(const gk::Camera &camera);

		void setPosition(s32 x, s32 y, s32 z) {
			Player::setPosition(x, y, z);
		}

	private:
		void updateVertexBuffer();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::VertexBuffer m_vbo;

		const gk::Camera &m_camera;

		gk::Texture &m_texture;
};

#endif // PLAYERBOX_HPP_
