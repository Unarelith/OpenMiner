/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <sstream>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "DebugOverlay.hpp"

DebugOverlay::DebugOverlay(const ClientPlayer &player, const ClientWorld &world)
	: m_player(player), m_world(world)
{
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
	stream << '\n';
	stream << "Loaded chunks: " << m_world.loadedChunkCount();

	m_positionText.setText(stream.str());
}

void DebugOverlay::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_versionText, states);
	target.draw(m_positionText, states);
}

