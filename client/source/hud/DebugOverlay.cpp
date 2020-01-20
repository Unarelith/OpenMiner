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
	s32 pcx = std::floor(m_player.x() / CHUNK_WIDTH);
	s32 pcy = std::floor(m_player.y() / CHUNK_HEIGHT);
	s32 pcz = std::floor(m_player.z() / CHUNK_DEPTH);

	std::stringstream stream;
	stream << "x: " << std::floor(m_player.x()) << " | ";
	stream << "y: " << std::floor(m_player.y()) << " | ";
	stream << "z: " << std::floor(m_player.z());
	stream << '\n';
	stream << "rx: " << int(std::floor(m_player.x()) + std::abs(pcx) * CHUNK_WIDTH) % CHUNK_WIDTH << " | ";
	stream << "ry: " << int(std::floor(m_player.y()) + std::abs(pcy) * CHUNK_HEIGHT) % CHUNK_HEIGHT  << " | ";
	stream << "rz: " << int(std::floor(m_player.z()) + std::abs(pcz) * CHUNK_DEPTH) % CHUNK_DEPTH;
	stream << '\n';
	stream << "cx: " << pcx << " | ";
	stream << "cy: " << pcy << " | ";
	stream << "cz: " << pcz;

	m_positionText.setText(stream.str());
}

void DebugOverlay::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_versionText, states);
	target.draw(m_positionText, states);
}

