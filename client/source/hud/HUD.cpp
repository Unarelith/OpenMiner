/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/GameClock.hpp>

#include "ClientPlayer.hpp"
#include "Config.hpp"
#include "HUD.hpp"

HUD::HUD(ClientPlayer &player, ClientWorld &world, ClientCommandHandler &client)
	: m_client(client),
	m_hotbar(player.inventory()),
	m_blockCursor(player, world, client),
	m_debugOverlay(player, world)
{
	setScale(GUI_SCALE, GUI_SCALE, 1);

	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	setup();
}

void HUD::setup() {
	m_orthoMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	m_hotbar.setPosition(SCREEN_WIDTH / getScale().x / 2 - m_hotbar.width() / 2, SCREEN_HEIGHT / getScale().y - m_hotbar.height(), 0);

	m_blockInfoWidget.setPosition(SCREEN_WIDTH / getScale().x / 2 - m_blockInfoWidget.width() / 2, 2, 0);

	m_fpsText.setPosition(SCREEN_WIDTH / getScale().x - 36, 2);

	m_crosshair.setup();
}

void HUD::onEvent(const SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3)
		m_isDebugOverlayVisible ^= 1;

	m_hotbar.onEvent(event);
	m_blockCursor.onEvent(event, m_hotbar);
}

void HUD::update() {
	// FIXME: Shouldn't be called every tick
	m_hotbar.update();

	m_fpsText.setText(std::to_string(gk::GameClock::getFpsAverage()) + " FPS");

	m_blockCursor.update(m_hotbar, false);

	if (m_isDebugOverlayVisible)
		m_debugOverlay.update();

	m_blockInfoWidget.update();

	if (m_blockCursor.currentBlock() != m_blockInfoWidget.currentBlock())
		m_blockInfoWidget.setCurrentBlock(m_blockCursor.currentBlock());
}

void HUD::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	target.draw(m_blockCursor, states);

	target.disableView();

	states.shader = &m_shader;
	states.projectionMatrix = m_orthoMatrix;
	states.viewMatrix = gk::Transform::Identity;
	states.vertexAttributes = gk::VertexAttribute::Only2d;

	states.transform *= getTransform();

	if (m_isDebugOverlayVisible)
		target.draw(m_debugOverlay, states);

	target.draw(m_blockInfoWidget, states);
	target.draw(m_hotbar, states);
	target.draw(m_fpsText, states);

	states.transform = gk::Transform::Identity;

	target.draw(m_crosshair, states);
}

