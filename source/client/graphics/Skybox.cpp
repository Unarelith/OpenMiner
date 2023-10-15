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

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

Skybox::Skybox(gk::Camera &camera, ClientWorld &world) : m_camera(camera), m_world(world) {
#ifdef OM_NOT_IMPLEMENTED
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/skybox.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/skybox.f.glsl");
	m_shader.linkProgram();
#endif // OM_NOT_IMPLEMENTED
}

void Skybox::loadSky(const Sky &sky) {
	const Sky::SunDefinition &sun = sky.sunDefinition();
	m_sun = CelestialObject{};
	m_sun.setSize(sun.size, sun.size);
	m_sun.setPosition(DIST_SUN, 0, 0);
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
	m_moon.setPosition(-DIST_MOON, 0, 0);
	m_moon.setPhaseCount(moon.phaseCount, moon.phaseSize);
	m_moon.setCurrentPhase(0);
	m_moon.setRotationSpeed(sky.daylightCycleSpeed());

	try {
		m_moon.setTexture(moon.texture);
	}
	catch (...) {
		m_moon.setColor(gk::Color::fromRGBA32(240, 240, 240));
		gkWarning() << "Failed to load moon texture" << moon.texture;
	}

	const Sky::StarsDefinition &stars = sky.starsDefinition();
	m_stars.clear();
	m_stars.reserve(stars.count);
	for (int i = 0 ; i < stars.count ; ++i) {
		auto &star = m_stars.emplace_back();
		star.setColor(gk::Color::fromRGBA32(0, 0, 0, 0));
		star.setSize(stars.size, stars.size);

		// This formula makes the distribution uniform
		glm::vec3 v{0.f, 1.f, 0.f};
		// i = 0 adds a Polaris star for reference. m_rotationAxis will point
		// to this star all the time.
		if (i != 0) {
			// Generate a uniform random coordinate
			// NOTE: MUST NOT USE FAST MATH!
			// Otherwise it risks taking the square root of a negative!
			v.y = (float(rand() % 32768) - 16383.5f) / 16383.5f;
			// Project it to the +X semicircle in the XY plane
			v.x = sqrtf(1.f - v.y*v.y);
		}
		v *= DIST_STARS;

		star.setPosition(v.x, v.y, v.z);
		// Rotate the star to make it face the camera
		star.rotateZ(atan2f(v.y, v.x) * float(180./M_PI));

		// Set a random rotation in the day cycle
		star.setRotationOffset(u16(rand() % GameTime::dayLength));

		star.setRotationSpeed(sky.daylightCycleSpeed());
		// Maybe sometimes stars could have a random axis?
		// star.setRotationAxis({rand() % 100 / 100.f, rand() % 100 / 100.f, rand() % 100 / 100.f});
	}
}

void Skybox::draw(RenderTarget &target, RenderStates states) const {
#ifdef OM_NOT_IMPLEMENTED
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

	// Set the camera temporarily to zero - see comment in ClientWorld::draw()
	gk::Vector3d cameraPos(m_camera.getDPosition());
	m_camera.setDPosition(0, 0, 0);  // Temporarily move the camera to the origin

	// Move the centre of the skybox under the horizon
	glm::mat4 trans{1};
	trans[3].z = SKYBOX_OFFSET_Z;
	states.transform = trans * states.transform;

	if (m_sun.width() > 0.f && m_sun.height() > 0.f)
		target.draw(m_sun, states);

	if (m_moon.width() > 0.f && m_moon.height() > 0.f)
		target.draw(m_moon, states);

	if (Config::isStarRenderingEnabled && skyColor != starColor && m_world.sky()->starsDefinition().size > 0.f) {
		for (auto &it : m_stars)
			target.draw(it, states);
	}

	m_camera.setDPosition(cameraPos);  // Restore the camera to its original position
#endif // OM_NOT_IMPLEMENTED
}

