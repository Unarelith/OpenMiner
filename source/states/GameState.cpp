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
#include "ScriptEngine.hpp"

GameState::GameState() {
	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);

	auto &lua = ScriptEngine::getInstance().lua();
	lua["player"] = &m_player;
	lua["init"]();

	initShaders();
}

void GameState::onEvent(const S_Event &event) {
#ifdef USE_SDL
	if (event.type == SDL_MOUSEMOTION) {
		if(SCREEN_WIDTH / 2 != event.motion.x || SCREEN_HEIGHT / 2 != event.motion.y) {
			m_camera.turnH(event.motion.xrel * 0.06);
			m_camera.turnV(-event.motion.yrel * 0.06);
#elif defined USE_SFML
    static sf::Vector2i lastPosition;
	if (event.type == sf::Event::MouseMoved) {
		if(SCREEN_WIDTH / 2 != event.mouseMove.x || SCREEN_HEIGHT / 2 != event.mouseMove.y) {
			m_camera.turnH((event.mouseMove.x-lastPosition.x) * 0.06);
			m_camera.turnV(-(event.mouseMove.y-lastPosition.y) * 0.06);
#endif // USE_SDL, USE_SFML

			Mouse::resetToWindowCenter();
        #ifdef USE_SFML
            lastPosition = Mouse::getPosition();
        #endif // USE_SFML

			m_camera.updateViewMatrix();
		}
	}
#ifdef USE_SDL
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && &m_stateStack->top() == this) {
#elif defined USE_SFML
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && &m_stateStack->top() == this) {
#endif // USE_SDL, USE_SFML
		m_stateStack->push<PauseMenuState>(this);
	}
#ifdef USE_SDL
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
#elif defined USE_SFML
		if (event.type == sf::Event::MouseLeft) {
#endif // USE_SDL, USE_SFML
			Mouse::setCursorGrabbed(false);
			Mouse::setCursorVisible(true);
		}
#ifdef USE_SDL
		else if (event.window.event == SDL_WINDOWEVENT_ENTER) {
#elif defined USE_SFML
		else if (event.type == sf::Event::MouseEntered) {
#endif // USE_SDL, USE_SFML
			Mouse::setCursorGrabbed(true);
			Mouse::setCursorVisible(false);
		}
#ifdef USE_SDL
	}
#endif // USE_SDL

	m_hud.onEvent(event);
}

void GameState::update() {
	Shader::bind(&m_shader);
	m_shader.setUniform("u_time", GameClock::getTicks());
	Shader::bind(nullptr);

	m_world.update(m_player);

	if (&m_stateStack->top() == this) {
		m_camera.processInputs();

		if (Keyboard::isKeyPressedOnce(Keyboard::E)) {
			auto &inventoryState = m_stateStack->push<InventoryState>(this);
			inventoryState.setupWidget<PlayerInventoryWidget>(m_player.inventory());
		}
	}

	m_viewMatrix = m_camera.updatePosition(m_world);

	m_hud.update();
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
	states.projectionMatrix = m_projectionMatrix;
	states.viewMatrix = m_viewMatrix;

	target.draw(m_world, states);
	target.draw(m_hud, states);
}

