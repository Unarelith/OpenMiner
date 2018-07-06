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
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "GameClock.hpp"
#include "GameState.hpp"
#include "InventoryState.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"

GameState::GameState() {
	m_player.hotbarInventory().addStack(ItemType::Workbench, 1);
	m_player.hotbarInventory().addStack(ItemType::Dirt, 64);
	m_player.hotbarInventory().addStack(ItemType::Grass, 64);
	m_player.hotbarInventory().addStack(ItemType::Stone, 64);
	m_player.hotbarInventory().addStack(ItemType::Glass, 64);
	m_player.hotbarInventory().addStack(ItemType::Glowstone, 64);
	m_player.hotbarInventory().addStack(ItemType::Furnace, 1);
	m_player.hotbarInventory().addStack(ItemType::PlankSlab, 64);

	m_player.inventory().addStack(ItemType::Wood, 64);
	m_player.inventory().addStack(ItemType::Planks, 64);
	m_player.inventory().addStack(ItemType::Stick, 64);
	m_player.inventory().addStack(ItemType::Cobblestone, 64);
	m_player.inventory().addStack(ItemType::StoneAxe, 1);
	m_player.inventory().addStack(ItemType::StoneHoe, 1);
	m_player.inventory().addStack(ItemType::StonePickaxe, 1);
	m_player.inventory().addStack(ItemType::StoneShovel, 1);
	m_player.inventory().addStack(ItemType::StoneSword, 1);
	m_player.inventory().addStack(ItemType::IronOre, 64);
	m_player.inventory().addStack(ItemType::Coal, 64);

	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);

	initShaders();
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		if(SCREEN_WIDTH / 2 != event.motion.x || SCREEN_HEIGHT / 2 != event.motion.y) {
			m_camera.turnH(event.motion.xrel * 0.06);
			m_camera.turnV(-event.motion.yrel * 0.06);

			Mouse::resetToWindowCenter();

			m_camera.update();
		}
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && &m_stateStack->top() == this) {
		m_stateStack->push<PauseMenuState>(this);
	}
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
			Mouse::setCursorGrabbed(false);
			Mouse::setCursorVisible(true);
		}
		else if (event.window.event == SDL_WINDOWEVENT_ENTER) {
			Mouse::setCursorGrabbed(true);
			Mouse::setCursorVisible(false);
		}
	}

	m_hud.onEvent(event);
}

void GameState::update() {
	Shader::bind(&m_shader);
	m_shader.setUniform("u_time", GameClock::getTicks());
	Shader::bind(nullptr);

	m_world.update(m_player);

	if (&m_stateStack->top() == this)
		m_viewMatrix = m_camera.processInputs(m_world);

	m_hud.update();

	if (Keyboard::isKeyPressedOnce(Keyboard::E) && &m_stateStack->top() == this) {
		auto &inventoryState = m_stateStack->push<InventoryState>(this);
		inventoryState.setupWidget<PlayerInventoryWidget>(m_player.inventory(), m_player.hotbarInventory());
	}
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

void GameState::draw(RenderTarget &target, RenderStates states) const {
	states.shader = &m_shader;
	states.projectionMatrix = &m_projectionMatrix;
	states.viewMatrix = &m_viewMatrix;

	target.draw(m_world, states);
	target.draw(m_hud, states);
}

