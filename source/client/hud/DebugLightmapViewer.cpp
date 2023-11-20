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
#include "client/hud/DebugLightmapViewer.hpp"
#include "client/world/ClientPlayer.hpp"
#include "client/world/ClientWorld.hpp"

DebugLightmapViewer::DebugLightmapViewer(const ClientPlayer &player) : m_player(player) {
	m_playerRect.setSize(7, 7);
	m_playerRect.setOutlineThickness(1);
	m_playerRect.setFillColor(Color::White);
	m_playerRect.setOutlineColor(Color::Black);

	for (u32 x = 0 ; x < CHUNK_WIDTH ; ++x) {
		for (u32 y = 0 ; y < CHUNK_DEPTH ; ++y) {
			Text &text = m_chunkLightmapValues[y][x];
			text.setPosition((float)x * 8.f, float(CHUNK_DEPTH - y - 1) * 8.f);
			text.setScale(0.5f, 0.5f);
		}
	}
}

void DebugLightmapViewer::update(const ClientWorld &world) {
	Vector3i playerChunkPos = m_player.getCurrentChunk();

	u32 rx = math::pmod((s32)std::floor(m_player.x()), CHUNK_WIDTH);
	u32 ry = math::pmod((s32)std::floor(m_player.y()), CHUNK_DEPTH);
	u32 rz = math::pmod((s32)std::floor(m_player.z()), CHUNK_HEIGHT);

	// If this is the first time or if player moved, update the texts
	if (!m_playerChunkPos.has_value()
	 || m_playerChunkPos.value() != playerChunkPos
	 || m_playerRelativeZ.value() != rz)
	{
		m_playerChunkPos = playerChunkPos;
		m_playerRelativeZ = rz;

		Chunk *chunk = world.getChunk(playerChunkPos.x, playerChunkPos.y, playerChunkPos.z);
		if (chunk) {
			for (u32 x = 0 ; x < CHUNK_WIDTH ; ++x) {
				for (u32 y = 0 ; y < CHUNK_DEPTH ; ++y) {
					Text &text = m_chunkLightmapValues[x][y];
					text.setString(std::to_string(chunk->lightmap().getSunlight(x, y, rz)));
				}
			}
		}
	}

	m_playerRect.setPosition((float)rx * 8.f, float(CHUNK_DEPTH - ry - 1) * 8.f);
}

void DebugLightmapViewer::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_playerRect, states);

	for (u32 x = 0 ; x < CHUNK_WIDTH ; ++x)
		for (u32 y = 0 ; y < CHUNK_DEPTH ; ++y)
			target.draw(m_chunkLightmapValues[x][y], states);
}
