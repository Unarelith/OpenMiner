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
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "DebugLightmapViewer.hpp"

DebugLightmapViewer::DebugLightmapViewer(const ClientPlayer &player) : m_player(player) {
	m_playerRect.setSize(7, 7);
	m_playerRect.setOutlineThickness(1);
	m_playerRect.setFillColor(gk::Color::White);
	m_playerRect.setOutlineColor(gk::Color::Black);
}

void DebugLightmapViewer::update(const ClientWorld &world) {
	gk::Vector3i playerChunkPos = m_player.getCurrentChunk();
	if (!m_playerChunkPos.has_value() || m_playerChunkPos.value() != playerChunkPos) {
		m_playerChunkPos = playerChunkPos;

		Chunk *chunk = world.getChunk(playerChunkPos.x, playerChunkPos.y, playerChunkPos.z);
		if (chunk) {
			for (u32 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				for (u32 y = 0 ; y < CHUNK_DEPTH ; ++y) {
					for (u32 z = 0 ; z < CHUNK_HEIGHT ; ++z) {
						Text &text = m_chunkLightmapValues[x][y][z];
						text.setString(std::to_string(chunk->lightmap().getSunlight(x, y, z)));
						text.setPosition(x * 8, (CHUNK_DEPTH - y - 1) * 8);
						text.setScale(0.5f, 0.5f);
					}
				}
			}
		}
	}

	s32 rx = gk::pmod(std::floor(m_player.x()), CHUNK_WIDTH);
	s32 ry = gk::pmod(std::floor(m_player.y()), CHUNK_DEPTH);
	m_playerRect.setPosition(rx * 8, (CHUNK_DEPTH - ry - 1) * 8);
}

void DebugLightmapViewer::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_playerRect, states);

	for (u32 x = 0 ; x < CHUNK_WIDTH ; ++x)
		for (u32 y = 0 ; y < CHUNK_DEPTH ; ++y)
			target.draw(m_chunkLightmapValues[x][y][gk::pmod(m_player.z(), CHUNK_HEIGHT)], states);
}

