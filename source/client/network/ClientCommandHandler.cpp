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
#include "DrawableComponent.hpp"
#include "DrawableDef.hpp"
#include "LuaGUIState.hpp"
#include "NetworkComponent.hpp"
#include "PositionComponent.hpp"
#include "RotationComponent.hpp"
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
	packet << m_player.x();
	packet << m_player.y();
	packet << m_player.z();
	packet << false;
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerDigBlock(const glm::ivec4 &selectedBlock) {
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
	packet << Network::Command::PlayerInventory
		<< u16(Config::screenWidth) << u16(Config::screenHeight) << u8(Config::guiScale);
	m_client.send(packet);
}

void ClientCommandHandler::sendPlayerCreativeWindowRequest() {
	sf::Packet packet;
	packet << Network::Command::PlayerCreativeWindow
		<< u16(Config::screenWidth) << u16(Config::screenHeight) << u8(Config::guiScale);
	m_client.send(packet);
}

void ClientCommandHandler::sendBlockActivated(const glm::ivec4 &selectedBlock) {
	sf::Packet packet;
	packet << Network::Command::BlockActivated
		<< s32(selectedBlock.x)
		<< s32(selectedBlock.y)
		<< s32(selectedBlock.z)
		<< u16(Config::screenWidth) << u16(Config::screenHeight) << u8(Config::guiScale);
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

void ClientCommandHandler::sendChatMessage(const std::string &message) {
	sf::Packet packet;
	packet << Network::Command::ChatMessage;
	// FIXME: Sending client id shouldn't be necessary
	packet << m_client.id();
	packet << message;
	m_client.send(packet);
}

void ClientCommandHandler::setupCallbacks() {
	m_client.setCommandCallback(Network::Command::ClientDisconnect, [this](sf::Packet &packet) {
		u16 clientID;
		packet >> clientID;

		auto it = m_playerBoxes.find(clientID);
		if (it != m_playerBoxes.end())
			m_playerBoxes.erase(it);
	});

	m_client.setCommandCallback(Network::Command::RegistryData, [this](sf::Packet &packet) {
		// FIXME: This is a quick fix for concurrency between client and server in singleplayer
		if (!m_isSingleplayer)
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

	m_client.setCommandCallback(Network::Command::PlayerSpawn, [this](sf::Packet &packet) {
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

	m_client.setCommandCallback(Network::Command::PlayerChangeDimension, [this](sf::Packet &packet) {
		u16 clientId, dimension;
		s32 x, y, z;
		packet >> clientId >> x >> y >> z >> dimension;

		if (clientId == m_client.id()) {
			m_player.setDimension(dimension);
			m_player.setPosition(x, y, z);
			m_world.clear();
			m_world.updateSky(dimension);
		}
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

	m_client.setCommandCallback(Network::Command::EntitySpawn, [this](sf::Packet &packet) {
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

	m_client.setCommandCallback(Network::Command::EntityDespawn, [this](sf::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = m_entityMap.find(entityID);
		if (it != m_entityMap.end()) {
			m_world.scene().registry().destroy(it->second);
		}
		else
			gkError() << "EntityDespawn: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});

	m_client.setCommandCallback(Network::Command::EntityPosition, [this](sf::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = m_entityMap.find(entityID);
		if (it != m_entityMap.end()) {
			auto &position = m_world.scene().registry().get_or_emplace<PositionComponent>(it->second);
			packet >> position.x >> position.y >> position.z;
		}
		else
			gkError() << "EntityPosition: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});

	m_client.setCommandCallback(Network::Command::EntityRotation, [this](sf::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = m_entityMap.find(entityID);
		if (it != m_entityMap.end()) {
			float w, x, y, z;
			packet >> w >> x >> y >> z;

			auto &rotation = m_world.scene().registry().get_or_emplace<RotationComponent>(it->second);
			rotation.quat = glm::quat(w, x, y, z);
		}
		else
			gkError() << "EntityRotation: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});

	m_client.setCommandCallback(Network::Command::EntityAnimation, [this](sf::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = m_entityMap.find(entityID);
		if (it != m_entityMap.end()) {
			auto &animation = m_world.scene().registry().get_or_emplace<AnimationComponent>(it->second);
			animation.deserialize(packet);
		}
		else
			gkError() << "EntityAnimation: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});

	m_client.setCommandCallback(Network::Command::EntityDrawableDef, [this](sf::Packet &packet) {
		entt::entity entityID;
		packet >> entityID;

		auto it = m_entityMap.find(entityID);
		if (it != m_entityMap.end()) {
			packet >> m_world.scene().registry().get_or_emplace<DrawableDef>(it->second);
		}
		else
			gkError() << "EntityDrawableDef: Entity ID" << std::underlying_type_t<entt::entity>(entityID) << "is invalid";
	});
}

