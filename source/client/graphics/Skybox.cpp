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
#include "ClientWorld.hpp"
#include "GameTime.hpp"
#include "Sky.hpp"
#include "Skybox.hpp"

Skybox::Skybox(gk::Camera &camera, ClientWorld &world) : m_camera(camera), m_world(world) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/skybox.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/skybox.f.glsl");
	m_shader.linkProgram();
}

void Skybox::loadSky(const Sky &sky) {
	const Sky::SunDefinition &sun = sky.sunDefinition();
	m_sun = CelestialObject{};
	m_sun.setSize(sun.size, sun.size);
	m_sun.setPosition(500, -m_sun.width() / 2, -m_sun.height() / 2);
	m_sun.setRotationSpeed(sky.daylightCycleSpeed());

	try {
		m_sun.setTexture(sun.texture);
	}
	catch (...) {
		m_sun.setColor(gk::Color::Yellow);
		gkWarning() << "Failed to load sun texture" << sun.texture;
	}

	const Sky::MoonDefinition &moon = sky.moonDefinition();
	m_moon = CelestialObject{};
	m_moon.setSize(moon.size, moon.size);
	m_moon.setPosition(-500, -m_moon.width() / 2, -m_moon.height() / 2);
	m_moon.setPhaseCount(moon.phaseCount, moon.phaseSize);
	m_moon.setCurrentPhase(0);
	m_moon.setRotationSpeed(sky.daylightCycleSpeed());

	try {
		m_moon.setTexture(moon.texture);
	}
	catch (...) {
		m_moon.setColor(gk::Color{240, 240, 240});
		gkWarning() << "Failed to load moon texture" << sun.texture;
	}

	const Sky::StarsDefinition &stars = sky.starsDefinition();
	m_stars.clear();
	m_stars.reserve(stars.count);
	for (int i = 0 ; i < stars.count ; ++i) {
		auto &star = m_stars.emplace_back();
		star.setColor(gk::Color{0, 0, 0, 0});
		star.setSize(stars.size, stars.size);
		star.setPosition(650 * ((rand() % 2) * 2 - 1), (rand() % 500) * 2 - 500, (rand() % 500) * 2 - 500);
		star.setRotationOffset(rand() % GameTime::dayLength);
		star.setRotationAxis({rand() % 100 / 100.f, rand() % 100 / 100.f, rand() % 100 / 100.f});
		star.setRotationSpeed(sky.daylightCycleSpeed());
	}
}

void Skybox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!m_world.sky()) return;

	float time = GameTime::getCurrentTime(0, m_world.sky()->daylightCycleSpeed());
	gk::Color skyColor = GameTime::getSkyColorFromTime(*m_world.sky(), time);
	gk::Color starColor = m_world.sky()->color();

	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_skyColor", skyColor);
	m_shader.setUniform("u_starColor", starColor);
	gk::Shader::bind(nullptr);

	m_moon.setCurrentPhase((GameTime::getTicks() / GameTime::dayLength) % 8);

	states.shader = &m_shader;

	// Subtract the camera position - see comment in ClientWorld::draw()
	const gk::Vector3d &cameraPosition = m_camera.getDPosition();
	states.transform.translate(cameraPosition.x, cameraPosition.y, cameraPosition.z - 50);

	if (m_sun.width() && m_sun.height())
		target.draw(m_sun, states);

	if (m_moon.width() && m_moon.height())
		target.draw(m_moon, states);

	if (Config::isStarRenderingEnabled && skyColor != starColor) {
		glDisable(GL_CULL_FACE);

		for (auto &it : m_stars)
			target.draw(it, states);
	}
}

