/*
 * =====================================================================================
 *
 *       Filename:  TitleScreenState.cpp
 *
 *    Description:
 *
 *        Created:  08/04/2019 18:00:59
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>

#include "Config.hpp"
#include "ServerConnectState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"

TitleScreenState::TitleScreenState() {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	m_menuWidget.addButton("Play", [this] (TextButton &) {
		m_stateStack->push<ServerConnectState>();
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>();
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		m_stateStack->pop();
	});
}

void TitleScreenState::onEvent(const SDL_Event &event) {
	InterfaceState::onEvent(event);

	m_menuWidget.onEvent(event);
}

void TitleScreenState::update() {
}

void TitleScreenState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	prepareDraw(target, states);

	target.draw(m_menuWidget, states);
}

