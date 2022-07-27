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
#include "ClientEvents.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameTime.hpp"
#include "MessageBus.hpp"
#include "RenderingSystem.hpp"
#include "Skybox.hpp"

RenderingSystem::RenderingSystem(MessageBus &messageBus, gk::Camera &camera, ClientWorld &world, HUD &hud, Skybox &skybox)
	: m_messageBus(messageBus), m_camera(camera), m_world(world), m_hud(hud), m_skybox(skybox)
{
	m_messageBus.subscribe<RenderingEvent::DrawObjects, &RenderingSystem::onDrawObjects>(*this);
	m_messageBus.subscribe<RenderingEvent::WindowSizeChanged, &RenderingSystem::onWindowSizeChanged>(*this);
}

void RenderingSystem::init() {
	// Shaders
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/game.f.glsl");
	m_shader.bindAttributeLocation(3, "normal");
	m_shader.bindAttributeLocation(4, "lightValue");
	m_shader.bindAttributeLocation(5, "ambientOcclusion");
	m_shader.linkProgram();

	// Framebuffer
	m_fbo.loadShader("screen");

	// Camera
	m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
	m_camera.setFarClippingPlane(1000.0f);
}

void RenderingSystem::update() {
	if (m_camera.getFieldOfView() != Config::cameraFOV)
		m_camera.setFieldOfView(Config::cameraFOV);

	static const Sky *sky = nullptr;
	if (sky != m_world.sky() && m_world.sky()) {
		sky = m_world.sky();
		m_skybox.loadSky(*sky);
	}
}

void RenderingSystem::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	setSkyColor();

	m_fbo.begin();
	{
		states.shader = &m_shader;

		target.setView(m_camera);

		for (const gk::Drawable *drawable : m_events[0].objects)
			target.draw(*drawable, states);
	}
	m_fbo.end();

	for (const gk::Drawable *drawable : m_events[1].objects) {
		target.draw(*drawable, states);
	}
}

void RenderingSystem::onDrawObjects(const RenderingEvent::DrawObjects &event) {
	if (event.inFramebuffer)
		m_events[0] = event;
	else
		m_events[1] = event;
}

void RenderingSystem::onWindowSizeChanged(const RenderingEvent::WindowSizeChanged &event) {
	m_camera.setAspectRatio((float)event.width / event.height);
	m_hud.setup();

	m_fbo.init(event.width, event.height);
}

void RenderingSystem::setSkyColor() const {
	gk::Color color = gk::Color::fromRGBA32(50, 153, 204);
	float sunlightIntensity = 1.f;

	if (m_world.sky()) {
		if (m_world.sky()->daylightCycleSpeed() > 0.f) {
			float time = GameTime::getCurrentTime(0, m_world.sky()->daylightCycleSpeed());
			color = GameTime::getSkyColorFromTime(*m_world.sky(), time);
			sunlightIntensity = GameTime::getSunlightIntensityFromTime(time);
		}
		else {
			color = m_world.sky()->color();
		}
	}

	glClearColor(color.r, color.g, color.b, color.a);

	gk::Shader::bind(&m_shader);

	m_shader.setUniform("u_skyColor", color);
	m_shader.setUniform("u_sunlightIntensity", sunlightIntensity);

	gk::Shader::bind(nullptr);
}

