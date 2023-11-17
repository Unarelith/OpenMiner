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

#include "common/core/GameClock.hpp"
#include "common/event/Events.hpp"

#include "client/core/Config.hpp"
#include "client/core/ClientProfiler.hpp"
#include "client/hud/HUD.hpp"
#include "client/network/ClientCommandHandler.hpp"
#include "client/world/ClientPlayer.hpp"

HUD::HUD(ClientPlayer &player, ClientWorld &world, ClientCommandHandler &client)
	: m_player(player), m_world(world),
	m_hotbar(player, client),
	m_blockCursor(player, world, client),
	m_debugOverlay(player, world),
	m_chat(client.client()),
	m_debugLightmapViewer(player)
{
	setScale(Config::guiScale, Config::guiScale, 1);

	m_shader.loadFromFile("basic");

	setup();
}

void HUD::setup() {
	m_view.setSize((float)Config::screenWidth, (float)Config::screenHeight);
	m_view.setCenter((float)Config::screenWidth / 2.f, (float)Config::screenHeight / 2.f);

	m_hotbar.setPosition(
		Config::screenWidth / getScale().x / 2.f - (float)m_hotbar.width() / 2.f,
		Config::screenHeight / getScale().y - (float)m_hotbar.height()
	);

	m_blockInfoWidget.setPosition(Config::screenWidth / getScale().x / 2.f - (float)m_blockInfoWidget.width() / 2.f, 2.f);

	m_fpsText.setPosition(Config::screenWidth / getScale().x - 36.f, 2.f);

	m_crosshair.setup();

	m_chat.setPosition(2.f, Config::screenHeight / Config::guiScale - 50.f);

	m_minimap.setPosition(float(Config::screenWidth / Config::guiScale - Minimap::minimapSize) - 15.f, 15.f);

	m_debugLightmapViewer.setPosition(0.f, float(Config::screenHeight / Config::guiScale - DebugLightmapViewer::totalSize));

	m_debugProfilerWindow.setPosition(2.f, 2.f);
}

void HUD::onEvent(const SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3) {
		m_isDebugOverlayVisible ^= 1;
		m_printRendererInfo = event.key.keysym.mod & KMOD_SHIFT;
	}

	if (Config::isHotbarVisible)
		m_hotbar.onEvent(event);

	m_blockCursor.onEvent(event, m_hotbar);

	if (Config::isChunkMinimapEnabled)
		m_minimap.onEvent(event);

	if (Config::isProfilerWindowEnabled)
		m_debugProfilerWindow.onEvent(event);
}

void HUD::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	setScale(event.guiScale, event.guiScale, 1);

	setup();
}

void HUD::update() {
	// FIXME: Shouldn't be called every tick
	if (Config::isHotbarVisible)
		m_hotbar.update();

	if (Config::isFpsCounterEnabled)
		m_fpsText.setString(std::to_string(GameClock::getInstance().getFpsAverage()) + " FPS");

	m_blockCursor.update(m_hotbar);

	if (m_isDebugOverlayVisible)
		m_debugOverlay.update(m_printRendererInfo);
	else if (Config::isProfilerWindowEnabled)
		m_debugProfilerWindow.update();

	if (Config::isBlockInfoWidgetEnabled) {
		m_blockInfoWidget.update();

		if (m_blockCursor.currentBlock() != m_blockInfoWidget.currentBlock())
			m_blockInfoWidget.setCurrentBlock(m_blockCursor.currentBlock());
	}

	if (Config::isChunkMinimapEnabled)
		m_minimap.update(m_player, m_world);

	if (Config::isLightmapViewerEnabled)
		m_debugLightmapViewer.update(m_world);
}

void HUD::draw(RenderTarget &target, RenderStates states) const {
	OM_PROFILE_START("HUD::draw");

	target.setView(m_view);

	states.shader = &m_shader;

	states.transform *= getTransform();

	if (m_isDebugOverlayVisible)
		target.draw(m_debugOverlay, states);
	else if (Config::isProfilerWindowEnabled)
		target.draw(m_debugProfilerWindow, states);

	if (Config::isBlockInfoWidgetEnabled)
		target.draw(m_blockInfoWidget, states);

	if (Config::isHotbarVisible)
		target.draw(m_hotbar, states);

	if (Config::isFpsCounterEnabled)
		target.draw(m_fpsText, states);

	if (Config::isChunkMinimapEnabled)
		target.draw(m_minimap, states);

	if (Config::isLightmapViewerEnabled)
		target.draw(m_debugLightmapViewer, states);

	target.draw(m_chat, states);

	states.transform = Transform::Identity;

	if (Config::isCrosshairVisible)
		target.draw(m_crosshair, states);

	OM_PROFILE_END("HUD::draw");
}
