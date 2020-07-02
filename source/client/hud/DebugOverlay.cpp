/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <sstream>

#include <gk/math/Math.hpp>

#include "ClientPlayer.hpp"
#include "ClientScene.hpp"
#include "ClientWorld.hpp"
#include "Config.hpp"
#include "DebugOverlay.hpp"

DebugOverlay::DebugOverlay(const ClientPlayer &player, const ClientWorld &world)
	: m_player(player), m_world(world)
{
	setPosition(4, 4, 0);

	std::string versionString = APP_NAME;
	versionString.append(" ");
	versionString.append(1, '0' + VERSION_MAJOR);
	versionString.append(".");
	versionString.append(1, '0' + VERSION_MINOR);
	versionString.append(".");
	versionString.append(1, '0' + VERSION_PATCH);
	m_versionText.setString(versionString);
	m_versionText.setColor(gk::Color::White);

	m_positionText.setPosition(0, 10, 0);
	m_positionText.setColor(gk::Color::White);
}

void DebugOverlay::update() {
	s32 px = std::floor(m_player.x());
	s32 py = std::floor(m_player.y());
	s32 pz = std::floor(m_player.z());

	// Directions is now an angle4
	const char *directions[4] = {"East", "North", "West", "South"};
	const char *direction = directions[m_player.getDirection()];

	std::stringstream stream;
	stream << "x: " << px << " | ";
	stream << "y: " << py << " | ";
	stream << "z: " << pz;
	stream << '\n';
	stream << "rx: " << gk::pmod(px, CHUNK_WIDTH) << " | ";
	stream << "ry: " << gk::pmod(py, CHUNK_DEPTH) << " | ";
	stream << "rz: " << gk::pmod(pz, CHUNK_HEIGHT);
	stream << '\n';
	stream << "cx: " << (px & -CHUNK_WIDTH)  / CHUNK_WIDTH << " | ";
	stream << "cy: " << (py & -CHUNK_DEPTH)  / CHUNK_DEPTH << " | ";
	stream << "cz: " << (pz & -CHUNK_HEIGHT) / CHUNK_HEIGHT;
	stream << '\n';
	stream << "dir: " << direction << " (" << m_player.cameraYaw() << ")";
	stream << '\n';
	stream << "dimension: " << m_player.dimension();
	stream << '\n';
	stream << "Loaded chunks: " << m_world.loadedChunkCount();
	stream << '\n';
	stream << "Alive entities: " << m_world.scene().registry().alive();

	m_positionText.setString(stream.str());
}

void DebugOverlay::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_versionText, states);
	target.draw(m_positionText, states);
}

