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
#include <gk/gl/Vertex.hpp>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "Minimap.hpp"

Minimap::Minimap() {
	m_border.setFillColor(gk::Color::Transparent);
	m_border.setOutlineColor(gk::Color{224, 224, 224});
	m_border.setOutlineThickness(1);
	m_border.setSize(minimapSize, minimapSize);

	m_playerChunk.setSize(chunkSize, chunkSize);
	m_playerChunk.setFillColor(gk::Color::Red);

	updatePlayerFovVertexBuffer();
}

void Minimap::update(const ClientPlayer &player, class ClientWorld &world) {
	m_playerChunkPos = gk::Vector3i{
		((int)player.x() & -CHUNK_WIDTH)  / CHUNK_WIDTH,
		((int)player.y() & -CHUNK_DEPTH)  / CHUNK_DEPTH,
		((int)player.z() & -CHUNK_HEIGHT) / CHUNK_HEIGHT
	};

	m_playerChunk.setPosition(m_playerChunkPos.x * (chunkSize + 2), -m_playerChunkPos.y * (chunkSize + 2));

	for (auto &it : m_chunks) {
		if (it.first.z == m_playerChunkPos.z) {
			ClientChunk *chunk = (ClientChunk *)world.getChunk(it.first.x, it.first.y, it.first.z);
			if (chunk) {
				if (chunk->hasBeenDrawn()) {
					it.second.setFillColor(gk::Color::Green);
				}
				else if (chunk->isInitialized()) {
					it.second.setFillColor(gk::Color{224, 224, 224});
				}
				else {
					it.second.setFillColor(gk::Color{127, 127, 127});
				}
			}
			else {
				it.second.setFillColor(gk::Color::Blue);
			}
		}
	}

	m_playerFovRotationTransform = gk::Transform::Identity;
	m_playerFovRotationTransform.rotateZ(90.f - player.cameraYaw());

	static float oldCameraFov = Config::cameraFOV;
	static u16 oldRenderDistance = Config::renderDistance;

	if (oldCameraFov != Config::cameraFOV || oldRenderDistance != Config::renderDistance) {
		updatePlayerFovVertexBuffer();

		oldCameraFov = Config::cameraFOV;
		oldRenderDistance = Config::renderDistance;
	}
}

void Minimap::onChunkCreatedEvent(const ChunkCreatedEvent &event) {
	auto &rect = m_chunks[event.chunkPos];
	rect.setSize(chunkSize, chunkSize);
	rect.setPosition(event.chunkPos.x * (chunkSize + 2), -event.chunkPos.y * (chunkSize + 2));
	rect.setFillColor(event.isLoaded ? gk::Color{224, 224, 224} : gk::Color{127, 127, 127});
	rect.setOutlineThickness(1);
	rect.setOutlineColor(gk::Color::Transparent);
}

void Minimap::onChunkRemovedEvent(const ChunkRemovedEvent &event) {
	m_chunks.erase(event.chunkPos);
}

void Minimap::updatePlayerFovVertexBuffer() {
	// FOV/render distance viewer
	gk::Vertex vertices[3];
	vertices[0].coord3d[0] = 0.f;
	vertices[0].coord3d[1] = 0.f;

	vertices[1].coord3d[0] = -sin(glm::radians(Config::cameraFOV / 2.f)) * Config::renderDistance * (chunkSize + 2) / cos(glm::radians(Config::cameraFOV / 2.f));
	vertices[1].coord3d[1] = -(Config::renderDistance * (chunkSize + 2));

	vertices[2].coord3d[0] = sin(glm::radians(Config::cameraFOV / 2.f)) * Config::renderDistance * (chunkSize + 2) / cos(glm::radians(Config::cameraFOV / 2.f));
	vertices[2].coord3d[1] = -(Config::renderDistance * (chunkSize + 2));

	gk::Color color = gk::Color::Blue;
	for (u8 i = 0 ; i < 3 ; ++i) {
		vertices[i].color[0] = color.r;
		vertices[i].color[1] = color.g;
		vertices[i].color[2] = color.b;
		vertices[i].color[3] = color.a;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void Minimap::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_border, states);

	{
		gk::RenderStates states2 = states;
		states2.vertexAttributes = gk::VertexAttribute::All;
		states2.transform.translate(minimapSize / 2 + chunkSize / 2, minimapSize / 2 + chunkSize / 2);
		states2.transform *= m_playerFovRotationTransform;

		target.draw(m_vbo, GL_TRIANGLES, 0, 3, states2);
	}

	states.transform.translate(-m_playerChunkPos.x * (chunkSize + 2) + minimapSize / 2.f, m_playerChunkPos.y * (chunkSize + 2) + minimapSize / 2.f);

	for (auto &it : m_chunks)
		if (it.first.z == m_playerChunkPos.z)
			target.draw(it.second, states);

	target.draw(m_playerChunk, states);
}

