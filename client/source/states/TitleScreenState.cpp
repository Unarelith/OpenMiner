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
#include "Config.hpp"
#include "GameState.hpp"
#include "SettingsMenuState.hpp"
#include "TitleScreenState.hpp"

TitleScreenState::TitleScreenState() {
	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	m_menuWidget.addButton("Play", [this] (TextButton &) {
		m_stateStack->push<GameState>();
	});

	m_menuWidget.addButton("Options...", [this] (TextButton &) {
		m_stateStack->push<SettingsMenuState>();
	});

	m_menuWidget.addButton("Exit", [this] (TextButton &) {
		m_stateStack->pop();
	});
}

void TitleScreenState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);
}

void TitleScreenState::update() {
}

void TitleScreenState::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	prepareDraw(target, states);

	target.draw(m_menuWidget, states);
}

