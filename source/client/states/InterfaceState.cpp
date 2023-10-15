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
#include <glm/gtc/matrix_transform.hpp>

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "EngineConfig.hpp"
#include "InterfaceState.hpp"

InterfaceState::InterfaceState(ApplicationState *parent) : DrawableState(parent) {
	m_shader.loadFromFile("basic");

	m_background.setFillColor(gk::Color::fromRGBA32(0, 0, 0, 127));

	setup();
}

void InterfaceState::setup() {
	m_view.setSize((float)Config::screenWidth, (float)Config::screenHeight);
	m_view.setCenter((float)Config::screenWidth / 2.f, (float)Config::screenHeight / 2.f);

	m_background.setSize(Config::screenWidth, Config::screenHeight);
}

void InterfaceState::onEvent(const SDL_Event &event) {
	if (m_parent) {
		m_parent->onEvent(event);
	}
	else if (event.type == SDL_QUIT) {
		m_stateStack->clear();
	}

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (!m_parent) {
			Config::screenWidth = (u16)event.window.data1;
			Config::screenHeight = (u16)event.window.data2;
		}

		setup();

		updateWidgetPosition();
	}
}

void InterfaceState::update() {
	if (m_parent)
		m_parent->update();
}

void InterfaceState::prepareDraw(RenderTarget &target, RenderStates &states) const {
	states.transform *= getTransform();
	states.shader = &m_shader;

	target.setView(m_view);

	if (m_parent && m_drawBackground)
		target.draw(m_background, states);
}

