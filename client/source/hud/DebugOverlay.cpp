/*
 * =====================================================================================
 *
 *       Filename:  DebugOverlay.cpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 13:54:25
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <sstream>

#include "ClientPlayer.hpp"
#include "Config.hpp"
#include "DebugOverlay.hpp"

DebugOverlay::DebugOverlay(const ClientPlayer &player) : m_player(player) {
	setPosition(4, 4, 0);

	m_versionText.setText(APP_NAME + std::string(" v0.0.1"));
	m_versionText.setColor(gk::Color::White);

	m_positionText.setPosition(0, 10, 0);
	m_positionText.setColor(gk::Color::White);
}

void DebugOverlay::update() {
	std::stringstream stream;
	stream << "x: " << floorf(m_player.x()) << " | ";
	stream << "y: " << floorf(m_player.y()) << " | ";
	stream << "z: " << floorf(m_player.z());

	m_positionText.setText(stream.str());
}

void DebugOverlay::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_versionText, states);
	target.draw(m_positionText, states);
}

