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
#ifndef BGFXTESTSTATE_HPP_
#define BGFXTESTSTATE_HPP_

#include <bgfx/bgfx.h>

#include "Camera.hpp"
#include "DrawableState.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"

class Cube : public Drawable {
	public:
		Cube();

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Shader m_shader;
		VertexBuffer m_vbo;
		IndexBuffer m_ibo;
};

class BGFXTestState : public DrawableState {
	public:
		BGFXTestState();

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Cube m_cube;

		Camera m_camera;

		float m_view[16];
		float m_proj[16];
};

#endif // BGFXTESTSTATE_HPP_
