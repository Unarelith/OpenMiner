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
#include <gk/core/Debug.hpp>
#include <gk/gl/Camera.hpp>

#include <entt/entt.hpp>

#include "AnimationComponent.hpp"
#include "Client.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "ClientCommandHandler.hpp"
#include "ConnectionErrorState.hpp"
#include "DrawableComponent.hpp"
#include "DrawableDef.hpp"
#include "LuaGUIState.hpp"
#include "NetworkComponent.hpp"
#include "PositionComponent.hpp"
#include "RotationComponent.hpp"
#include "Registry.hpp"

void ClientCommandHandler::sendPlayerInvUpdate() {
	Network::Packet invPacket;
	invPacket << Network::Command::PlayerInvUpdate;
	// FIXME: Sending client id shouldn't be necessary when sending this packet from client
	invPacket << m_client.id();
	invPacket << m_player.inventory();
	m_client.send(invPacket);
}

void ClientCommandHandler::sendPlayerPosUpdate() {
	Network::Packet packet;
	packet << Network::Command::PlayerPosUpdate;
	// FIXME: Sending client id shouldn't be necessary when sending this packet from client
	packet << m_client.id();
	packet << m_player.x();
	packet << m_player.y();
	packet << m_player.z();
	packet << false;
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerRotUpdate() {
	Network::Packet packet;
	packet << Network::Command::PlayerRotUpdate;
	// FIXME: Sending client id shouldn't be necessary when sending this packet from client
	packet << m_client.id();
	packet << m_player.cameraYaw();
	packet << m_player.cameraPitch();
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerDigBlock(const glm::ivec4 &selectedBlock) {
	Network::Packet packet;
	packet << Network::Command::PlayerDigBlock
		<< s32(selectedBlock.x)
		<< s32(selectedBlock.y)
		<< s32(selectedBlock.z);
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerPlaceBlock(s32 x, s32 y, s32 z, u32 block) {
	Network::Packet packet;
	packet << Network::Command::PlayerPlaceBlock << x << y << z << block;
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerHeldItemChanged(u8 hotbarSlot, u16 itemID) {
	Network::Packet packet;
	packet << Network::Command::PlayerHeldItemChanged
		<< hotbarSlot << itemID;
	m_client.send(packet);
}

void ClientCommandHandler::sendBlockActivated(const glm::ivec4 &selectedBlock) {
	Network::Packet packet;
	packet << Network::Command::BlockActivated
		<< s32(selectedBlock.x)
		<< s32(selectedBlock.y)
		<< s32(selectedBlock.z)
		<< u16(Config::screenWidth) << u16(Config::screenHeight) << u8(Config::guiScale);
	m_client.send(packet);
}

void ClientCommandHandler::sendBlockInvUpdate(Inventory &inventory) {
	Network::Packet packet;
	packet << Network::Command::BlockInvUpdate;
	packet << s32(inventory.blockPos().x) << s32(inventory.blockPos().y) << s32(inventory.blockPos().z);
	packet << inventory;
	m_client.send(packet);
}

void ClientCommandHandler::sendChunkRequest(s32 chunkX, s32 chunkY, s32 chunkZ) {
	Network::Packet packet;
	packet << Network::Command::ChunkRequest;
	packet << chunkX << chunkY << chunkZ;
	m_client.send(packet);
}

void ClientCommandHandler::sendChatMessage(const std::string &message) {
	Network::Packet packet;
	packet << Network::Command::ChatMessage;
	// FIXME: Sending client id shouldn't be necessary
	packet << m_client.id();
	packet << message;
	m_client.send(packet);
}

void ClientCommandHandler::sendKeyPressed(u16 keyID) {
	Network::Packet packet;
	packet << Network::Command::KeyPressed << keyID
		<< Config::screenWidth << Config::screenHeight << Config::guiScale;
	m_client.send(packet);
}

template<typename ComponentType>
static void addComponentCommandCallback(Network::Command command, Client &client, ClientCommandHandler::EntityMap &entityMap, ClientWorld &world) {
	client.setCommandCallback(command, [&](Network::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = entityMap.find(entityID);
		if (it != entityMap.end()) {
			auto &component = world.scene().registry().get_or_emplace<ComponentType>(it->second);
			component.deserialize(packet);
		}
		else
			gkError() << Network::commandToString(command) + ": Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});
}

void ClientCommandHandler::setupCallbacks() {
	m_client.setCommandCallback(Network::Command::ClientDisconnect, [this](Network::Packet &packet) {
		u16 clientID;
		packet >> clientID;

		auto it = m_playerBoxes.find(clientID);
		if (it != m_playerBoxes.end())
			m_playerBoxes.erase(it);
	});

	m_client.setCommandCallback(Network::Command::ServerClosed, [this](Network::Packet &packet) {
		std::string message;
		packet >> message;

		gk::ApplicationStateStack::getInstance().clear();
		gk::ApplicationStateStack::getInstance().push<ConnectionErrorState>(message, m_client.serverAddress().toString(), m_client.serverPort(), m_client.texturePack(), nullptr);
	});

	m_client.setCommandCallback(Network::Command::RegistryData, [this](Network::Packet &packet) {
		// FIXME: This is a quick fix for concurrency between client and server in singleplayer
		if (!m_isSingleplayer)
			Registry::getInstance().deserialize(packet);

		m_isRegistryInitialized = true;
	});

	m_client.setCommandCallback(Network::Command::ChunkData, [this](Network::Packet &packet) {
		m_world.receiveChunkData(packet);
	});

	m_client.setCommandCallback(Network::Command::BlockUpdate, [this](Network::Packet &packet) {
		s32 x, y, z;
		u32 block;
		packet >> x >> y >> z >> block;
		m_world.setBlock(x, y, z, block);
		m_world.setData(x, y, z, block >> 16);
	});

	m_client.setCommandCallback(Network::Command::PlayerInvUpdate, [this](Network::Packet &packet) {
		u16 clientId;
		packet >> clientId;

		if (clientId == m_client.id())
			packet >> m_player.inventory();
		else
			packet >> m_playerBoxes.at(clientId).inventory();
	});

	m_client.setCommandCallback(Network::Command::PlayerPosUpdate, [this](Network::Packet &packet) {
		double x, y, z;
		u16 clientId;
		bool isTeleportation;
		packet >> clientId;
		packet >> x >> y >> z;
		packet >> isTeleportation;

		if (clientId != m_client.id()) {
			auto it = m_playerBoxes.find(clientId);
			if (it != m_playerBoxes.end())
				it->second.setPosition(x, y, z);
		}
		else if (isTeleportation) {
			m_player.setPosition(x, y, z);
		}
	});

	m_client.setCommandCallback(Network::Command::PlayerRotUpdate, [this](Network::Packet &packet) {
		float yaw, pitch;
		u16 clientId;
		packet >> clientId;
		packet >> yaw >> pitch;

		if (clientId != m_client.id()) {
			auto it = m_playerBoxes.find(clientId);
			if (it != m_playerBoxes.end())
				it->second.Player::setRotation(yaw, pitch);
		}
	});

	m_client.setCommandCallback(Network::Command::PlayerSpawn, [this](Network::Packet &packet) {
		u16 clientId;
		gk::Vector3d pos;
		packet >> clientId >> pos.x >> pos.y >> pos.z;

		if (clientId != m_client.id()) {
			m_playerBoxes.emplace(clientId, PlayerBox{m_player.camera()});
			m_playerBoxes.at(clientId).setPosition(pos.x, pos.y, pos.z);
			m_playerBoxes.at(clientId).setClientID(clientId);
		}
		else {
			m_player.setPosition(pos.x, pos.y, pos.z);
		}
	});

	m_client.setCommandCallback(Network::Command::PlayerChangeDimension, [this](Network::Packet &packet) {
		u16 clientId, dimension;
		s32 x, y, z;
		packet >> clientId >> x >> y >> z >> dimension;

		if (clientId == m_client.id()) {
			m_player.setDimension(dimension);
			m_player.setPosition(x, y, z);
			m_world.clear();
			m_world.changeDimension(dimension);
			m_entityMap.clear();
		}
	});

	m_client.setCommandCallback(Network::Command::BlockGUIData, [this](Network::Packet &packet) {
		gk::ApplicationStateStack::getInstance().push<LuaGUIState>(*this, m_player, m_world, packet, &gk::ApplicationStateStack::getInstance().top());
	});

	m_client.setCommandCallback(Network::Command::BlockInvUpdate, [this](Network::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = m_world.getBlockData(pos.x, pos.y, pos.z);
		if (!data)
			data = m_world.addBlockData(pos.x, pos.y, pos.z);

		if (data)
			packet >> data->inventory;
	});

	m_client.setCommandCallback(Network::Command::BlockDataUpdate, [this](Network::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		Chunk *chunk = m_world.getChunkAtBlockPos(pos.x, pos.y, pos.z);
		if (chunk) {
			BlockData *data = chunk->getBlockData(pos.x & (CHUNK_WIDTH - 1), pos.y & (CHUNK_DEPTH - 1), pos.z & (CHUNK_HEIGHT - 1));
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

	m_client.setCommandCallback(Network::Command::EntitySpawn, [this](Network::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto &registry = m_world.scene().registry();

		auto it = m_entityMap.find(entityID);
		if (it == m_entityMap.end()) {
			entt::entity entity = registry.create();
			m_entityMap.emplace(entityID, entity);
			registry.emplace<NetworkComponent>(entity, entityID);
		}
		else if (registry.get<NetworkComponent>(it->second).entityID != entityID) {
			gkError() << "EntitySpawn: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
		}
	});

	m_client.setCommandCallback(Network::Command::EntityDespawn, [this](Network::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = m_entityMap.find(entityID);
		if (it != m_entityMap.end()) {
			m_world.scene().registry().destroy(it->second);
		}
		else
			gkError() << "EntityDespawn: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});

	addComponentCommandCallback<PositionComponent>(Network::Command::EntityPosition, m_client, m_entityMap, m_world);
	addComponentCommandCallback<RotationComponent>(Network::Command::EntityRotation, m_client, m_entityMap, m_world);
	addComponentCommandCallback<AnimationComponent>(Network::Command::EntityAnimation, m_client, m_entityMap, m_world);
	addComponentCommandCallback<DrawableDef>(Network::Command::EntityDrawableDef, m_client, m_entityMap, m_world);
}

