/*
 * =====================================================================================
 *
 *       Filename:  GameState.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 03:51:55
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/input/GamePad.hpp>
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/Exception.hpp>
#include <gk/core/Mouse.hpp>
#include <gk/gl/OpenGL.hpp>

#include "GameKey.hpp"
#include "GameState.hpp"
#include "InventoryState.hpp"
#include "LuaGUIState.hpp"
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"
#include "Registry.hpp"

GameState::GameState(Client &client, const std::string &host, int port) : m_client(client) {
	m_camera.setAspectRatio((float)SCREEN_WIDTH / SCREEN_HEIGHT);

	initShaders();

	m_client.connect(host, port);

	m_client.setCommandCallback(Network::Command::RegistryData, [this](sf::Packet &packet) {
		Registry::getInstance().deserialize(packet);
		m_isRegistryInitialized = true;
	});

	m_client.setCommandCallback(Network::Command::ChunkData, [this](sf::Packet &packet) {
		m_world.receiveChunkData(packet);
	});

	m_client.setCommandCallback(Network::Command::BlockUpdate, [this](sf::Packet &packet) {
		s32 x, y, z;
		u32 block;
		packet >> x >> y >> z >> block;
		m_world.setBlock(x, y, z, block);
		m_world.setData(x, y, z, block >> 16);
	});

	m_client.setCommandCallback(Network::Command::PlayerInvUpdate, [this](sf::Packet &packet) {
		m_player.deserialize(packet);
	});

	m_client.setCommandCallback(Network::Command::BlockGUIData, [this](sf::Packet &packet) {
		m_stateStack->push<LuaGUIState>(m_client, m_player, m_world, packet, this);
	});

	m_client.setCommandCallback(Network::Command::BlockInvUpdate, [this](sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = m_world.getBlockData(pos.x, pos.y, pos.z);
		if (data) {
			packet >> data->inventory;
		}
	});

	m_client.setCommandCallback(Network::Command::BlockDataUpdate, [this](sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = m_world.getBlockData(pos.x, pos.y, pos.z);
		if (data) {
			packet >> data->data;
		}
	});

	// sf::Packet packet;
	// packet << Network::Command::ClientSettings;
	// packet << Config::renderDistance;
	// m_client.send(packet);
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		if(SCREEN_WIDTH / 2 != event.motion.x || SCREEN_HEIGHT / 2 != event.motion.y) {
			m_player.turnH(event.motion.xrel * 0.06);
			m_player.turnV(-event.motion.yrel * 0.06);

			gk::Mouse::resetToWindowCenter();
		}
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && &m_stateStack->top() == this) {
		m_stateStack->push<PauseMenuState>(this);
	}
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
			m_stateStack->push<PauseMenuState>(this);

			gk::Mouse::setCursorGrabbed(false);
			gk::Mouse::setCursorVisible(true);
		}
		else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
			gk::Mouse::setCursorGrabbed(true);
			gk::Mouse::setCursorVisible(false);
		}
	}

	if (m_isRegistryInitialized)
		m_hud.onEvent(event);
}

void GameState::update() {
	m_world.update();

	if (m_isRegistryInitialized) {
		if (&m_stateStack->top() == this) {
			m_player.processInputs();

			if (gk::GamePad::isKeyPressedOnce(GameKey::Inventory)) {
				auto &inventoryState = m_stateStack->push<InventoryState>(this);
				inventoryState.setupWidget<PlayerInventoryWidget>(m_client, m_player.inventory());
			}
		}

		m_player.updatePosition(m_world);
		m_skybox.update(m_player);

		m_hud.update();
	}

	m_client.update();
}

void GameState::initShaders() {
	m_shader.createProgram();

	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/color.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/game.f.glsl");

	m_shader.linkProgram();
}

void GameState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	gk::Shader::bind(&m_shader);
	m_shader.setUniform("u_time", gk::GameClock::getTicks());
	gk::Shader::bind(nullptr);

	states.shader = &m_shader;

	target.setView(m_camera);
	// target.draw(m_skybox, states);
	target.draw(m_world, states);

	if (m_isRegistryInitialized)
		target.draw(m_hud, states);
}

