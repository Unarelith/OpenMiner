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
#include "Skybox.hpp"

Skybox::Skybox(gk::Camera &camera) : m_camera(camera) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/skybox.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/skybox.f.glsl");
	m_shader.linkProgram();

	m_sun.setColor(gk::Color::Yellow);
	m_sun.setPosition(300, -10, -10);

	m_moon.setColor(gk::Color{240, 240, 240});
	m_moon.setPosition(-300, -10, -10);
}

void Skybox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.shader = &m_shader;

	// Subtract the camera position - see comment in ClientWorld::draw()
	const gk::Vector3d &cameraPosition = m_camera.getDPosition();
	states.transform.translate(cameraPosition.x, cameraPosition.y, cameraPosition.z - 50);

	target.draw(m_sun, states);
	target.draw(m_moon, states);
}

