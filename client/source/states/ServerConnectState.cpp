/*
 * =====================================================================================
 *
 *       Filename:  ServerConnectState.cpp
 *
 *    Description:
 *
 *        Created:  08/04/2019 18:52:06
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>

#include "GameState.hpp"
#include "ServerConnectState.hpp"

ServerConnectState::ServerConnectState() {
	m_textInput.setContent("localhost:4242");
	m_textInput.setCharacterLimit(15 + 1 + 6);
	m_textInput.setSize(400, 54);
	m_textInput.setPosition(SCREEN_WIDTH / 2 - m_textInput.getSize().x / 2, SCREEN_HEIGHT / 2 - m_textInput.getSize().y / 2);
	m_textInput.setCursor("_");

	m_connectButton.setText("Connect");
	m_connectButton.setPosition(SCREEN_WIDTH / 2 - m_connectButton.getGlobalBounds().width * GUI_SCALE / 2, SCREEN_HEIGHT - 291);
	m_connectButton.setScale(GUI_SCALE, GUI_SCALE, 1);
	m_connectButton.setCallback([this](TextButton &) {
		size_t sep = m_textInput.content().find_first_of(':');

		std::string host = m_textInput.content().substr(0, sep);

		int port = 0;
		try {
			port = std::stoi(m_textInput.content().substr(sep + 1));
		}
		catch (const std::invalid_argument &e) {
			std::cerr << "Error: Invalid server address." << std::endl;
		}

		m_stateStack->push<GameState>(host, port);
	});
}

void ServerConnectState::onEvent(const SDL_Event &event) {
	m_textInput.onEvent(event);

	m_connectButton.onEvent(event);
}

void ServerConnectState::update() {
}

void ServerConnectState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	prepareDraw(target, states);

	target.draw(m_textInput, states);
	target.draw(m_connectButton, states);
}

