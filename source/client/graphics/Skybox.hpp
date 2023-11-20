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
#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include <vector>

#include "client/graphics/CelestialObject.hpp"
#include "client/graphics/Shader.hpp"

class Camera;
class ClientWorld;
class Sky;

class Skybox : public Drawable, public Transformable {
	public:
		Skybox(Camera &camera, ClientWorld &world);
		~Skybox();

		void loadSky(const Sky &sky);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Camera &m_camera;
		ClientWorld &m_world;

		Shader m_shader;

		CelestialObject m_sun;
		CelestialObject m_moon;

		std::vector<CelestialObject> m_stars;

		bgfx::UniformHandle m_starColor = BGFX_INVALID_HANDLE;
};

#endif // SKYBOX_HPP_
