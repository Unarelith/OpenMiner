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
#include <gk/core/GameClock.hpp>

#include "ClientWorld.hpp"
#include "Sky.hpp"
#include "Skybox.hpp"

Skybox::Skybox(gk::Camera &camera, ClientWorld &world) : m_camera(camera), m_world(world) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/skybox.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/skybox.f.glsl");
	m_shader.linkProgram();

	m_sun.setColor(gk::Color::Yellow);
	m_sun.setSize(100, 100);
	m_sun.setPosition(300, -m_sun.width() / 2, -m_sun.height() / 2);
	m_sun.setTexture("texture-sun");

	m_moon.setColor(gk::Color{240, 240, 240});
	m_moon.setSize(20, 20);
	m_moon.setPosition(-300, -m_moon.width() / 2, -m_moon.height() / 2);
}

void Skybox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	// FIXME: Duplicated in GameState
	float time = std::fmod(gk::GameClock::getInstance().getTicks() * 1.f / 1000.f, 360.f) / 360.f;
	if (m_world.sky()) {
		const float pi = 3.1415927f;
		float sunlight = std::clamp(0.5f + std::sin(2 * pi * time) * 2.0f, 0.0f, 1.0f);

		gk::Color skyColor = m_world.sky()->color();
		skyColor.r = std::clamp(sunlight - (1 - skyColor.r), 0.0f, skyColor.r);
		skyColor.g = std::clamp(sunlight - (1 - skyColor.g), 0.0f, skyColor.g);
		skyColor.b = std::clamp(sunlight - (1 - skyColor.b), 0.0f, skyColor.b);

		gk::Shader::bind(&m_shader);
		m_shader.setUniform("u_skyColor", skyColor);
		gk::Shader::bind(nullptr);
	}

	states.shader = &m_shader;

	// Subtract the camera position - see comment in ClientWorld::draw()
	const gk::Vector3d &cameraPosition = m_camera.getDPosition();
	states.transform.translate(cameraPosition.x, cameraPosition.y, cameraPosition.z - 50);

	target.draw(m_sun, states);
	target.draw(m_moon, states);
}

