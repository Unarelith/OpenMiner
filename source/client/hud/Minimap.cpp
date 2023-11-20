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
#include "client/hud/Minimap.hpp"
#include "client/world/ClientPlayer.hpp"
#include "client/world/ClientWorld.hpp"

Minimap::Minimap() {
	m_vbo.setupDefaultLayout();

	m_border.setFillColor(Color::Transparent);
	m_border.setOutlineColor(Color::fromRGBA32(224, 224, 224));
	m_border.setOutlineThickness(1);
	m_border.setSize(minimapSize, minimapSize);

	m_playerChunk.setSize(chunkSize, chunkSize);
	m_playerChunk.setFillColor(Color::Red);

	updatePlayerFovVertexBuffer();
}

void Minimap::update(const ClientPlayer &player, class ClientWorld &world) {
	m_playerChunkPos = player.getCurrentChunk();

	m_playerChunk.setPosition(
		float(m_playerChunkPos.x * (chunkSize + 2)),
		float(-m_playerChunkPos.y * (chunkSize + 2))
	);

	for (auto &it : m_chunks) {
		if (it.first.z == m_playerChunkPos.z) {
			ClientChunk *chunk = (ClientChunk *)world.getChunk(it.first.x, it.first.y, it.first.z);
			if (chunk) {
				if (chunk->hasBeenDrawn()) {
					it.second.first.setFillColor(Color::Green);
				}
				else if (chunk->isInitialized()) {
					it.second.first.setFillColor(Color::fromRGBA32(224, 224, 224));
				}
				else {
					it.second.first.setFillColor(Color::fromRGBA32(127, 127, 127));
				}

				it.second.second.setString(std::to_string(chunk->debugTimesReceived));
			}
			else {
				it.second.first.setFillColor(Color::Blue);
			}
		}
	}

	m_playerFovRotationTransform = Transform::Identity;
	m_playerFovRotationTransform.rotateZ(90.f - player.cameraYaw());

	static float oldCameraFov = Config::cameraFOV;
	static u16 oldRenderDistance = Config::renderDistance;

	if (oldCameraFov != Config::cameraFOV || oldRenderDistance != Config::renderDistance) {
		updatePlayerFovVertexBuffer();

		oldCameraFov = Config::cameraFOV;
		oldRenderDistance = Config::renderDistance;
	}
}

void Minimap::onEvent(const SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) {
		m_displayTimesReceived ^= 1;
	}
}

void Minimap::onChunkCreatedEvent(const ChunkCreatedEvent &event) {
	if (Config::isChunkMinimapEnabled) {
		auto &[rect, text] = m_chunks[event.chunkPos];
		rect.setSize(chunkSize, chunkSize);
		rect.setPosition(float(event.chunkPos.x * (chunkSize + 2)), float(-event.chunkPos.y * (chunkSize + 2)));
		rect.setFillColor(event.isLoaded ? Color::fromRGBA32(224, 224, 224) : Color::fromRGBA32(127, 127, 127));
		rect.setOutlineThickness(1);
		rect.setOutlineColor(Color::Transparent);

		text.setBackgroundSize(chunkSize, chunkSize);
		text.setScale(0.5f, 0.5f);
		text.setShadowEnabled(true);
		text.setPosition(rect.getPosition());
		text.setString("0");
	}
}

void Minimap::onChunkRemovedEvent(const ChunkRemovedEvent &event) {
	if (Config::isChunkMinimapEnabled) {
		m_chunks.erase(event.chunkPos);
	}
}

void Minimap::updatePlayerFovVertexBuffer() {
	// FOV/render distance viewer
	Vertex vertices[3];
	vertices[0].coord3d[0] = 0.f;
	vertices[0].coord3d[1] = 0.f;

	vertices[1].coord3d[0] = -sinf(glm::radians(Config::cameraFOV / 2.f)) * Config::renderDistance * (chunkSize + 2) / cosf(glm::radians(Config::cameraFOV / 2.f));
	vertices[1].coord3d[1] = -(Config::renderDistance * (chunkSize + 2.f));

	vertices[2].coord3d[0] = sinf(glm::radians(Config::cameraFOV / 2.f)) * Config::renderDistance * (chunkSize + 2) / cosf(glm::radians(Config::cameraFOV / 2.f));
	vertices[2].coord3d[1] = -(Config::renderDistance * (chunkSize + 2.f));

	Color color = Color::Blue;
	for (u8 i = 0 ; i < 3 ; ++i) {
		vertices[i].color[0] = color.r;
		vertices[i].color[1] = color.g;
		vertices[i].color[2] = color.b;
		vertices[i].color[3] = color.a;
	}

	m_vbo.init(vertices, sizeof(vertices), true);
}

void Minimap::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_border, states);

	{
		RenderStates states2 = states;
		states2.transform.translate(minimapSize / 2 + chunkSize / 2, minimapSize / 2 + chunkSize / 2);
		states2.transform *= m_playerFovRotationTransform;

		target.draw(m_vbo, 0, 3, states2);
	}

	states.transform.translate(
		float(-m_playerChunkPos.x * (chunkSize + 2)) + minimapSize / 2.f,
		float(m_playerChunkPos.y * (chunkSize + 2)) + minimapSize / 2.f
	);

	for (auto &it : m_chunks)
		if (it.first.z == m_playerChunkPos.z) {
			if (!m_displayTimesReceived) {
				target.draw(it.second.first, states);
			}
			else {
				target.draw(it.second.second, states);
			}
		}

	target.draw(m_playerChunk, states);
}
