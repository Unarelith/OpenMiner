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
	s32 px = std::floor(m_player.x());
	s32 py = std::floor(m_player.y());
	s32 pz = std::floor(m_player.z());

	const char *directions[5] = {"West", "East", "South", "North"};
	const char *direction = directions[m_player.getDirection()];

	std::stringstream stream;
	stream << "x: " << px << " | ";
	stream << "y: " << py << " | ";
	stream << "z: " << pz;
	stream << '\n';
	stream << "rx: " << px % CHUNK_WIDTH << " | ";
	stream << "ry: " << py % CHUNK_DEPTH << " | ";
	stream << "rz: " << pz % CHUNK_HEIGHT;
	stream << '\n';
	stream << "cx: " << (px & -CHUNK_WIDTH)  / CHUNK_WIDTH << " | ";
	stream << "cy: " << (py & -CHUNK_DEPTH)  / CHUNK_DEPTH << " | ";
	stream << "cz: " << (pz & -CHUNK_HEIGHT) / CHUNK_HEIGHT;
	stream << '\n';
	stream << "dir: " << direction << " (" << m_player.cameraYaw() << ")";
	stream << '\n';
	stream << "Loaded chunks: " << m_world.loadedChunkCount();

	m_positionText.setText(stream.str());
}

void DebugOverlay::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_versionText, states);
	target.draw(m_positionText, states);
}

