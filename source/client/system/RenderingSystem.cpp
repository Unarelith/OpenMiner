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
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameTime.hpp"
#include "RenderingSystem.hpp"
#include "Skybox.hpp"

void RenderingSystem::initShaders() {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/game.f.glsl");
	m_shader.bindAttributeLocation(3, "normal");
	m_shader.bindAttributeLocation(4, "lightValue");
	m_shader.bindAttributeLocation(5, "ambientOcclusion");
	m_shader.linkProgram();

	m_fbo.loadShader("screen");
}

void RenderingSystem::onEvent(const SDL_Event &event) {
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		Config::screenWidth = (u16)event.window.data1;
		Config::screenHeight = (u16)event.window.data2;

		m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
		m_hud.setup();

		m_fbo.init(Config::screenWidth, Config::screenHeight);
	}
}

void RenderingSystem::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	setSkyColor();

	m_fbo.begin();

	states.shader = &m_shader;

	target.setView(m_camera);

	target.draw(m_skybox, states);
	target.draw(m_world, states);

	for (auto &it : m_playerBoxes)
		if (it.second.dimension() == m_player.dimension())
			target.draw(it.second, states);

	target.draw(m_hud.blockCursor(), states);

	m_fbo.end();

	target.draw(m_hud, states);
}

void RenderingSystem::setSkyColor() const {
	gk::Shader::bind(&m_shader);

	if (m_world.sky()) {
		if (m_world.sky()->daylightCycleSpeed() > 0.f) {
			float time = GameTime::getCurrentTime(0, m_world.sky()->daylightCycleSpeed());
			const gk::Color &color = GameTime::getSkyColorFromTime(*m_world.sky(), time);
			glClearColor(color.r, color.g, color.b, color.a);

			m_shader.setUniform("u_skyColor", color);
			m_shader.setUniform("u_sunlightIntensity", GameTime::getSunlightIntensityFromTime(time));
		}
		else {
			const gk::Color &color = m_world.sky()->color();
			glClearColor(color.r, color.g, color.b, color.a);

			m_shader.setUniform("u_skyColor", m_world.sky()->color());
			m_shader.setUniform("u_sunlightIntensity", 1.f);
		}
	}
	else {
		gk::Color color{50, 153, 204};
		glClearColor(color.r, color.g, color.b, color.a);

		m_shader.setUniform("u_skyColor", color);
		m_shader.setUniform("u_sunlightIntensity", 1.f);
	}

	gk::Shader::bind(nullptr);
}

