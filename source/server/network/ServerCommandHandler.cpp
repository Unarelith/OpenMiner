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
#include "AnimationComponent.hpp"
#include "BlockData.hpp"
#include "DrawableDef.hpp"
#include "NetworkComponent.hpp"
#include "PlayerList.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerCommandHandler.hpp"
#include "WorldController.hpp"

void ServerCommandHandler::sendServerClosed(const std::string &message, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::ServerClosed << message;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendBlockDataUpdate(s32 x, s32 y, s32 z, const BlockData *blockData, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::BlockDataUpdate << x << y << z
		<< blockData->meta << blockData->useAltTiles;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendBlockInvUpdate(s32 x, s32 y, s32 z, const Inventory &inventory, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::BlockInvUpdate << x << y << z << inventory;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendPlayerPosUpdate(u16 clientID, bool isTeleportation, const ClientInfo *client) const {
	const ServerPlayer *player = m_players.getPlayer(clientID);
	if (player) {
		Network::Packet packet;
		packet << Network::Command::PlayerPosUpdate;
		packet << clientID;
		packet << player->x() << player->y() << player->z();
		packet << isTeleportation;

		if (!client)
			m_server.sendToAllClients(packet);
		else
			client->tcpSocket->send(packet);
	}
	else
		gkError() << ("Failed to send position update for player " + std::to_string(clientID) + ": Player not found").c_str();
}

void ServerCommandHandler::sendPlayerRotUpdate(u16 clientID, const ClientInfo *client) const {
	const ServerPlayer *player = m_players.getPlayer(clientID);
	if (player) {
		Network::Packet packet;
		packet << Network::Command::PlayerRotUpdate;
		packet << clientID;
		packet << player->cameraYaw() << player->cameraPitch();

		if (!client)
			m_server.sendToAllClients(packet);
		else
			client->tcpSocket->send(packet);
	}
	else
		gkError() << ("Failed to send rotation update for player " + std::to_string(clientID) + ": Player not found").c_str();
}

void ServerCommandHandler::sendPlayerInvUpdate(u16 clientID, const ClientInfo *client) const {
	ServerPlayer *player = m_players.getPlayer(clientID);
	if (player) {
		Network::Packet packet;
		packet << Network::Command::PlayerInvUpdate;
		packet << clientID << player->inventory();

		if (!client)
			m_server.sendToAllClients(packet);
		else
			client->tcpSocket->send(packet);
	}
	else
		gkError() << ("Failed to send inventory update for player " + std::to_string(clientID) + ": Player not found").c_str();
}

void ServerCommandHandler::sendPlayerChangeDimension(u16 clientID, s32 x, s32 y, s32 z, u16 dimension, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::PlayerChangeDimension;
	packet << clientID << x << y << z << dimension;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);

	// FIXME: sendPlayerChangeDimension shouldn't be exposed to Lua
	//        Instead, there should be a world.changePlayerDimension function that sends
	//        the packet above + the entities (instead of doing that here)
	if (client)
		m_worldController.getWorld(dimension).scene().sendEntities(*client);
}

void ServerCommandHandler::sendChatMessage(u16 clientID, const std::string &message, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::ChatMessage << clientID << message;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntitySpawn(entt::entity entityID, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::EntitySpawn << entityID;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntityDespawn(entt::entity entityID, const ClientInfo *client) const {
	Network::Packet packet;
	packet << Network::Command::EntityDespawn << entityID;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::setupCallbacks() {
	m_server.setConnectionCallback([this](ClientInfo &client) {
		Network::Packet packet;
		packet << Network::Command::RegistryData;
		m_registry.serialize(packet);
		client.tcpSocket->send(packet);

		// Send already connected players to the new client
		for (auto &it : m_players) {
			Network::Packet spawnPacket;
			spawnPacket << Network::Command::PlayerSpawn << it.first;
			spawnPacket << it.second.x() << it.second.y() << it.second.z();
			client.tcpSocket->send(spawnPacket);
		}

		auto &player = m_players.addPlayer(client);
		player.setPosition(m_spawnPosition.x, m_spawnPosition.y, m_spawnPosition.z);

		m_scriptEngine.luaCore().onEvent(LuaEventType::PlayerConnected, glm::ivec3{m_spawnPosition.x, m_spawnPosition.y, m_spawnPosition.z}, player, client, *this);

		Network::Packet invPacket;
		invPacket << Network::Command::PlayerInvUpdate << client.id;
		invPacket << player.inventory();
		client.tcpSocket->send(invPacket);

		// Send spawn packet to all clients for this player
		Network::Packet spawnPacket;
		spawnPacket << Network::Command::PlayerSpawn << client.id;
		spawnPacket << m_spawnPosition.x << m_spawnPosition.y << m_spawnPosition.z;
		m_server.sendToAllClients(spawnPacket);

		// Send entities to the client
		m_worldController.getWorld(player.dimension()).scene().sendEntities(client);
	});

	m_server.setCommandCallback(Network::Command::ClientDisconnect, [this](ClientInfo &client, Network::Packet &) {
		m_players.removePlayer(client.id);
	});

	m_server.setCommandCallback(Network::Command::ChunkRequest, [this](ClientInfo &client, Network::Packet &packet) {
		s32 cx, cy, cz;
		packet >> cx >> cy >> cz;

		getWorldForClient(client.id).sendRequestedData(client, cx, cy, cz);
	});

	m_server.setCommandCallback(Network::Command::PlayerInvUpdate, [this](ClientInfo &client, Network::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			packet >> player->inventory();
		}
		else
			gkError() << ("Failed to update inventory of player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerPosUpdate, [this](ClientInfo &client, Network::Packet &packet) {
		double x, y, z;
		packet >> x >> y >> z;

		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			player->setPosition(x, y, z);
		}
		else
			gkError() << ("Failed to update position of player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerRotUpdate, [this](ClientInfo &client, Network::Packet &packet) {
		float yaw, pitch;
		packet >> yaw >> pitch;

		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			player->setRotation(yaw, pitch);
		}
		else
			gkError() << ("Failed to update rotation of player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerPlaceBlock, [this](ClientInfo &client, Network::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			s32 x, y, z;
			u32 block;
			packet >> x >> y >> z >> block;

			ServerWorld &world = getWorldForClient(client.id);
			world.setData(x, y, z, block >> 16);
			world.setBlock(x, y, z, block & 0xffff);
			const Block &blockDef = Registry::getInstance().getBlock(block & 0xffff);

			m_scriptEngine.luaCore().onEvent(LuaEventType::BlockPlaced, glm::ivec3{x, y, z}, blockDef, *player, world, client, *this);

			Network::Packet answer;
			answer << Network::Command::BlockUpdate << x << y << z << block;
			m_server.sendToAllClients(answer);
		}
		else
			gkError() << ("Failed to place block using player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerDigBlock, [this](ClientInfo &client, Network::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			s32 x, y, z;
			packet >> x >> y >> z;

			ServerWorld &world = getWorldForClient(client.id);
			const Block &blockDef = Registry::getInstance().getBlock(world.getBlock(x, y, z));
			world.setBlock(x, y, z, 0);

			m_scriptEngine.luaCore().onEvent(LuaEventType::BlockDigged, glm::ivec3{x, y, z}, blockDef, *player, world, client, *this);

			Network::Packet answer;
			answer << Network::Command::BlockUpdate << x << y << z << u32(0);
			m_server.sendToAllClients(answer);
		}
		else
			gkError() << ("Failed to dig block using player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerHeldItemChanged, [this](ClientInfo &client, Network::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			u8 hotbarSlot;
			u16 itemID;
			packet >> hotbarSlot >> itemID;
			if (player->inventory().getStack(hotbarSlot, 0).item().id() != itemID)
				gkWarning() << "PlayerHeldItemChanged:" << "Desync of item ID between client and server";

			player->setHeldItemSlot(hotbarSlot);
		}
		else
			gkError() << ("Failed to change held item of player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::BlockActivated, [this](ClientInfo &client, Network::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			s32 x, y, z;
			u16 screenWidth, screenHeight;
			u8 guiScale;
			packet >> x >> y >> z >> screenWidth >> screenHeight >> guiScale;

			ServerWorld &world = getWorldForClient(client.id);

			u16 id = world.getBlock(x, y, z);
			ServerBlock &block = (ServerBlock &)(m_registry.getBlock(id));
			bool hasBeenActivated = block.onBlockActivated({x, y, z}, *player, world, client, *this, screenWidth, screenHeight, guiScale);

			if (hasBeenActivated)
				m_scriptEngine.luaCore().onEvent(LuaEventType::BlockActivated, glm::ivec3{x, y, z}, block, *player, world, client, *this);
		}
		else
			gkError() << ("Failed to activate block using player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::BlockInvUpdate, [this](ClientInfo &client, Network::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = getWorldForClient(client.id).getBlockData(pos.x, pos.y, pos.z);
		if (data)
			packet >> data->inventory;
		else
			gkError() << "BlockInvUpdate: No block data found at" << pos.x << pos.y << pos.z;
	});

	m_server.setCommandCallback(Network::Command::BlockDataUpdate, [this](ClientInfo &client, Network::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = getWorldForClient(client.id).getBlockData(pos.x, pos.y, pos.z);
		if (data) {
			packet >> data->meta >> data->useAltTiles;
		}
	});

	m_server.setCommandCallback(Network::Command::ChatMessage, [this](ClientInfo &client, Network::Packet &packet) {
		std::string message;
		packet >> message;

		if (message[0] != '/' || (message.length() > 1 && message[1] == '/')) {
			if (message[0] == '/' && message.length() > 1 && message[1] == '/')
				sendChatMessage(client.id, message.substr(1));
			else
				sendChatMessage(client.id, message);
		}
		else {
			m_chatCommandHandler.parseCommand(message.substr(1), client);
		}
	});

	m_server.setCommandCallback(Network::Command::KeyPressed, [this](ClientInfo &client, Network::Packet &packet) {
		u16 keyID, screenWidth, screenHeight;
		u8 guiScale;
		packet >> keyID >> screenWidth >> screenHeight >> guiScale;

		m_registry.getKey(keyID).callback()(client, screenWidth, screenHeight, guiScale);
	});
}

void ServerCommandHandler::setPlayerPosition(u16 clientID, s32 x, s32 y, s32 z) {
	ServerPlayer *player = m_players.getPlayer(clientID);
	if (player)
		player->setPosition(x, y, z);
	else
		gkError() << ("Failed to set position for player " + std::to_string(clientID) + ": Player not found").c_str();
}

inline ServerWorld &ServerCommandHandler::getWorldForClient(u16 clientID) {
	ServerPlayer *player = m_players.getPlayer(clientID);
	if (!player)
		throw EXCEPTION("Player instance not found for client", clientID);

	return m_worldController.getWorld(player->dimension());
}

void ServerCommandHandler::stopServer() const {
	m_server.setRunning(false);
}

// Please update 'docs/lua-api-cpp.md' if you change this
void ServerCommandHandler::initUsertype(sol::state &lua) {
	lua.new_usertype<ServerCommandHandler>("ServerCommandHandler",
		"send_player_change_dimension", &ServerCommandHandler::sendPlayerChangeDimension,
		"send_chat_message", &ServerCommandHandler::sendChatMessage
	);
}

