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

GameState::GameState() {
	m_hotbarInventory.addStack(ItemType::Dirt, 64);
	m_hotbarInventory.addStack(ItemType::Grass, 64);
	m_hotbarInventory.addStack(ItemType::Stone, 64);
	m_hotbarInventory.addStack(ItemType::Glass, 64);
	m_hotbarInventory.addStack(ItemType::Glowstone, 64);

	m_playerInventory.addStack(ItemType::Wood, 64);
	m_playerInventory.addStack(ItemType::Planks, 64);
	m_playerInventory.addStack(ItemType::Stick, 64);
	m_playerInventory.addStack(ItemType::Cobblestone, 64);
	m_playerInventory.addStack(ItemType::StoneAxe, 1);
	m_playerInventory.addStack(ItemType::StoneHoe, 1);
	m_playerInventory.addStack(ItemType::StonePickaxe, 1);
	m_playerInventory.addStack(ItemType::StoneShovel, 1);
	m_playerInventory.addStack(ItemType::StoneSword, 1);

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

	m_hotbar.onEvent(event);
	m_blockCursor.onEvent(event, m_hotbarInventory, m_hotbar);
}

void GameState::update() {
	Shader::bind(&m_shader);
	m_shader.setUniform("u_time", GameClock::getTicks());
	Shader::bind(nullptr);

	m_world.updateChunks();

	if (&m_stateStack->top() == this)
		m_viewMatrix = m_camera.processInputs(m_world);

	// FIXME: Shouldn't be called every tick
	m_hotbar.update();

	m_blockCursor.update(m_playerInventory, false);

	if (Keyboard::isKeyPressedOnce(Keyboard::E) && &m_stateStack->top() == this) {
		m_stateStack->push<InventoryState>(m_playerInventory, m_hotbarInventory, this);
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
	target.draw(m_crosshair, states);
	target.draw(m_blockCursor, states);
	target.draw(m_hotbar, states);
}

