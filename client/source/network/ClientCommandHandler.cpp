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
#include <gk/core/Debug.hpp>
#include <gk/gl/Camera.hpp>

#include "Client.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "ClientCommandHandler.hpp"
#include "LuaGUIState.hpp"
#include "Registry.hpp"

void ClientCommandHandler::sendPlayerInvUpdate() {
	sf::Packet invPacket;
	invPacket << Network::Command::PlayerInvUpdate;
	// FIXME: Sending client id shouldn't be necessary
	invPacket << m_client.id();
	invPacket << m_player.inventory();
	m_client.send(invPacket);
}

void ClientCommandHandler::sendPlayerPosUpdate() {
	sf::Packet packet;
	packet << Network::Command::PlayerPosUpdate;
	// FIXME: Sending client id shouldn't be necessary
	packet << m_client.id();
	packet << m_player.Player::x();
	packet << m_player.Player::y();
	packet << m_player.Player::z();
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerDigBlock(const glm::vec4 &selectedBlock) {
	sf::Packet packet;
	packet << Network::Command::PlayerDigBlock
		<< s32(selectedBlock.x)
		<< s32(selectedBlock.y)
		<< s32(selectedBlock.z);
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerPlaceBlock(s32 x, s32 y, s32 z, u32 block) {
	sf::Packet packet;
	packet << Network::Command::PlayerPlaceBlock << x << y << z << block;
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerInventoryRequest() {
	sf::Packet packet;
	packet << Network::Command::PlayerInventory;
	m_client.send(packet);
}

void ClientCommandHandler::sendBlockActivated(const glm::vec4 &selectedBlock) {
	sf::Packet packet;
	packet << Network::Command::BlockActivated
		<< s32(selectedBlock.x)
		<< s32(selectedBlock.y)
		<< s32(selectedBlock.z);
	m_client.send(packet);
}

void ClientCommandHandler::sendBlockInvUpdate(Inventory &inventory) {
	sf::Packet packet;
	packet << Network::Command::BlockInvUpdate;
	packet << s32(inventory.blockPos().x) << s32(inventory.blockPos().y) << s32(inventory.blockPos().z);
	packet << inventory;
	m_client.send(packet);
}

void ClientCommandHandler::sendChunkRequest(s32 chunkX, s32 chunkY, s32 chunkZ) {
	sf::Packet packet;
	packet << Network::Command::ChunkRequest;
	packet << chunkX << chunkY << chunkZ;
	m_client.send(packet);
}

void ClientCommandHandler::setupCallbacks() {
	m_client.setCommandCallback(Network::Command::RegistryData, [this](sf::Packet &packet) {
		Registry::getInstance().deserialize(packet);
		m_isRegistryInitialized = true;
	});

	m_client.setCommandCallback(Network::Command::ChunkData, [this](sf::Packet &packet) {
		m_world.receiveChunkData(packet);
	});

	m_client.setCommandCallback(Network::Command::BlockUpdate, [this](sf::Packet &packet) {
		s32 x, y, z;
		u32 block;
		packet >> x >> y >> z >> block;
		m_world.setBlock(x, y, z, block);
		m_world.setData(x, y, z, block >> 16);
	});

	m_client.setCommandCallback(Network::Command::PlayerInvUpdate, [this](sf::Packet &packet) {
		u16 clientId;
		packet >> clientId;

		if (clientId == m_client.id())
			packet >> m_player.inventory();
		else
			packet >> m_playerBoxes.at(clientId).inventory();
	});

	m_client.setCommandCallback(Network::Command::PlayerPosUpdate, [this](sf::Packet &packet) {
		s32 x, y, z;
		u16 clientId;
		packet >> clientId;
		packet >> x >> y >> z;

		if (clientId != m_client.id())
			m_playerBoxes.at(clientId).setPosition(x, y, z);
		// else {
		// 	m_camera.setPosition(x, y, z);
		// 	m_player.setPosition(x, y, z);
		// }
	});

	m_client.setCommandCallback(Network::Command::PlayerSpawn, [this](sf::Packet &packet) {
		u16 clientId;
		gk::Vector3<s32> pos;
		packet >> clientId >> pos.x >> pos.y >> pos.z;

		if (clientId != m_client.id()) {
			m_playerBoxes.emplace(clientId, PlayerBox{});
			m_playerBoxes.at(clientId).setPosition(pos.x, pos.y, pos.z);
			m_playerBoxes.at(clientId).setClientID(clientId);
		}

		m_world.init(pos.x, pos.y, pos.z);
	});

	m_client.setCommandCallback(Network::Command::BlockGUIData, [this](sf::Packet &packet) {
		gk::ApplicationStateStack::getInstance().push<LuaGUIState>(*this, m_player, m_world, packet, &gk::ApplicationStateStack::getInstance().top());
	});

	m_client.setCommandCallback(Network::Command::BlockInvUpdate, [this](sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = m_world.getBlockData(pos.x, pos.y, pos.z);
		if (!data)
			data = m_world.addBlockData(pos.x, pos.y, pos.z);

		if (data)
			packet >> data->inventory;
	});

	m_client.setCommandCallback(Network::Command::BlockDataUpdate, [this](sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		Chunk *chunk = m_world.getChunkAtBlockPos(pos.x, pos.y, pos.z);
		if (chunk) {
			BlockData *data = chunk->getBlockData(pos.x & (CHUNK_WIDTH - 1), pos.y & (CHUNK_HEIGHT - 1), pos.z & (CHUNK_DEPTH - 1));
			if (!data)
				data = m_world.addBlockData(pos.x, pos.y, pos.z);

			if (data) {
				bool useAltTiles;
				packet >> data->meta >> useAltTiles;

				if (data->useAltTiles != useAltTiles) {
					chunk->setChanged(true);
					data->useAltTiles = useAltTiles;
				}
			}
		}
	});
}

