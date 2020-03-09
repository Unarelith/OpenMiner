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
#ifndef CLOUDRENDERER_HPP_
#define CLOUDRENDERER_HPP_

#include <gk/gl/Camera.hpp>
#include <gk/gl/Drawable.hpp>
#include <gk/gl/Transformable.hpp>
#include <gk/gl/VertexBuffer.hpp>

class CloudRenderer : public gk::Drawable, public gk::Transformable {
	public:
		CloudRenderer(gk::Camera &camera) : m_camera(camera) {}

		void initCloudMap();
		void initVertexBuffer();

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Camera &m_camera;

		gk::VertexBuffer m_vbo;

		static constexpr u16 CLOUD_MAP_SIZE = 256;
		static constexpr u8 CLOUD_HEIGHT = 80;

		u8 m_cloudMap[CLOUD_MAP_SIZE][CLOUD_MAP_SIZE];
		std::size_t m_verticesCount = 0;
};

#endif // CLOUDRENDERER_HPP_
