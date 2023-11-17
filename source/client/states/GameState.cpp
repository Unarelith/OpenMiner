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

#include "common/core/ApplicationStateStack.hpp"
#include "common/core/Exception.hpp"
#include "common/core/GameClock.hpp"
#include "common/core/GameTime.hpp"
#include "common/core/Registry.hpp"
#include "common/core/input/GameKey.hpp"
#include "common/event/Events.hpp"
#include "common/resource/ResourceHandler.hpp"

#include "client/core/KeyboardHandler.hpp"
#include "client/core/Mouse.hpp"
#include "client/core/input/GamePad.hpp"
#include "client/graphics/BgfxView.hpp"
#include "client/graphics/RenderTarget.hpp"
#include "client/graphics/TextureAtlas.hpp"
#include "client/states/ChatState.hpp"
#include "client/states/GameState.hpp"
#include "client/states/LuaGUIState.hpp"
#include "client/states/PauseMenuState.hpp"

GameState::GameState()
	: m_textureAtlas(ResourceHandler::getInstance().get<TextureAtlas>("atlas-blocks"))
{
	Registry::setInstance(m_registry);

	initShaders();

	m_clientCommandHandler.setupCallbacks();

	m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
	m_camera.setFarClippingPlane(1000.0f);

	m_world.setClient(m_clientCommandHandler);
	m_world.setCamera(m_player.camera());

	m_keyboardHandler = dynamic_cast<KeyboardHandler *>(GamePad::getInputHandler());

	m_skyColor.init("u_skyColor", bgfx::UniformType::Vec4);
	m_sunlightIntensity.init("u_sunlightIntensity", bgfx::UniformType::Vec4);

	m_fbo.init(Config::screenWidth, Config::screenHeight);
}

void GameState::init() {
	m_world.setEventHandler(*m_eventHandler);

	m_eventHandler->addListener<GuiScaleChangedEvent>(&GameState::onGuiScaleChanged, this);

	m_eventHandler->addListener<ChunkCreatedEvent>(&Minimap::onChunkCreatedEvent, &m_hud.minimap());
	m_eventHandler->addListener<ChunkRemovedEvent>(&Minimap::onChunkRemovedEvent, &m_hud.minimap());
}

void GameState::onStateInactive() {
	m_hud.pause();
}

void GameState::connect(const std::string &host, u16 port, const std::string &username) {
	m_player.setName(username.empty() ? "Player" : username);
	m_client.connect(host, port, m_player);
	m_player.setClientID(m_client.id());
	m_client.addPlayer(m_player);

	Mouse::setCursorVisible(false);
	Mouse::setCursorGrabbed(true);
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_QUIT) {
		m_client.disconnect();

		m_stateStack->clear();
	}

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		KeyboardHandler *keyboardHandler = (KeyboardHandler *)GamePad::getInputHandler();

		if (event.type == SDL_MOUSEMOTION) {
			if(Config::screenWidth / 2 != event.motion.x || Config::screenHeight / 2 != event.motion.y) {
				m_player.turnH((float)event.motion.xrel * -0.01f * Config::mouseSensitivity);
				m_player.turnViewV((float)event.motion.yrel * -0.01f * Config::mouseSensitivity);

				Mouse::resetToWindowCenter();
			}
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			m_stateStack->push<PauseMenuState>(m_client, this);
		}
		else if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
				m_stateStack->push<PauseMenuState>(m_client, this);

				Mouse::setCursorGrabbed(false);
				Mouse::setCursorVisible(true);
			}
			else if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
				Mouse::setCursorGrabbed(true);
				Mouse::setCursorVisible(false);
			}
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Chat)
			 || event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command)) {
				m_stateStack->push<ChatState>(m_clientCommandHandler, m_hud.chat(), event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::Command), this);
			}
			else if (event.key.keysym.sym == keyboardHandler->getKeycode(GameKey::BlockInfoToggle)) {
				Config::isBlockInfoWidgetEnabled = !Config::isBlockInfoWidgetEnabled;
			}
			else if (event.key.keysym.sym == SDLK_F2) {
				std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

				char filename[100];
				std::strftime(filename, sizeof(filename), "Screenshot-%Y-%m-%d-%H-%M-%S", std::localtime(&now));

				Window::saveScreenshot(filename);
				m_hud.chat().addChatMessage(0, "Screenshot saved: " + std::string(filename) + ".png");
			}

			for (auto &key : Registry::getInstance().keys()) {
				if (event.key.keysym.sym == key.keycode()) {
					m_clientCommandHandler.sendKeyPressed(key.id());
				}
			}
		}

		m_hud.onEvent(event);
	}

	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		if (event.window.data1 != 0 && event.window.data2 != 0) {
			Config::screenWidth = (u16)event.window.data1;
			Config::screenHeight = (u16)event.window.data2;

			m_camera.setAspectRatio((float)Config::screenWidth / Config::screenHeight);
			m_hud.setup();

			m_fbo.init(Config::screenWidth, Config::screenHeight);
		}
	}
}

void GameState::update() {
	m_world.checkPlayerChunk(m_player.x(), m_player.y(), m_player.z());
	m_world.update(!m_stateStack->empty() && (&m_stateStack->top() == this || m_stateStack->top().parent() == this));

	if (m_camera.getFieldOfView() != Config::cameraFOV)
		m_camera.setFieldOfView(Config::cameraFOV);

	if (!m_stateStack->empty() && &m_stateStack->top() == this) {
		m_player.processInputs();
	}

	if (!m_areModKeysLoaded) {
		for (auto &it : Registry::getInstance().keys()) {
			m_keyboardHandler->addKey(it.id(), it.name(), it.keycode(), it.stringID(), &it);
		}

		m_areModKeysLoaded = true;
	}

	m_player.updatePosition(m_world);

	m_hud.update();

	if (GameClock::getInstance().getTicks() % 100 < 10) {
		m_clientCommandHandler.sendPlayerPosUpdate();
		m_clientCommandHandler.sendPlayerRotUpdate();
	}

	m_client.update();

	static const Sky *sky = nullptr;
	if (sky != m_world.sky() && m_world.sky()) {
		sky = m_world.sky();
		m_skybox.loadSky(*sky);
	}
}

void GameState::initShaders() {
	m_shader.loadFromFile("game");

	m_fbo.loadShader("screen");
}

void GameState::onGuiScaleChanged(const GuiScaleChangedEvent &event) {
	m_hud.onGuiScaleChanged(event);
}

void GameState::draw(RenderTarget &target, RenderStates states) const {
	if (m_world.sky()) {
		Color color = m_world.sky()->color();
		float sunlightIntensity = 1.f;

		if (m_world.sky()->daylightCycleSpeed() > 0.f) {
			float time = GameTime::getCurrentTime(0, m_world.sky()->daylightCycleSpeed());
			color = GameTime::getSkyColorFromTime(*m_world.sky(), time);
			sunlightIntensity = GameTime::getSunlightIntensityFromTime(time);
		}

		u32 iColor = (color.r255() << 24)
		           | (color.g255() << 16)
		           | (color.b255() << 8)
		           | color.a255();

		bgfx::setViewClear(BgfxView::Sky, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, iColor);

		m_skyColor.setValue(color, true);

		m_sunlightIntensity.setValue(sunlightIntensity);
	}

	states.shader = &m_shader;

	target.setView(m_camera);

	m_fbo.prepareDraw();

	states.view = BgfxView::Sky;
	{
		target.draw(m_skybox, states);
	}

	states.view = BgfxView::World;
	{
		target.draw(m_world, states);

		for (auto &it : m_playerBoxes)
			if (it.second.dimension() == m_player.dimension())
				target.draw(it.second, states);

		target.draw(m_hud.blockCursor(), states);
	}

	states.view = BgfxView::HUD;
	{
		target.draw(m_hud, states);
	}

	m_fbo.draw();
}
